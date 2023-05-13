#ifndef IP_ADDR_UTILITY_H
#define IP_ADDR_UTILITY_H

struct sockaddr_in; // forward declaration of sockaddr_in

/**
 * @brief Resolves the IP address from the given hostname and store it in a sockaddr_in structure.
 *
 * @param p_hostname The hostname for which IP address is to be resolved.
 * @param ipv4 A pointer to a sockaddr_in structure to store the resolved IP address.
 * @return int Returns 0 on success and a non-zero value on failure.
 */
int resolve_ip_addr_from_host_name(const char *p_hostname, struct sockaddr_in *ipv4);

#endif