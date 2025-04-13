cmake_minimum_required(VERSION 3.15)
project(x64vm VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Find required packages
find_package(Threads REQUIRED)
find_package(fmt REQUIRED)

# Add subdirectories
add_subdirectory(src/core)
add_subdirectory(src/handlers)
add_subdirectory(src/debug)
add_subdirectory(src/security)
add_subdirectory(src/network)
add_subdirectory(src/utils)

# Main executable
add_executable(x64vm
    src/entry.cpp
)

target_link_libraries(x64vm
    PRIVATE
    core
    handlers
    debug
    security
    network
    utils
    fmt::fmt
    Threads::Threads
)

# Install rules
install(TARGETS x64vm
    RUNTIME DESTINATION bin
)

# Testing
enable_testing()
add_subdirectory(tests) 