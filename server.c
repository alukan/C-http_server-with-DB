#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 30000

// Function to create and bind the server socket
int create_server_socket() {
    int server_fd;
    struct sockaddr_in address;
    int opt = 1;

    // Create socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Set socket options to allow address reuse (optional, but useful)
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Define the address for the socket
    address.sin_family = AF_INET;  // IPv4
    address.sin_addr.s_addr = INADDR_ANY;  // Any incoming interface
    address.sin_port = htons(PORT);  // Port to bind the server

    // Bind the socket to the specified port and address
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Start listening on the socket for incoming connections
    if (listen(server_fd, 3) < 0) {
        perror("listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on port %d\n", PORT);
    return server_fd;
}

// Function to accept an incoming connection
int accept_connection(int server_fd, struct sockaddr_in *address, int *addrlen) {
    int new_socket;
    if ((new_socket = accept(server_fd, (struct sockaddr *)address, (socklen_t*)addrlen)) < 0) {
        perror("accept failed");
        exit(EXIT_FAILURE);
    }
    printf("Connection established\n");
    return new_socket;
}

void send_response(int socket, const char *response) {
    write(socket, response, strlen(response));
    printf("Response sent\n");
}

void handle_request(int new_socket) {
    char buffer[BUFFER_SIZE] = {0};
    const char *http_response = 
        "HTTP/1.1 200 OK\n"
        "Content-Type: text/html\n"
        "Content-Length: 39\n\n"
        "<html><body>Hello, World!</body></html>";

    read(new_socket, buffer, BUFFER_SIZE);
    printf("Received request:\n%s\n", buffer);

    if (strncmp(buffer, "GET", 3) == 0) {
        send_response(new_socket, http_response);
    } else {
        printf("Non-GET request received, not handling.\n");
    }

    close(new_socket);
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    server_fd = create_server_socket();

    while (1) {
        new_socket = accept_connection(server_fd, &address, &addrlen);

        handle_request(new_socket);
    }

    close(server_fd);

    return 0;
}
