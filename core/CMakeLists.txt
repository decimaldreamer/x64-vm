add_library(core
    virtual_machine.cpp
    virtual_memory.cpp
    register.cpp
    nested_virtualization.cpp
    plugin_system.cpp
    exceptions.cpp
)

target_include_directories(core
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
) 