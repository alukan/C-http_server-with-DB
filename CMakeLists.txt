cmake_minimum_required(VERSION 3.10)

project(ServerApp C)

# Set the C standard
set(CMAKE_C_STANDARD 99)

# Use FetchContent to download and include both dotenv-c and cJSON directly
include(FetchContent)

# Fetch cJSON from the official repository
FetchContent_Declare(
  cjson
  GIT_REPOSITORY https://github.com/DaveGamble/cJSON.git
  GIT_TAG        master
)
FetchContent_MakeAvailable(cjson)

# Fetch dotenv-c from GitHub
FetchContent_Declare(
  dotenv-c
  GIT_REPOSITORY https://github.com/Isty001/dotenv-c.git
  GIT_TAG        master
)
FetchContent_MakeAvailable(dotenv-c)

# Set the output directory for the binaries
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Define source files for the project
set(SOURCES
    src/main.c
    src/server.c
    src/count_handler.c
    src/request_handler.c
    src/config.c
    src/db.c
    src/request_utils.c
    src/file_request_handlers.c
)

# Add the executable target
add_executable(${PROJECT_NAME} ${SOURCES})

# Include dotenv-c headers and cJSON
target_include_directories(${PROJECT_NAME} PRIVATE
    ${dotenv-c_SOURCE_DIR}/src
    ${cjson_SOURCE_DIR}
)

# Add dotenv-c and cJSON source files to the project
target_sources(${PROJECT_NAME} PRIVATE
    ${dotenv-c_SOURCE_DIR}/src/dotenv.c
)

# macOS-specific configurations
if(APPLE)
    # Check if the /opt/homebrew directory exists (Apple Silicon M1/M2)
    if(EXISTS "/opt/homebrew")
        # This is an M1 or M2 (Apple Silicon) Mac
        set(ENV{PKG_CONFIG_PATH} "/opt/homebrew/opt/libmongoc/lib/pkgconfig:/opt/homebrew/opt/libbson/lib/pkgconfig:$ENV{PKG_CONFIG_PATH}")
    else()
        # This is an Intel Mac
        set(ENV{PKG_CONFIG_PATH} "/usr/local/opt/libmongoc/lib/pkgconfig:/usr/local/opt/libbson/lib/pkgconfig:$ENV{PKG_CONFIG_PATH}")
    endif()
endif()

# Find and link MongoDB libraries (libmongoc and libbson)
find_package(PkgConfig REQUIRED)
pkg_check_modules(MONGOC REQUIRED libmongoc-1.0)
pkg_check_modules(BSONC REQUIRED libbson-1.0)

include_directories(${MONGOC_INCLUDE_DIRS})
link_directories(${MONGOC_LIBRARY_DIRS})

# Link MongoDB, BSON, dotenv-c, and cJSON libraries
target_link_libraries(${PROJECT_NAME} ${MONGOC_LIBRARIES} ${BSONC_LIBRARIES} cjson)
