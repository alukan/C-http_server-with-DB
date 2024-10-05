#ifndef SERVER_H
#define SERVER_H

#include <netinet/in.h>
#include "config.h"  // Include configuration constants

int create_server_socket(int port);

// Function to accept an incoming connection
int accept_connection(int server_fd, struct sockaddr_in *address, int *addrlen);

// Function to send an HTTP response
void send_response(int socket, const char *response);

// Helper function to extract a number from a URI
int extract_num_from_uri(const char *uri, const char *operation);

#endif // SERVER_H
