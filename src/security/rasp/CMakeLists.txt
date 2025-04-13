add_library(rasp_manager STATIC
    rasp_manager.cpp
)

add_executable(rasp_example
    rasp_example.cpp
)

target_link_libraries(rasp_example
    PRIVATE
    rasp_manager
    core
    security_common
) 