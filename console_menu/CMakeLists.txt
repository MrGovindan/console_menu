# Define library. Only source files here!
project(libconsole_menu VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# - Console Menu Library ---------------------------------------------------------------------------
add_library(console_menu
  src/console_menu.cpp
  src/menu_item.cpp
  src/submenu_item.cpp
  )

target_include_directories(console_menu PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
  PRIVATE src)

# - Console Menu Test ------------------------------------------------------------------------------
add_executable(test_console_menu
  test/test_main.cpp
  test/test_console_menu.cpp
  )

target_link_libraries (test_console_menu
  console_menu
  gtest
  )

add_test(test_console_menu test_console_menu)
