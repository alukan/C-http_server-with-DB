#include <mongoc/mongoc.h>
#include <stdio.h>
#include <stdlib.h>
#include "dotenv.h"

mongoc_client_t *init_mongo_connection() {
    const char *mongo_uri = getenv("MONGO_URI");
    if (mongo_uri == NULL) {
        fprintf(stderr, "MONGO_URI not set in .env file\n");
        return NULL;
    }

    mongoc_init();

    mongoc_client_t *client = mongoc_client_new(mongo_uri);
    if (!client) {
        fprintf(stderr, "Failed to create a MongoDB client for URI: %s\n", mongo_uri);
        return NULL;
    }

    printf("Connected to MongoDB successfully!\n");

    return client;
}


void cleanup_mongo_connection(mongoc_client_t *client) {
    if (client) {
        mongoc_client_destroy(client);
        printf("MongoDB connection closed.\n");
    }
    mongoc_cleanup();
}
