#ifndef POPEN3_H
#define POPEN3_H

#include <stdio.h>

typedef struct
{
    FILE *in;
    FILE *out;
    FILE *err;
} stdpipes_t;

/**
 * @brief Opens a process by creating a pipe, forking, and invoking the shell.
 *
 * @param[out] p_stdpipes Pointer to struct containing standard pipes
 * @param[in] p_command Command to be executed
 * @param[in] p_command_arg Arguments for command to be executed
 * @return int Returns 0 on success and a non-zero value on failure.
 */
int popen3(stdpipes_t *p_stdpipes, const char *p_command, const char *p_command_arg);

#endif