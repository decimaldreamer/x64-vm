add_library(core STATIC
    instruction.cpp
    instruction_cache.cpp
    instruction_opcode.cpp
    instruction_operand.cpp
    instruction_prefix.cpp
    memory_pool.cpp
    memory_protection.cpp
    thread_pool.cpp
)

target_include_directories(core
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(core
    PRIVATE
    fmt::fmt
    Threads::Threads
) 