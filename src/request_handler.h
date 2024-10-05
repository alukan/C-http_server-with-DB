#ifndef REQUEST_HANDLER_H
#define REQUEST_HANDLER_H

#include "config.h"  // For BUFFER_SIZE and BODY_SIZE
#include "count_handler.h"  // For count handling functions

void handle_request(int new_socket);

#endif
