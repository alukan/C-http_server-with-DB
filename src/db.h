#ifndef DB_H
#define DB_H
#include <mongoc/mongoc.h>

mongoc_client_t *init_mongo_connection();
void cleanup_mongo_connection(mongoc_client_t *client);

#endif
