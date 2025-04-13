add_library(instructions
    instruction.cpp
    instruction_opcode.cpp
    instruction_operand.cpp
    instruction_prefix.cpp
    instruction_cache.cpp
    floating_point.cpp
)

target_include_directories(instructions
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
) 