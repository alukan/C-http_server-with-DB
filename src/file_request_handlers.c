#include "file_request_handlers.h"
#include <cjson/cJSON.h>


void file_save_handler(int new_socket, char *non_processed_buffer) {
    char *body;
    if (extract_body_from_request(non_processed_buffer, &body) == 0 && body != NULL) {
        cJSON *json = cJSON_Parse(body);
        if (json == NULL) {
            send_response(new_socket, "HTTP/1.1 400 Bad Request\nContent-Type: text/plain\nContent-Length: 16\n\nInvalid JSON format.");
            return;
        }

        cJSON *name_json = cJSON_GetObjectItemCaseSensitive(json, "name");
        cJSON *content_json = cJSON_GetObjectItemCaseSensitive(json, "content");

        if (!cJSON_IsString(name_json) || !cJSON_IsString(content_json)) {
            send_response(new_socket, "HTTP/1.1 400 Bad Request\nContent-Type: text/plain\nContent-Length: 16\n\nInvalid data format.");
            cJSON_Delete(json);
            return;
        }

        if (save_file(name_json->valuestring, content_json->valuestring)) {
            send_response(new_socket, "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 19\n\nFile saved successfully.");
        } else {
            send_response(new_socket, "HTTP/1.1 500 Internal Server Error\nContent-Type: text/plain\nContent-Length: 21\n\nFailed to save file.");
        }

        cJSON_Delete(json);
    } else {
        send_response(new_socket, "HTTP/1.1 400 Bad Request\nContent-Type: text/plain\nContent-Length: 16\n\nInvalid body.");
    }
}

void file_update_handler(int new_socket, char *non_processed_buffer) {
    char *body;
    if (extract_body_from_request(non_processed_buffer, &body) == 0 && body != NULL) {
        cJSON *json = cJSON_Parse(body);
        if (json == NULL) {
            send_response(new_socket, "HTTP/1.1 400 Bad Request\nContent-Type: text/plain\nContent-Length: 16\n\nInvalid JSON format.");
            return;
        }

        cJSON *name_json = cJSON_GetObjectItemCaseSensitive(json, "name");
        cJSON *content_json = cJSON_GetObjectItemCaseSensitive(json, "content");

        if (!cJSON_IsString(name_json) || !cJSON_IsString(content_json)) {
            send_response(new_socket, "HTTP/1.1 400 Bad Request\nContent-Type: text/plain\nContent-Length: 16\n\nInvalid data format.");
            cJSON_Delete(json);
            return;
        }

        if (update_file(name_json->valuestring, content_json->valuestring)) {
            send_response(new_socket, "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 22\n\nFile updated successfully.");
        } else {
            send_response(new_socket, "HTTP/1.1 500 Internal Server Error\nContent-Type: text/plain\nContent-Length: 23\n\nFailed to update file.");
        }

        cJSON_Delete(json);
    } else {
        send_response(new_socket, "HTTP/1.1 400 Bad Request\nContent-Type: text/plain\nContent-Length: 16\n\nInvalid body.");
    }
}

void file_delete_handler(int new_socket, const char *name) {
    if (delete_file(name) == 1) {
        send_response(new_socket, "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 23\n\nFile deleted successfully.");
    } else {
        printf("Failed to delete file: %s\n", name);
        send_response(new_socket, "HTTP/1.1 500 Internal Server Error\nContent-Type: text/plain\nContent-Length: 22\n\nFailed to delete file.");
    }
}

void file_get_handler(int new_socket, const char *name) {
    char *file_content = get_file(name);
    char response[1024];
    snprintf(response, sizeof(response),
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: application/json\r\n"
        "Content-Length: %lu\r\n"
        "\r\n"
        "%s",
        strlen(file_content), file_content);

    if (file_content != NULL) {
        send_response(new_socket, response);
    } else {
        send_response(new_socket, "HTTP/1.1 404 Not Found\nContent-Type: text/plain\nContent-Length: 19\n\nFile not found.");
    }
}
