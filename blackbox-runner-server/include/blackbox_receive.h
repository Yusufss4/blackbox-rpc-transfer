#ifndef BLACKBOX_RECEIVE_H
#define BLACKBOX_RECEIVE_H

#include <stdint.h>

#include "blackbox_data.h"

/**
 * @brief Prints the information of a given packet to the stdout.
 *
 * This function prints the packet information to the stdout, including its packet lenght,
 * status of transfer, total binary lenght.
 *
 *
 * @param p_part_info Pointer to the blackbox_part_info struct containing packet information.
 * @return Returns 0 on success and a non-zero value on failure.
 */
int print_packet_information(const blackbox_part_info *p_part_info);

/**
 * @brief Opens the blackbox file for writing.
 *
 * This function opens the blackbox file with the given filename in write mode.
 *
 * @param p_blackbox_file_name Pointer to a string containing the blackbox filename.
 * @return Returns a FILE pointer to the opened file if successful, otherwise returns NULL.
 */
FILE *open_blackbox_file(const char *p_blackbox_file_name);

/**
 * @brief Closes the blackbox file.
 *
 * This function closes the given blackbox file.
 *
 * @param p_file Pointer to a FILE struct containing the open file.
 * @return Returns 0 on success and a non-zero value on failure.
 */
int close_blackbox_file(FILE *p_file);

/**
 * @brief Adds a binary packet to the blackbox file.
 *
 * This function adds a packet to the given blackbox file with the provided packet information.
 *
 * @param p_blackbox_file Pointer to a FILE struct containing the open file.
 * @param p_part_info Pointer to the blackbox_part_info struct containing packet information.
 * @return Returns the number of bytes written to the file if successful, otherwise returns 0.
 */
size_t add_packet_to_blackbox_file(FILE *p_blackbox_file, const blackbox_part_info *p_part_info);

#endif