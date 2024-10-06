cmake_minimum_required(VERSION 3.10)

project(ServerApp C)

# Set the C standard
set(CMAKE_C_STANDARD 99)

# Use FetchContent to download and include cJSON directly
include(FetchContent)

# Fetch cJSON from the official repository
FetchContent_Declare(
  cjson
  GIT_REPOSITORY https://github.com/DaveGamble/cJSON.git
  GIT_TAG        master
)

# Make cJSON available to the project
FetchContent_MakeAvailable(cjson)

# External project to download dotenv-c from GitHub
include(ExternalProject)
ExternalProject_Add(dotenv-c
    GIT_REPOSITORY https://github.com/Isty001/dotenv-c.git
    GIT_TAG master
    PREFIX ${CMAKE_BINARY_DIR}/dotenv-c
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
    LOG_DOWNLOAD ON
)

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

# Include dotenv-c headers
ExternalProject_Get_Property(dotenv-c source_dir)
include_directories(${source_dir})

# Add dotenv.c to the project
add_dependencies(${PROJECT_NAME} dotenv-c)
target_sources(${PROJECT_NAME} PRIVATE ${source_dir}/src/dotenv.c)

# Find and link MongoDB libraries (libmongoc and libbson)
find_package(PkgConfig REQUIRED)
pkg_check_modules(MONGOC REQUIRED libmongoc-1.0)
pkg_check_modules(BSONC REQUIRED libbson-1.0)

include_directories(${MONGOC_INCLUDE_DIRS})
link_directories(${MONGOC_LIBRARY_DIRS})

# Link MongoDB and BSON libraries
target_link_libraries(${PROJECT_NAME} ${MONGOC_LIBRARIES} ${BSONC_LIBRARIES})

# Link cJSON to the project
target_link_libraries(${PROJECT_NAME} cjson)
