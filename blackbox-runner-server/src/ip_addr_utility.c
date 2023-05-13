#include <string.h>
#include <stdio.h>
#include <assert.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

#include "ip_addr_utility.h"

int resolve_ip_addr_from_host_name(const char *hostname, struct sockaddr_in *ipv4)
{
    assert(hostname != NULL);
    assert(ipv4 != NULL);

    struct addrinfo hints;
    struct addrinfo *result;
    struct addrinfo *p;
    char ipstr[INET6_ADDRSTRLEN];

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET; // AF_INET or AF_INET6 to force version

    int status = getaddrinfo(hostname, NULL, &hints, &result);
    if (status != 0)
    {
        printf("[SERVER][ERROR]getaddrinfo error: %s\n", gai_strerror(status));
        return 1;
    }

    for (p = result; p != NULL; p = p->ai_next)
    {
        void *addr;
        memcpy(ipv4, (struct sockaddr_in *)p->ai_addr, sizeof(struct sockaddr_in));
        addr = &(ipv4->sin_addr);

        // convert the IP to a string and print it
        inet_ntop(p->ai_family, addr, ipstr, sizeof ipstr);
        printf("[SERVER][INFO] Logger ip address resolved to: %s\n", ipstr);
        break;
    }

    freeaddrinfo(result); // free the linked list

    return 0;
}