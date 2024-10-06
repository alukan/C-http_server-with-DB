#ifndef SERVER_H
#define SERVER_H

#include <netinet/in.h>
#include "config.h"  // Include configuration constants

int create_server_socket(int port);

int accept_connection(int server_fd, struct sockaddr_in *address, int *addrlen);

#endif // SERVER_H
