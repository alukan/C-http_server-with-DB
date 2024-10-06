#ifndef REQUEST_UTILS_H
#define REQUEST_UTILS_H

int extract_body_from_request(char *non_processed_buffer, char **body);
void send_response(int socket, const char *response);
char* extract_param_from_uri(const char *uri, const char *uri_before, const char *uri_after);

#endif
