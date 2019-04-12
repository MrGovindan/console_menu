cmake_minimum_required(VERSION 3.10)

project(libconsole_menu LANGUAGES CXX)

add_subdirectory(libs/googletest)

enable_testing()

# Console Menu Library -----------------------------------------------------------------------------
add_library(console_menu
  src/ConsoleMenu.cpp
  src/MenuItem.cpp
  src/SubmenuItem.cpp
  )

target_include_directories(console_menu
  PUBLIC
    include
  )

target_compile_features(console_menu
  PRIVATE
    cxx_std_14
  )

target_compile_options(console_menu
  PRIVATE
    -Wall
    -Werror
  )

# Console Menu Test --------------------------------------------------------------------------------
add_executable(test_console_menu
  test/TestMain.cpp
  test/TestConsoleMenu.cpp
  )

target_link_libraries (test_console_menu
  PRIVATE
    console_menu
    gtest
  )

add_test(unit_test test_console_menu)

add_subdirectory(examples)
