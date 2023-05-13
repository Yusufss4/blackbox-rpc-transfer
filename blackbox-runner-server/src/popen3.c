// #define _XOPEN_SOURCE 600
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>

#include "popen3.h"

#define POPEN3_WRITE_END 1
#define POPEN3_READ_END 0

static void cleanup(int pipe_pair[2])
{
    close(pipe_pair[0]);
    close(pipe_pair[1]);
}

int popen3(stdpipes_t *p_stdpipes, const char *p_command, const char *p_command_arg)
{
    assert(p_stdpipes != NULL);
    assert(p_command != NULL);

    p_stdpipes->out = NULL;
    p_stdpipes->in = NULL;
    p_stdpipes->err = NULL;

    int stdout_pair[2];
    int stderr_pair[2];
    int stdin_pair[2];
    pid_t pid;

    // open pipe for stdout
    if (pipe(stdout_pair) < 0)
    {
        return (-1);
    }

    // open pipe for stderr
    if (pipe(stderr_pair) < 0)
    {
        cleanup(stdin_pair);
        return (-1);
    }

    // open pipe for stdin
    if (pipe(stdin_pair) < 0)
    {
        cleanup(stdin_pair);
        cleanup(stderr_pair);
        return (-1);
    }

    pid = fork(); // clone the process
    if (pid < 0)
    {
        cleanup(stdin_pair);
        cleanup(stderr_pair);
        cleanup(stdin_pair);
        return (-1);
    }

    if (0 == pid)
    {
        // Child process

        // duplicate write end of the pipe to stdout.
        // the child processes stdout is not connected to the pipes write end.
        dup2(stdout_pair[POPEN3_WRITE_END], STDOUT_FILENO);
        cleanup(stdout_pair);

        // duplicate write end of the pipe to stderr
        dup2(stderr_pair[POPEN3_WRITE_END], STDERR_FILENO);
        cleanup(stderr_pair);

        // duplicate read end of the pipe to stdin
        dup2(stdin_pair[POPEN3_READ_END], STDIN_FILENO);
        cleanup(stdin_pair);

        execl(p_command, "", NULL);
        // if execl fails, return command not found error.
        exit(127);
    }

    // associate an input or output stream with the file descriptor that is identified by handle.
    p_stdpipes->out = fdopen(stdout_pair[POPEN3_READ_END], "r");
    close(stdout_pair[POPEN3_WRITE_END]);

    p_stdpipes->err = fdopen(stderr_pair[POPEN3_READ_END], "r");
    close(stderr_pair[POPEN3_WRITE_END]);

    p_stdpipes->in = fdopen(stdin_pair[POPEN3_WRITE_END], "w");
    close(stdin_pair[POPEN3_READ_END]);

    return 0;
}