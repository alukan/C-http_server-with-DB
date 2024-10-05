#include "server.h"
#include "request_handler.h"
#include "config.h"
#include "db.h"
#include <stdio.h>
#include <unistd.h>

#define PORT 8080

int main() {
    load_config();
    init_mongo_connection();
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    // Create and bind the server socket
    server_fd = create_server_socket(PORT);

    // Continuously accept and handle incoming connections
    while (1) {
        new_socket = accept_connection(server_fd, &address, &addrlen);
        handle_request(new_socket);
    }

    // Close the server socket when the loop is done
    close(server_fd);
    return 0;
}
