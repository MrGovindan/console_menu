add_executable(console_menu_test
	TestMain.cpp
	TestConsoleMenu.cpp
)

target_link_libraries(console_menu_test PRIVATE console_menu)

add_test(unit_test console_menu_test)
