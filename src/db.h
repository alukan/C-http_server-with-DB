#ifndef DB_H
#define DB_H
#include <mongoc.h>

mongoc_client_t *init_mongo_connection();
void cleanup_mongo_connection(mongoc_client_t *client);

int save_file(const char *name, const char *content);
int update_file(const char *name, const char *new_content);
int delete_file(const char *name);

#endif
