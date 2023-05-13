
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "logger.h"

static char g_log_file_name[100];
static int g_is_log_file_name_set = 0;

/**
 * @brief Set the log file name
 *
 * @param p_log_file_name Name of the log file
 *
 * @warning Before calling @c write_result_to_log_file this function must be called.
 */
void set_log_file_name(const char *p_log_file_name)
{
    strncpy(g_log_file_name, p_log_file_name, sizeof(g_log_file_name));
    g_is_log_file_name_set = 1;
    printf("[LOGGER][INFO] Logger file name is set as %s.\n", g_log_file_name);
}

/**
 * @brief Append and wrtie the log result to already configured log file.
 *
 * @param p_log The log string to be written to file
 * @return int Returns 0 on success, -1 on failure
 */
int write_result_to_log_file(const char *p_log)
{

    if (1 == g_is_log_file_name_set)
    {
        FILE *fptr;
        fptr = fopen(g_log_file_name, "a+");
        if (fptr == NULL)
        {
            printf("[LOGGER][ERROR] Could not open the file named %s\n", g_log_file_name);
            return (-1);
        }

        fprintf(fptr, "%s\n", p_log);
        fclose(fptr);
        return 0;
    }

    return (-1);
}
