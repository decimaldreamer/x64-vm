add_library(handlers STATIC
    handler_add.cpp
    handler_add_byte.cpp
    handler_add_displaced_destination.cpp
    handler_add_displaced_source.cpp
    handler_unknown.cpp
)

target_include_directories(handlers
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(handlers
    PRIVATE
    core
) 