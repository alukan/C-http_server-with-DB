#ifndef REQUEST_HANDLER_H
#define REQUEST_HANDLER_H

#include "config.h"
#include "count_handler.h"
#include "db.h"
#include "request_utils.h"
#include "file_request_handlers.h"

void handle_request(int new_socket);

#endif
