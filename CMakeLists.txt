cmake_minimum_required(VERSION 3.10)

project(ServerApp C)

# Set the C standard
set(CMAKE_C_STANDARD 99)

include(ExternalProject)

# External project to download dotenv-c from GitHub
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
)

# Add the executable target
add_executable(${PROJECT_NAME} ${SOURCES})

# Include dotenv-c headers
ExternalProject_Get_Property(dotenv-c source_dir)
include_directories(${source_dir})

# Add dotenv.c to the project
add_dependencies(${PROJECT_NAME} dotenv-c)
target_sources(${PROJECT_NAME} PRIVATE ${source_dir}/src/dotenv.c)

find_package(PkgConfig REQUIRED)
pkg_check_modules(MONGOC REQUIRED libmongoc-1.0)
pkg_check_modules(BSONC REQUIRED libbson-1.0)

include_directories(${MONGOC_INCLUDE_DIRS})
link_directories(${MONGOC_LIBRARY_DIRS})

target_link_libraries(${PROJECT_NAME} ${MONGOC_LIBRARIES} ${BSONC_LIBRARIES})
