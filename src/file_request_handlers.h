#ifndef FILES_REQUEST_HANDLER_H
#define FILES_REQUEST_HANDLER_H

#include "db.h"
#include "request_utils.h"

void file_save_handler(int new_socket, char *non_processed_buffer);
void file_update_handler(int new_socket, char *non_processed_buffer);
void file_delete_handler(int new_socket, const char *name);
void file_get_handler(int new_socket, const char *name);

#endif
