#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "logger.h"


static int write_result_to_log(const char *p_situation, const char *p_file_name, const int *p_inputs, int number_of_inputs, const char *p_buffer)
{
    FILE *fptr;
    fptr = fopen(p_file_name, "a+");
    if (fptr == NULL)
    {
        printf("[CLIENT][ERROR] An error happened while trying to open log file.");
        return (-1);
    }

    fprintf(fptr, "%s", p_situation);
    for (int i = 0; i < number_of_inputs; i++)
    {
        fprintf(fptr, "<%d>", p_inputs[i]);
    }
    fprintf(fptr, ": %s", p_buffer);
    fclose(fptr);
    return 0;
}

int write_success_result_to_log(const char *p_file_name, const int *p_inputs, int number_of_inputs, const char *p_buffer)
{
    assert(p_file_name != NULL);
    assert(p_buffer != NULL);
    assert(p_inputs != NULL || number_of_inputs == 0); // if number_of_inputs is zero then p_inputs can be null.
    assert(number_of_inputs >= 0);

    return write_result_to_log("SUCCESS:\n", p_file_name, p_inputs, number_of_inputs, p_buffer);
}

int write_failure_result_to_log(const char *p_file_name, const int *p_inputs, int number_of_inputs, const char *p_buffer)
{
    assert(p_file_name != NULL);
    assert(p_buffer != NULL);
    assert(p_inputs != NULL || number_of_inputs == 0); // if number_of_inputs is zero then p_inputs can be null.
    assert(number_of_inputs >= 0);

    return write_result_to_log("FAIL:\n", p_file_name, p_inputs, number_of_inputs, p_buffer);
}
