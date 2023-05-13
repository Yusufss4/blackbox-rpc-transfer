#ifndef BLACKBOX_SEND_H
#define BLACKBOX_SEND_H

#include <stdint.h>

/**
 * @brief Send given binary executable file given the exact path via RPC. 
 *        The RPC will be done with seperated requests according to the packet_size 
 *        and size of the binary file. 
 * 
 * @param[in] client Client that will binary file will going to be send. Client must be already created.
 * @param[in] p_blackbox_path Exact path of the binary executable file. 
 * @param[in] packet_size Packet size to determine how many bytes will be send in each request. 
 *                    The number of requests will be determined by the size of the binary file and 
 *                    packet size parameter. 
 * @return int 0 if SUCCESS else if FAILURE
 */
int send_blackbox(CLIENT *client, const char *p_blackbox_path, size_t packet_size);

#endif