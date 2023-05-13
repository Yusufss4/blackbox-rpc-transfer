#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>

#include "progress_bar.h"

#define PERCENTAGE(V, T) (100 - (((T - V) * 100) / T))
#define BAR_WIDTH 50

void print_progress(const char *bar_name, size_t count, size_t max)
{
    assert(bar_name != NULL);

    const int bar_width = BAR_WIDTH;
    int percent = 100 * count / max;
    int num_chars = bar_width * count / max;
    printf("\r%s : [", bar_name);
    for (int i = 0; i < num_chars; ++i)
    {
        printf("#");
    }
    for (int i = num_chars; i < bar_width; ++i)
    {
        printf(" ");
    }
    printf("] %d%%", percent);
    fflush(stdout);

    // add '\n' when the progress is finished.
    if (count == max)
    {
        printf("\n");
    }
}