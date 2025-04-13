add_library(debug STATIC
    debugger.cpp
    disasm.cpp
    loggr.cpp
)

target_include_directories(debug
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(debug
    PRIVATE
    core
    fmt::fmt
) 