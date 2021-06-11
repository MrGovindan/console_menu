cmake_minimum_required(VERSION 3.10)

project(libconsole_menu LANGUAGES CXX)

# Console Menu Library
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

# Console Menu Test
enable_testing()
add_subdirectory(test)

# Examples directory
add_subdirectory(examples)
