#include <stdio.h>
#include "logger_data.h"
#include "logger_server.h"

int g_result;

int *save_log_1_svc(logger_info *p_log_info, struct svc_req *svc_req)
{

    printf("[LOGGER][INFO] String to registered : %s\n", p_log_info->log_string);
    g_result = write_result_to_log_file(p_log_info->log_string);
    return &g_result;
}