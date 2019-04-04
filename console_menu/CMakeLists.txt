project(libconsole_menu VERSION 0.1 LANGUAGES CXX)

# - Console Menu Library ---------------------------------------------------------------------------

add_library(console_menu
  src/ConsoleMenu.cpp
  src/MenuItem.cpp
  src/SubmenuItem.cpp
  )

target_include_directories(console_menu PUBLIC include)

target_compile_options(console_menu PUBLIC "-std=c++14")

# - Console Menu Test ------------------------------------------------------------------------------

add_executable(test_console_menu
  test/TestMain.cpp
  test/TestConsoleMenu.cpp
  )

target_link_libraries (test_console_menu
  console_menu
  gtest
  )

add_test(unit_test test_console_menu)
