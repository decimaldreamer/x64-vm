add_library(handlers
    handler_unknown.cpp
    handler_add.cpp
    handler_add_byte.cpp
    handler_add_displaced_destination.cpp
    handler_add_displaced_source.cpp
)

target_include_directories(handlers
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
) 