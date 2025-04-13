add_library(utils
    thread_pool.cpp
    spinner.cpp
    numerical_helper.cpp
    loggr.cpp
    global.cpp
    compiler_helper.cpp
)

target_include_directories(utils
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
) 