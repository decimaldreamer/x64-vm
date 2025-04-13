add_library(memory
    memory_pool.cpp
    memory_protection.cpp
    virtual_section.cpp
    virtual_stack.cpp
)

target_include_directories(memory
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
) 