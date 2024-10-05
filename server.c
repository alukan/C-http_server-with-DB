#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/time.h>

#define PORT 8080
#define BUFFER_SIZE 30000
#define BODY_SIZE 20000 // Adjusted size for the response body

// Function to create and bind the server socket
int create_server_socket() {
    int server_fd;
    struct sockaddr_in address;
    int opt = 1;

    // Create socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Set socket options to allow address reuse (optional, but useful)
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
    {
        perror("setsockopt failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Define the address for the socket
    address.sin_family = AF_INET;         // IPv4
    address.sin_addr.s_addr = INADDR_ANY; // Any incoming interface
    address.sin_port = htons(PORT);       // Port to bind the server

    // Bind the socket to the specified port and address
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Start listening on the socket for incoming connections
    if (listen(server_fd, 3) < 0)
    {
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
    if ((new_socket = accept(server_fd, (struct sockaddr *)address, (socklen_t *)addrlen)) < 0)
    {
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

void print_buffer_debug(const char *buffer, int length) {
    printf("Buffer content (with control characters):\n");
    for (int i = 0; i < length; i++)
    {
        // Check for control characters and print them in a readable format
        if (buffer[i] == '\r')
        {
            printf("\\r"); // Print \r as \\r
        }
        else if (buffer[i] == '\n')
        {
            printf("\\n"); // Print \n as \\n
        }
        else if (buffer[i] == '\t')
        {
            printf("\\t"); // Print tab as \\t
        }
        else if (buffer[i] >= 32 && buffer[i] <= 126)
        {
            printf("%c", buffer[i]); // Print printable ASCII characters as-is
        }
        else
        {
            printf("\\x%02x", (unsigned char)buffer[i]); // Print other non-printable characters as hex
        }
    }
    printf("\n");
}

int extract_num_from_uri(const char *uri, const char *operation) {
    char *start = strstr(uri, "/count/");
    if (start == NULL)
        return -1;
    start += strlen("/count/");

    char *end = strstr(start, operation);
    if (end == NULL)
        return -1;

    char num_str[20] = {0};
    strncpy(num_str, start, end - start);
    return atoi(num_str);
}

void handle_request(int new_socket) {
    char buffer[BUFFER_SIZE] = {0};
    char *request_line, *method, *uri;
    int content_length = 0;
    char *body = NULL;

    read(new_socket, buffer, BUFFER_SIZE);
    // Get the first line of the HTTP request (request line)
    request_line = strtok(buffer, "\r\n");
    // Save where non-changed (after null terminator created by strtok) part of buffer starts
    char *non_processed_buffer = buffer + strlen(request_line) + 1; // +1 for the null terminator

    // Parse the method (GET) and URI (e.g., "/")
    method = strtok(request_line, " ");
    uri = strtok(NULL, " ");
    if (method == NULL || uri == NULL) {
        printf("Error: Could not parse method or URI from request line.\n");
        close(new_socket);
        return;
    }
    printf("Method: %s\n", method);
    printf("URI: %s\n", uri);

    if (strcmp(method, "GET") == 0) {
        if (strcmp(uri, "/") == 0) {
            send_response(new_socket, "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: 39\n\n<html><body>Welcome to Home Page</body></html>");
        }
        else if (strcmp(uri, "/about") == 0) {
            send_response(new_socket, "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: 44\n\n<html><body>About Us Page: We do cool stuff!</body></html>");
        }
        else if (strstr(uri, "/count/") != NULL) {
            int num;
            struct timeval start, end;
            long time_taken;

            // Check for /count/{num}/addition
            if (strstr(uri, "/addition") != NULL) {
                num = extract_num_from_uri(uri, "/addition");
                if (num == -1) {
                    send_response(new_socket, "HTTP/1.1 400 Bad Request\nContent-Type: text/plain\nContent-Length: 16\n\nInvalid number.");
                    close(new_socket);
                    return;
                }

                gettimeofday(&start, NULL);
                int result = 0;
                for (int i = 0; i < num; i++) {
                    result = i + 1;
                }
                gettimeofday(&end, NULL);

                time_taken = ((end.tv_sec - start.tv_sec) * 1000000L + end.tv_usec) - start.tv_usec;

                char response_body[BODY_SIZE];
                snprintf(response_body, sizeof(response_body),
                         "<html><body>Result: %d<br>Time taken: %ld microseconds</body></html>", result, time_taken);

                char response_header[BUFFER_SIZE];
                snprintf(response_header, sizeof(response_header),
                         "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: %lu\n\n%s",
                         strlen(response_body), response_body);

                send_response(new_socket, response_header);

                // Check for /count/{num}/multiplication
            }
            else if (strstr(uri, "/multiplication") != NULL) {
                num = extract_num_from_uri(uri, "/multiplication");
                if (num == -1) {
                    send_response(new_socket, "HTTP/1.1 400 Bad Request\nContent-Type: text/plain\nContent-Length: 16\n\nInvalid number.");
                    close(new_socket);
                    return;
                }

                gettimeofday(&start, NULL);
                int result = 1;
                for (int i = 0; i < num; i++) {
                    result = i * 2;
                }
                gettimeofday(&end, NULL);

                time_taken = ((end.tv_sec - start.tv_sec) * 1000000L + end.tv_usec) - start.tv_usec;

                char response_body[BODY_SIZE];
                snprintf(response_body, sizeof(response_body),
                         "<html><body>Result: %d<br>Time taken: %ld microseconds</body></html>", result, time_taken);

                char response_header[BUFFER_SIZE];
                snprintf(response_header, sizeof(response_header),
                         "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: %lu\n\n%s",
                         strlen(response_body), response_body);

                send_response(new_socket, response_header);
            }
            else{
                send_response(new_socket, "HTTP/1.1 404 Not Found\nContent-Type: text/plain\nContent-Length: 16\n\nEndpoint not found.");
            }
        }
        else{
            send_response(new_socket, "HTTP/1.1 404 Not Found\nContent-Type: text/html\nContent-Length: 45\n\n<html><body>404 - Page Not Found!</body></html>");
        }
    }
    else if (strcmp(method, "POST") == 0) {
        char *content_length_header = strstr(non_processed_buffer, "Content-Length:");
        if (content_length_header != NULL) {
            content_length = atoi(content_length_header + 15); // 15 is the length of "Content-Length:"
            printf("Content-Length: %d\n", content_length);
        }

        // we know that header end is after content_length_header
        char *headers_end = strstr(content_length_header, "\r\n\r\n");
        if (headers_end == NULL) {
            printf("Error: Could not find the end of headers.\n");
            close(new_socket);
            return;
        }
        // Move past "\r\n\r\n" to the body
        body = headers_end + 4;

        if (body != NULL && content_length > 0) {
            printf("POST body: %s\n", body);

            // Respond with the received body
            char response_body[BODY_SIZE];
            snprintf(response_body, sizeof(response_body),
                     "<html><body>POST data received: %s</body></html>", body);

            // Prepare and send the HTTP response
            char response_header[BUFFER_SIZE];
            snprintf(response_header, sizeof(response_header),
                     "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: %lu\n\n%s",
                     strlen(response_body), response_body);

            send_response(new_socket, response_header);
        }
        else{
            char error_response[BUFFER_SIZE];
            snprintf(error_response, sizeof(error_response),
                     "HTTP/1.1 500 Internal Server Error\nContent-Type: text/html\nContent-Length: %lu\n\n<html><body>500 Internal Server Error: Body not found.</body></html>",
                     strlen("<html><body>500 Internal Server Error: Body not found.</body></html>"));
            send_response(new_socket, error_response);
        }
    }
    else{
        // If it's not a GET or POST request, ignore it for now
        printf("Unsupported method received: %s\n", method);
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
