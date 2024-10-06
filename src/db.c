#include <mongoc/mongoc.h>
#include <stdio.h>
#include <stdlib.h>
#include "dotenv.h"

// Global/static variables for MongoDB client and database name
static mongoc_client_t *client = NULL;
static const char *db_name = NULL;

void init_mongo_connection() {
    if (client == NULL) {
        const char *mongo_uri = getenv("MONGO_URI");
        if (mongo_uri == NULL) {
            fprintf(stderr, "MONGO_URI not set in .env file\n");
            return;
        }

        db_name = getenv("MONGO_DB_NAME");
        if (db_name == NULL) {
            fprintf(stderr, "MONGO_DB_NAME not set in .env file\n");
            return;
        }

        mongoc_init();
        client = mongoc_client_new(mongo_uri);
        if (!client) {
            fprintf(stderr, "Failed to create a MongoDB client for URI: %s\n", mongo_uri);
            return;
        }

        printf("Connected to MongoDB successfully, using database: %s\n", db_name);
    }
}

void cleanup_mongo_connection() {
    if (client) {
        mongoc_client_destroy(client);
        printf("MongoDB connection closed.\n");
        client = NULL;  // Set to NULL after cleanup
    }
    mongoc_cleanup();
}

int save_file(const char *name, const char *content) {
    if (client == NULL) {
        init_mongo_connection();
        if (client == NULL) {
            return 0;  // Failed to initialize connection
        }
    }

    mongoc_collection_t *collection;
    bson_t *doc;
    bson_error_t error;

    collection = mongoc_client_get_collection(client, db_name, "files");

    doc = bson_new();
    BSON_APPEND_UTF8(doc, "name", name);
    BSON_APPEND_UTF8(doc, "content", content);

    if (!mongoc_collection_insert_one(collection, doc, NULL, NULL, &error)) {
        fprintf(stderr, "Failed to insert file: %s\n", error.message);
        bson_destroy(doc);
        mongoc_collection_destroy(collection);
        return 0; // Failed to insert
    }

    printf("File inserted successfully!\n");

    bson_destroy(doc);
    mongoc_collection_destroy(collection);

    return 1; // Success
}

int update_file(const char *name, const char *new_content) {
    if (client == NULL) {
        init_mongo_connection();
        if (client == NULL) {
            return 0;  // Failed to initialize connection
        }
    }

    mongoc_collection_t *collection;
    bson_t *query;
    bson_t *update;
    bson_error_t error;

    collection = mongoc_client_get_collection(client, db_name, "files");

    query = bson_new();
    BSON_APPEND_UTF8(query, "name", name);
    update = BCON_NEW("$set", "{", "content", BCON_UTF8(new_content), "}");

    if (!mongoc_collection_update_one(collection, query, update, NULL, NULL, &error)) {
        fprintf(stderr, "Failed to update file: %s\n", error.message);
        bson_destroy(query);
        bson_destroy(update);
        mongoc_collection_destroy(collection);
        return 0; // Failed to update
    }

    printf("File updated successfully!\n");

    bson_destroy(query);
    bson_destroy(update);
    mongoc_collection_destroy(collection);

    return 1; // Success
}

int delete_file(const char *name) {
    if (client == NULL) {
        init_mongo_connection();
        if (client == NULL) {
            return 0;  // Failed to initialize connection
        }
    }

    mongoc_collection_t *collection;
    bson_t *query;
    bson_t reply;
    bson_error_t error;

    collection = mongoc_client_get_collection(client, db_name, "files");

    query = bson_new();
    BSON_APPEND_UTF8(query, "name", name);

    if (!mongoc_collection_delete_one(collection, query, NULL, &reply, &error)) {
        fprintf(stderr, "Failed to delete file: %s\n", error.message);
        bson_destroy(query);
        mongoc_collection_destroy(collection);
        return 0; // Failed to delete
    }

    // Check how many documents were deleted
    bson_iter_t iter;
    if (bson_iter_init_find(&iter, &reply, "deletedCount") && BSON_ITER_HOLDS_INT32(&iter)) {
        int32_t deleted_count = bson_iter_int32(&iter);
        if (deleted_count == 0) {
            printf("No file found with the name: %s\n", name);
            bson_destroy(query);
            bson_destroy(&reply);
            mongoc_collection_destroy(collection);
            return -1;
        }
    }

    printf("File deleted successfully!\n");

    bson_destroy(query);
    mongoc_collection_destroy(collection);

    return 1; // Success
}