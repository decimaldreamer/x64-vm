add_library(tests
    unit_tests.cpp
    vm_test.cpp
)

target_include_directories(tests
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
) 