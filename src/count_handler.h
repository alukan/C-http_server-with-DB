#ifndef COUNT_HANDLER_H
#define COUNT_HANDLER_H

#include "config.h"   // Include configuration constants
#include "server.h"   // Include server functionalities

void handle_count_addition(int new_socket, int num);

void handle_count_multiplication(int new_socket, int num);

#endif // COUNT_HANDLER_H
