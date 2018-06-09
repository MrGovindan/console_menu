set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(console_menu_demo
  src/Main.cpp
  )

target_link_libraries(console_menu_demo
  console_menu)
