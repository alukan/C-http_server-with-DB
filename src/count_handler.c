#include "count_handler.h"
#include "server.h"
#include "config.h"
#include <stdio.h>
#include <sys/time.h>
#include <string.h>

void handle_count_addition(int new_socket, int num) {
    struct timeval start, end;
    long time_taken;
    int body_size = get_body_size();
    gettimeofday(&start, NULL);
    int result = 0;
    for (int i = 0; i < num; i++) {
        result = i % num + i % 41258;
    }
    gettimeofday(&end, NULL);

    time_taken = ((end.tv_sec - start.tv_sec) * 1000000L + end.tv_usec) - start.tv_usec;

    char response_body[body_size];
    snprintf(response_body, sizeof(response_body),
             "<html><body>Result: %d<br>Time taken: %ld microseconds</body></html>", result, time_taken);

    char response_header[body_size];
    snprintf(response_header, sizeof(response_header),
             "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: %lu\n\n%s",
             strlen(response_body), response_body);

    send_response(new_socket, response_header);
}

void handle_count_multiplication(int new_socket, int num) {
    struct timeval start, end;
    long time_taken;

    gettimeofday(&start, NULL);
    int result = 1;
    for (int i = 0; i < num; i++) {
        result = i % num * i % 41258;
    }
    gettimeofday(&end, NULL);

    time_taken = ((end.tv_sec - start.tv_sec) * 1000000L + end.tv_usec) - start.tv_usec;

    char response_body[get_body_size()];
    snprintf(response_body, sizeof(response_body),
             "<html><body>Result: %d<br>Time taken: %ld microseconds</body></html>", result, time_taken);

    char response_header[get_buffer_size()];
    snprintf(response_header, sizeof(response_header),
             "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: %lu\n\n%s",
             strlen(response_body), response_body);

    send_response(new_socket, response_header);
}
