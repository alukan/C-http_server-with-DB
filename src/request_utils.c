#include "request_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int extract_body_from_request(char *non_processed_buffer, char **body) {
    *body = NULL;
    // Find the end of the headers
    char *headers_end = strstr(non_processed_buffer, "\r\n\r\n");
    if (headers_end == NULL) {
        printf("Error: Could not find the end of headers.\n");
        return -1; // Error: End of headers not found
    }

    // The body starts after the "\r\n\r\n"
    *body = headers_end + 4;

    // Return success if body and content length are found
    if (*body != NULL) {
        return 0; // Success
    }
    else{
        return -1; // Error: Body not found
    }
}

// Function to send an HTTP response
void send_response(int socket, const char *response) {
    write(socket, response, strlen(response));
    printf("Response sent\n");
}

char* extract_param_from_uri(const char *uri, const char *uri_before, const char *uri_after) {
    size_t param_length;
    char *start = strstr(uri, uri_before);
    if (start == NULL)
        return NULL;

    start += strlen(uri_before);

    if (uri_after != NULL) {
        char *end = strstr(start, uri_after);
        if (end == NULL)
            return NULL;

        param_length = end - start;
    }
    else {
        param_length = strlen(start);
    }
    // Dynamically allocated, because local arrays are destroyed when function returns
    char *res_str = (char *)malloc(param_length + 1); // +1 for the null terminator
    strncpy(res_str, start, param_length);
    res_str[param_length] = '\0'; // Null-terminate the string
    return res_str;
}

