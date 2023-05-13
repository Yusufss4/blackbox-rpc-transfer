#include <stdio.h>
#include "blackbox_data.h"
#include "blackbox_send.h"
#include <stddef.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <inttypes.h>
#include <assert.h>

#include "progress_bar.h"
#include "logger.h"

#define NUM_SECS 100

#define NUMBER_OF_INPUTS 2 //< @todo In the current setting only 2 is supported.
#define MAX_NUM_OF_INPUTS NUMBER_OF_INPUTS

#define BLACKBOX_PART_SIZE_BYTE 1000

/**
 * @brief Get the inputs from the user via stdin and check the validity of them.
 *
 * @param[out] inputs Input array to be filled with given inputs from the user if succesfull.
 * @param[in] num_of_inputs Number of inputs are expected from the user.
 * @return int 0 if SUCCESS, else FAILURE
 */
static int get_input_from_user(int inputs[], size_t num_of_inputs)
{
    assert(inputs != NULL);
    assert(num_of_inputs > 0 && num_of_inputs <= MAX_NUM_OF_INPUTS);

    char str[20];

    for (int i = 0; i < num_of_inputs; i++)
    {
        printf("[CLIENT] Enter the %d.input: ", i + 1);
        if (fgets(str, sizeof(str), stdin) == NULL)
        {
            printf("[CLIENT][ERROR] Reading input\n");
            return 1;
        }

        char *endptr;
        errno = 0;
        const long strtol_res = strtol(str, &endptr, 10);

        if (endptr == str)
        {
            printf("[CLIENT][ERROR] Given input is not a decimal number\n");
            return 1;
        }
        if ((LONG_MIN == strtol_res || LONG_MAX == strtol_res) && ERANGE == errno)
        {
            printf("[CLIENT][ERROR] Given input is out of range of type long\n");
            return 1;
        }
        if (strtol_res > INT_MAX)
        {
            printf("[CLIENT][ERROR] Given input is greater than INT_MAX\n");
            return 1;
        }
        if (strtol_res < INT_MIN)
        {
            printf("[CLIENT][ERROR] Given input is less than INT_MIN\n");
            return 1;
        }

        inputs[i] = (int)strtol_res;
    }

    return 0;
}

int main(int argc, char *argv[])
{

    if (argc != 4)
    {
        printf("[CLIENT] Number of arguments are wrong.Use <blacboxpath> <filename.txt> <server_name>\n");
        exit(EXIT_FAILURE);
    }

    const char *p_blackbox_path = argv[1];
    const char *p_log_filename = argv[2];
    const char *p_server_name = argv[3];

    CLIENT *client = clnt_create(p_server_name, BLACKBOX_DATA, BLACKBOX_DATA_V1, "tcp");
    if (client == NULL)
    {
        printf("[CLIENT][ERROR] Could not connect to the blackbox runner server.\n");
        exit(EXIT_FAILURE);
    }

    // Send the blackbox to the runner server.
    int ret = send_blackbox(client, p_blackbox_path, BLACKBOX_PART_SIZE_BYTE);
    printf("[CLIENT][INFO] Blackbox transfer RPC result: %d\n", ret);
    if (ret != 0)
    {
        printf("[CLIENT][ERROR] Blackbox RPC file transfer failed.\n");
        exit(EXIT_FAILURE);
    }

    // Enter the inputs for the blackbox.
    int inputs[NUMBER_OF_INPUTS] = {0};
    ret = get_input_from_user(inputs, NUMBER_OF_INPUTS);
    if (ret != 0)
    {
        printf("[CLIENT][ERROR] Error occured while getting inputs from the user.\n");
        exit(EXIT_FAILURE);
    }

    blackbox_run_info my_blackbox_info = {
        .input1 = inputs[0],
        .input2 = inputs[1],
    };

    blackbox_response *my_blackbox_response = run_blackbox_1(&my_blackbox_info, client);
    if (my_blackbox_response == NULL)
    {
        printf("[CLIENT][ERROR] Blackbox runner RPC failed!\n");
        return 1;
    }
    printf("[CLIENT][INFO] Blackbox process result from server : %d\n", my_blackbox_response->process_result);
    if (my_blackbox_response->process_result != 0)
    {
        printf("[CLIENT][INFO] Blackbox standard error : %s", my_blackbox_response->std_err);
        write_failure_result_to_log(p_log_filename, inputs, NUMBER_OF_INPUTS, my_blackbox_response->std_err);
    }
    else
    {
        printf("[CLIENT][INFO] Blackbox standard output : %s", my_blackbox_response->std_out);
        write_success_result_to_log(p_log_filename, inputs, NUMBER_OF_INPUTS, my_blackbox_response->std_out);
    }
    return 0;
}