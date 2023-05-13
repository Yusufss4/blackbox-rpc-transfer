
#ifndef PROGRESS_BAR_H
#define PROGRESS_BAR_H

/**
 * @brief Prints progress bar for a given count, and the max count.
 *
 * @example <bar_name> : [##################################################] 100%
 * @param bar_name The name of the bar to be displayed.
 * @param count The current count value for the progress.
 * @param max The maximum count value for the progress.
 *
 */
void print_progress(const char *bar_name, size_t count, size_t max);

#endif