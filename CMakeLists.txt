cmake_minimum_required(VERSION 3.10)

project(ServerApp C)

# Set the C standard
set(CMAKE_C_STANDARD 99)

# Set the output directory for the binaries
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Define source files for the project
set(SOURCES
    src/main.c
    src/server.c
    src/count_handler.c
    src/request_handler.c
)

# Define the header files for the project (optional, CMake finds headers automatically)
set(HEADERS
    src/server.h
    src/count_handler.h
    src/request_handler.h
    src/config.h
)

# Add the executable target
add_executable(${PROJECT_NAME} ${SOURCES})
