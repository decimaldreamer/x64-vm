add_library(debug
    debugger.cpp
    disasm.cpp
)

target_include_directories(debug
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
) 