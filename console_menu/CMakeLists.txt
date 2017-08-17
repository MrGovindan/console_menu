project(libconsole_menu VERSION 0.1 LANGUAGES CXX)

# - Console Menu Library ---------------------------------------------------------------------------

add_library(console_menu
  src/console_menu.cpp
  src/menu_item.cpp
  src/submenu_item.cpp
  )

target_include_directories(console_menu PUBLIC include)

target_compile_options(console_menu PRIVATE "-std=c++14")

# - Console Menu Test ------------------------------------------------------------------------------

add_executable(test_console_menu
  test/test_main.cpp
  test/test_console_menu.cpp
  )

target_link_libraries (test_console_menu
  console_menu
  gtest
  )
