#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "logger.h"
#include "ip_addr_utility.h"

#include "logger_data.h"

logger_server_info_t g_logger_server_info;

static int connect_to_logger_server(const char *p_server_name, unsigned int server_port, CLIENT **client)
{

    struct sockaddr_in serveraddr;
    memset(&serveraddr, 0, sizeof(serveraddr));

    resolve_ip_addr_from_host_name(p_server_name, &serveraddr);
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(server_port);

    int clnt_sock = RPC_ANYSOCK;
    (*client) = clnttcp_create(&serveraddr,
                               LOGGER_DATA, LOGGER_DATA_V1,
                               &clnt_sock, 0, 0);
    if ((*client) == NULL)
    {
        return (1);
    }

    return 0;
}

static int disconnect_from_logger_server(CLIENT *client)
{
    clnt_destroy(client);
}

static int logger_info_to_string(const log_info_t *p_log_info, char *p_buffer, size_t buffer_size)
{

    if (LOG_STATUS_SUCCESS == p_log_info->log_status)
    {
        int ret = snprintf(p_buffer, buffer_size, "<%d> <%d> <%d>", p_log_info->input1, p_log_info->input2, p_log_info->output);
        if (ret >= buffer_size)
        {
            return (-1);
        }
    }
    else
    {
        int ret = snprintf(p_buffer, buffer_size, "<%d> <%d> <_>", p_log_info->input1, p_log_info->input2);
        if (ret >= buffer_size)
        {
            return (-1);
        }
    }
    return 0;
}

int logger_send_log_message(const logger_server_info_t *p_logger_server_info, const log_info_t *p_log_info)
{
    assert(p_logger_server_info != NULL);
    assert(p_log_info != NULL);

    char log_buffer[128];
    logger_info_to_string(p_log_info, log_buffer, sizeof(log_buffer));

    logger_info my_logger_info = {
        .log_string = log_buffer,
    };

    CLIENT *client;
    if (connect_to_logger_server(p_logger_server_info->server_name, p_logger_server_info->server_port, &client) != 0)
    {
        printf("[SERVER][ERROR] Could not connect to the logger server: %s:%d\n", p_logger_server_info->server_name, p_logger_server_info->server_port);
        return (-1);
    }

    int *result = save_log_1(&my_logger_info, client);
    if (result == NULL)
    {
        printf("[SERVER][ERROR] RPC failed while trying to send log message to logger server.\n");
        disconnect_from_logger_server(client);
        return *result;
    }

    disconnect_from_logger_server(client);
    return *result;
}

void logger_set_server_info(const char *p_server_name, unsigned int server_port)
{
    assert(p_server_name != NULL);

    strncpy(g_logger_server_info.server_name, p_server_name, LOGGER_MAX_SERVER_NAME);
    g_logger_server_info.server_port = server_port;
}

void logger_get_server_info(logger_server_info_t *p_logger_server_info)
{
    assert(p_logger_server_info != NULL);

    strncpy(p_logger_server_info->server_name, g_logger_server_info.server_name, LOGGER_MAX_SERVER_NAME);
    p_logger_server_info->server_port = g_logger_server_info.server_port;
}
