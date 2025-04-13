cmake_minimum_required(VERSION 3.10)
project(x64_vm)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required packages
find_package(OpenSSL REQUIRED)
find_package(OpenGL REQUIRED)
find_package(DirectX REQUIRED)
find_package(TensorFlow REQUIRED)
find_package(Boost REQUIRED COMPONENTS system serialization)
find_package(LLVM REQUIRED CONFIG)

# Add source directories
add_subdirectory(core)
add_subdirectory(instructions)
add_subdirectory(handlers)
add_subdirectory(utils)
add_subdirectory(debug)
add_subdirectory(config)
add_subdirectory(memory)
add_subdirectory(tests)
add_subdirectory(jit)
add_subdirectory(security)
add_subdirectory(graphics)
add_subdirectory(distributed)
add_subdirectory(ai)
add_subdirectory(optimization)

# Add main executable
add_executable(x64_vm entry.cpp)

# Link libraries
target_link_libraries(x64_vm
    PRIVATE
    core
    instructions
    handlers
    utils
    debug
    config
    memory
    jit
    security
    graphics
    distributed
    ai
    optimization
    ${OPENSSL_LIBRARIES}
    ${OPENGL_LIBRARIES}
    ${DIRECTX_LIBRARIES}
    ${TENSORFLOW_LIBRARIES}
    ${Boost_LIBRARIES}
    ${LLVM_LIBS}
)

# Include directories
target_include_directories(x64_vm
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${OPENSSL_INCLUDE_DIR}
    ${OPENGL_INCLUDE_DIR}
    ${DIRECTX_INCLUDE_DIR}
    ${TENSORFLOW_INCLUDE_DIR}
    ${Boost_INCLUDE_DIRS}
    ${LLVM_INCLUDE_DIRS}
) 