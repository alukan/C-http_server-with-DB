#include <stdio.h>
#include <stdlib.h>
#include "config.h"
#include "dotenv.h" 

static int buffer_size = 30000;  // Default values
static int body_size = 20000;

int get_buffer_size() {
    return buffer_size;
}

int get_body_size() {
    return body_size;
}

// Helper function to load an integer from the environment
static int get_env_int(const char *varname, int default_value) {
    const char *value = getenv(varname);
    if (value) {
        return atoi(value);
    }
    return default_value;
}

// Load configuration from.env file
void load_config() {
    env_load(".env", false);  

    // Load configuration values
    buffer_size = get_env_int("BUFFER_SIZE", buffer_size);  // Load or use default
    body_size = get_env_int("BODY_SIZE", body_size);
    printf("Loaded config: BUFFER_SIZE=%d, BODY_SIZE=%d\n", buffer_size, body_size);
}
