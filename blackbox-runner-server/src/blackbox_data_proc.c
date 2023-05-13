#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

#include "blackbox_data.h"
#include "popen3.h"
#include "logger_data.h"

#include "logger.h"

#include "blackbox_receive.h"

#define RCP_RESPONSE_STDIO_BUFSIZE 128
#define STDIO_BUFSIZE RCP_RESPONSE_STDIO_BUFSIZE
#define NUMBER_OF_INPUTS 2
#define NUM_CHILDREN 1

#define BLACKBOX_NAME "recreated_blackbox"

blackbox_response g_blackbox_response;
int g_send_blackbox_1_svc_result;

static int send_success_to_log_server(const logger_server_info_t *p_logger_server_info, const blackbox_run_info *p_blackbox, const char *p_buffer)
{
    log_info_t log_info;

    log_info.log_status = LOG_STATUS_SUCCESS;
    log_info.input1 = p_blackbox->input1;
    log_info.input2 = p_blackbox->input2;
    log_info.output = strtol(p_buffer, NULL, 10);
    return (logger_send_log_message(p_logger_server_info, &log_info));
}

static int send_failure_to_log_server(const logger_server_info_t *p_logger_server_info, const blackbox_run_info *p_blackbox)
{
    log_info_t log_info;

    log_info.log_status = LOG_STATUS_FAILURE;
    log_info.input1 = p_blackbox->input1;
    log_info.input2 = p_blackbox->input2;
    return (logger_send_log_message(p_logger_server_info, &log_info));
}

static int get_std_io_from_process(char *p_buffer, FILE *p_io)
{
    int is_read = 0;
    while (fgets(p_buffer, STDIO_BUFSIZE, p_io))
    {
        is_read = 1;
    }

    return (!is_read);
}

blackbox_response *run_blackbox_1_svc(blackbox_run_info *p_blackbox, struct svc_req *req)
{
    const char *p_blackbox_name = BLACKBOX_NAME; // TODO

    printf("[SERVER][INFO] Blackbox 1.input : %d \n", p_blackbox->input1);
    printf("[SERVER][INFO] Blackbox 2.input : %d \n", p_blackbox->input2);

    // Allocate memory for rcp response
    g_blackbox_response.std_out = malloc(sizeof(char) * (RCP_RESPONSE_STDIO_BUFSIZE));
    if (NULL == g_blackbox_response.std_out)
    {
        return NULL;
    }
    g_blackbox_response.std_err = malloc(sizeof(char) * (RCP_RESPONSE_STDIO_BUFSIZE));
    if (NULL == g_blackbox_response.std_err)
    {
        return NULL;
    }

    // Open a child process for blackbox and pipes for communication.
    stdpipes_t process_pipes;
    int err = popen3(&process_pipes, p_blackbox_name, "");
    if (0 != err)
    {
        printf("[SERVER][ERROR] An error occured while trying to run the blackbox.\n");
        g_blackbox_response.process_result = err;
        return &g_blackbox_response;
    }

    // Pass the inputs to the blackbox.
    fprintf(process_pipes.in, "%d\n", p_blackbox->input1);
    fprintf(process_pipes.in, "%d\n", p_blackbox->input2);
    fclose(process_pipes.in);

    char buffer[STDIO_BUFSIZE];

    // Get logger server info.
    logger_server_info_t logger_server_info;
    logger_get_server_info(&logger_server_info);

    // Get the outputs from the blackbox.
    if (!get_std_io_from_process(buffer, process_pipes.out))
    {
        strncpy(g_blackbox_response.std_out, buffer, STDIO_BUFSIZE);
        printf("[SERVER][INFO] Blackbox standard out : %s", buffer); // the message also consist '\n'.
        int ret = send_success_to_log_server(&logger_server_info, p_blackbox, g_blackbox_response.std_out);
        // printf("[SERVER] Success Message Logger server returned : %d\n", ret);
    }

    memset(buffer, '\0', sizeof(buffer));

    if (!get_std_io_from_process(buffer, process_pipes.err))
    {
        strncpy(g_blackbox_response.std_err, buffer, STDIO_BUFSIZE);
        printf("[SERVER][INFO] Blackbox standard error : %s\n", buffer);
        int ret = send_failure_to_log_server(&logger_server_info, p_blackbox);
        // printf("[SERVER] Failure Message Logger server returned : %d\n", ret);
    }

    // Wait for process to finish.
    for (int i = 0; i < NUM_CHILDREN; i++)
    {
        int status = 0;
        pid_t childpid = wait(&status);
        printf("[SERVER][INFO] Process child %d done. The exit status from the blackbox is %d\n\n", (int)childpid, status);

        // Because we have only one process, the status is the blackbox process result.
        g_blackbox_response.process_result = status;
    }

    return &g_blackbox_response;
}

int *send_blackbox_1_svc(blackbox_part_info *p_part_info, struct svc_req *req)
{
    static FILE *g_blackbox_file;
    static size_t byte_written = 0;
    static int is_start_applied = 0;

    const char *blackbox_name = BLACKBOX_NAME;

    int transfer_status = p_part_info->transfer_status;
    switch (transfer_status)
    {
    case BLACKBOX_TRANSFER_START:
        is_start_applied = 1;
        g_blackbox_file = open_blackbox_file(blackbox_name);
        if (g_blackbox_file)
        {
            byte_written += add_packet_to_blackbox_file(g_blackbox_file, p_part_info);
            // printf("[SERVER] byte_written %lu , p_part_info->total_size %u\n", byte_written, p_part_info->total_size);
        }
        else
        {
            // could not open blackbox file
            g_send_blackbox_1_svc_result = 1;
            return &g_send_blackbox_1_svc_result;
        }
        if (byte_written == p_part_info->total_size)
        {
            byte_written = 0;
            is_start_applied = 0;
            if (0 != close_blackbox_file(g_blackbox_file))
            {
                g_send_blackbox_1_svc_result = 1;
                return &g_send_blackbox_1_svc_result;
            }
        }
        break;

    case BLACKBOX_TRANSFER_PROGRESS:
        if (0 == is_start_applied)
        {
            printf("[SERVER][ERROR] BLACKBOX_TRANSFER_START is not send as a first packet.\n");
            g_send_blackbox_1_svc_result = 1;
            return &g_send_blackbox_1_svc_result;
        }

        byte_written += add_packet_to_blackbox_file(g_blackbox_file, p_part_info);
        // printf("[SERVER] byte_written %lu , p_part_info->total_size %u\n", byte_written, p_part_info->total_size);
        if (byte_written == p_part_info->total_size)
        {
            byte_written = 0;
            is_start_applied = 0;
            if (0 != close_blackbox_file(g_blackbox_file))
            {
                g_send_blackbox_1_svc_result = 1;
                return &g_send_blackbox_1_svc_result;
            }
        }
        break;

    case BLACKBOX_TRANSFER_CANCEL:
        // delete_old_blackbox_file(g_blackbox_file);
        break;

    default:
        // delete_old_blackbox_file(g_blackbox_file);
        byte_written = 0;
        is_start_applied = 0;
        close_blackbox_file(g_blackbox_file);
        printf("[SERVER][ERROR] Blackbox Transfer Send Status Operator is not correct.\n");
        g_send_blackbox_1_svc_result = 1;
        return &g_send_blackbox_1_svc_result;
    }

    g_send_blackbox_1_svc_result = 0;
    return &g_send_blackbox_1_svc_result;
}