cmake_minimum_required(VERSION 3.3)

project(console_menu VERSION 0.1 LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
IF( EXISTS "${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json" )
  EXECUTE_PROCESS( COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json
    ${CMAKE_CURRENT_SOURCE_DIR}/compile_commands.json
  )
ENDIF()

enable_testing()
add_subdirectory(console_menu)
add_subdirectory(console_menu_demo)
add_subdirectory(libs/googletest)


# # SET(GCC_COVERAGE_COMPILE_FLAGS "-fprofile-arcs -ftest-coverage")
# set(CMAKE_CXX_FLAGS "-std=c++14 -fprofile-arcs -ftest-coverage")
# SET(GCC_COVERAGE_LINK_FLAGS    "-lgcov")


# add_subdirectory(console_menu)
# add_subdirectory(test)

# add_executable(console_menu_demo main.cpp)
# target_link_libraries(console_menu_demo console_menu)
