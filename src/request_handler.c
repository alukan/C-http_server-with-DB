#include "request_handler.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <cjson/cJSON.h>


void root_post_handler(int new_socket, char *non_processed_buffer) {
    char *body;
    if(extract_body_from_request(non_processed_buffer, &body) == 0 && body!= NULL){
        printf("POST body: %s\n", body);

        // Respond with the received body
        char response_body[get_body_size()];
        snprintf(response_body, sizeof(response_body),
                 "POST data received: %s", body);

        char response_header[get_buffer_size()];
        snprintf(response_header, sizeof(response_header),
                 "HTTP/1.1 200 OK\nContent-Type: text\nContent-Length: %lu\n\n%s",
                 strlen(response_body), response_body);

        send_response(new_socket, response_header);
    }
    else {
        char error_response[get_buffer_size()];
        snprintf(error_response, sizeof(error_response),
                 "HTTP/1.1 500 Internal Server Error\nContent-Type: text/html\nContent-Length: %lu\n\n<html><body>500 Internal Server Error: Body not found.</body></html>",
                 strlen("<html><body>500 Internal Server Error: Body not found.</body></html>"));
        send_response(new_socket, error_response);
    }
}


// Function to handle incoming requests
void handle_request(int new_socket) {
    char buffer[get_buffer_size()];
    char *request_line, *method, *uri;

    // Read the incoming request
    read(new_socket, buffer, get_buffer_size());
    // Get the first line of the HTTP request (request line)
    request_line = strtok(buffer, "\r\n");
    // Save where non-changed (after null terminator created by strtok) part of buffer starts
    char *non_processed_buffer = buffer + strlen(request_line) + 1; // +1 for the null terminator

    // Parse the method (GET or POST) and URI
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
        else if(strstr(uri, "/file/") != NULL) {
            char *file_name = extract_param_from_uri(uri, "/file/", NULL);
            if (file_name == NULL) {
                    send_response(new_socket, "HTTP/1.1 400 Bad Request\nContent-Type: text/plain\nContent-Length: 18\n\nInvalid file name.");
                    close(new_socket);
                    return;
                }
            file_get_handler(new_socket, file_name);
        }
        else if (strstr(uri, "/count/") != NULL) {
            char *num_str;
            if (strstr(uri, "/addition") != NULL) {
                num_str = extract_param_from_uri(uri, "/count/", "/addition");
                if (num_str == NULL) {
                    send_response(new_socket, "HTTP/1.1 400 Bad Request\nContent-Type: text/plain\nContent-Length: 16\n\nInvalid number.");
                    close(new_socket);
                    return;
                }
                handle_count_addition(new_socket, atoi(num_str));
            }
            else if (strstr(uri, "/multiplication") != NULL) {
                num_str = extract_param_from_uri(uri, "/count/", "/multiplication");
                if (num_str == NULL) {
                    send_response(new_socket, "HTTP/1.1 400 Bad Request\nContent-Type: text/plain\nContent-Length: 16\n\nInvalid number.");
                    close(new_socket);
                    return;
                }
                handle_count_multiplication(new_socket, atoi(num_str));
            }
            else {
                send_response(new_socket, "HTTP/1.1 404 Not Found\nContent-Type: text/plain\nContent-Length: 16\n\nEndpoint not found.");
            }
        }
        else
            send_response(new_socket, "HTTP/1.1 404 Not Found\nContent-Type: text/html\nContent-Length: 45\n\n<html><body>404 - Page Not Found!</body></html>");
    }
    else if (strcmp(method, "POST") == 0) {
        if (strcmp(uri, "/") == 0) {
            root_post_handler(new_socket, non_processed_buffer);
        }
        else if(strcmp(uri, "/file") == 0) {
            file_save_handler(new_socket, non_processed_buffer);
        }
        else {
            send_response(new_socket, "HTTP/1.1 404 Not Found\nContent-Type: text/plain\nContent-Length: 16\n\nEndpoint not found.");
        }
    }
    else if(strcmp(method, "PUT") == 0) {
        if(strcmp(uri, "/file") == 0) {
            file_update_handler(new_socket, non_processed_buffer);
        }
    }
    else if(strcmp(method, "DELETE") == 0) {
        if(strstr(uri, "/file/") != NULL) {
            char *file_name = extract_param_from_uri(uri, "/file/", NULL);
            if (file_name == NULL) {
                    send_response(new_socket, "HTTP/1.1 400 Bad Request\nContent-Type: text/plain\nContent-Length: 18\n\nInvalid file name.");
                    close(new_socket);
                    return;
                }
            printf("Deleting file: %s\n", file_name);
            file_delete_handler(new_socket, file_name);
        }
    }
    else {
        printf("Unsupported method received: %s\n", method);
    }

    close(new_socket);
}
