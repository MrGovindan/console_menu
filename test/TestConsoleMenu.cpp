#include <sstream>

#include "catch.hpp">

#include <console_menu/ConsoleMenu.h>

using std::ostringstream;
using std::string;

TEST_CASE("ConsoleMenu with Display Menu Item")
{
	ostringstream outputStream;
	ConsoleMenu menu("Test Menu", outputStream, true);

	SECTION("ShouldDisplayMenuWithDefaultMenuItem")
	{
		menu.display();

		string expectedOutput("Test Menu\n"
			"=========\n"
			"m. Display menu.\n"
			"---------\n");

		REQUIRE(expectedOutput == outputStream.str());
	}

	SECTION("ShouldDisplayMenuWithChangedTitle")
	{
		menu.setTitle("Another Title");

		menu.display();

		string expectedOutput("Another Title\n"
			"=============\n"
			"m. Display menu.\n"
			"-------------\n");

		REQUIRE(expectedOutput == outputStream.str());
	}

	SECTION("ShouldHandleKeyForDefaultMenuItem")
	{
		menu.handleKey('m');

		string expectedOutput("Test Menu\n"
			"=========\n"
			"m. Display menu.\n"
			"---------\n");

		REQUIRE(expectedOutput == outputStream.str());
	}

	SECTION("ShouldDisplayMenuWithMultipleMenuItemsAdded")
	{
		menu.addMenuItem('c', "Test function", [&]() {});

		menu.display();

		string expectedOutput("Test Menu\n"
			"=========\n"
			"c. Test function.\n"
			"m. Display menu.\n"
			"---------\n");
		REQUIRE(expectedOutput == outputStream.str());
	}
}

TEST_CASE("ConsoleMenu without Display Menu Item")
{
	ostringstream outputStream;
	ConsoleMenu menu("Test Menu", outputStream, false);

	SECTION("ShouldDisplayMenuWithNoMenuItems")
	{
		menu.display();

		string expectedOutput("Test Menu\n"
			"=========\n"
			"---------\n");

		REQUIRE(expectedOutput == outputStream.str());
	}

	SECTION("ShouldDisplayMenuWithAddedMenuItem")
	{
		menu.addMenuItem('c', "Check", []() {});
		menu.display();

		string expectedOutput("Test Menu\n"
			"=========\n"
			"c. Check.\n"
			"---------\n");

		REQUIRE(expectedOutput == outputStream.str());
	}

	SECTION("ShouldOverrideExistingSubmenuWithSameKey")
	{
		menu.addSubmenu('s', "submenu item 1");
		menu.addSubmenu('s', "submenu item 2");

		menu.display();

		string expectedOutput("Test Menu\n"
			"=========\n"
			"s. submenu item 2.\n"
			"---------\n");
		REQUIRE(expectedOutput == outputStream.str());
	}

	SECTION("ShouldDisplayAddedSubmenus")
	{
		menu.addSubmenu('s', "Test submenu1");
		menu.addSubmenu('r', "Test submenu2");

		menu.display();
		menu.handleKey('s');

		string expectedOutput("Test Menu\n"
			"=========\n"
			"r. Test submenu2.\n"
			"s. Test submenu1.\n"
			"---------\n"
			"Test Menu > Test submenu1\n"
			"=========================\n"
			"b. Go back to Test Menu.\n"
			"-------------------------\n");
		REQUIRE(expectedOutput == outputStream.str());
	}

	SECTION("ShouldReturnToParentMenuFromSubmenu")
	{
		auto& submenu1 = menu.addSubmenu('s', "Test submenu1");
		submenu1.addSubmenu('r', "Test submenu2");

		menu.display();
		menu.handleKey('s');
		menu.handleKey('r');
		menu.handleKey('b');
		menu.handleKey('b');

		string expectedOutput("Test Menu\n"
			"=========\n"
			"s. Test submenu1.\n"
			"---------\n"
			"Test Menu > Test submenu1\n"
			"=========================\n"
			"b. Go back to Test Menu.\n"
			"r. Test submenu2.\n"
			"-------------------------\n"
			"Test Menu > Test submenu1 > Test submenu2\n"
			"=========================================\n"
			"b. Go back to Test Menu > Test submenu1.\n"
			"x. Return to root menu.\n"
			"-----------------------------------------\n"
			"Test Menu > Test submenu1\n"
			"=========================\n"
			"b. Go back to Test Menu.\n"
			"r. Test submenu2.\n"
			"-------------------------\n"
			"Test Menu\n"
			"=========\n"
			"s. Test submenu1.\n"
			"---------\n");
		REQUIRE(expectedOutput == outputStream.str());
	}

	SECTION("ShouldReturnToRootMenuFromMultipleSubmenusDown")
	{
		auto& submenu1 = menu.addSubmenu('s', "Test submenu1");
		submenu1.addSubmenu('r', "Test submenu2");

		menu.display();
		menu.handleKey('s');
		menu.handleKey('r');
		menu.handleKey('x');

		string expectedOutput("Test Menu\n"
			"=========\n"
			"s. Test submenu1.\n"
			"---------\n"
			"Test Menu > Test submenu1\n"
			"=========================\n"
			"b. Go back to Test Menu.\n"
			"r. Test submenu2.\n"
			"-------------------------\n"
			"Test Menu > Test submenu1 > Test submenu2\n"
			"=========================================\n"
			"b. Go back to Test Menu > Test submenu1.\n"
			"x. Return to root menu.\n"
			"-----------------------------------------\n"
			"Test Menu\n"
			"=========\n"
			"s. Test submenu1.\n"
			"---------\n");
		REQUIRE(expectedOutput == outputStream.str());
	}

	SECTION("ShouldDisplayMessageWhenHandlingAKeyThatHasNoAssociatedMenuItem")
	{
		menu.handleKey('m');

		string expectedOutput("No menu item matches the key 'm'.\n");
		REQUIRE(expectedOutput == outputStream.str());
	}

	SECTION("ShouldCallFunctionPassedInAMenuItem")
	{
		bool functionWasCalled = false;
		menu.addMenuItem('c', "Check key press", [&]() { functionWasCalled = true; });

		menu.handleKey('c');

		REQUIRE(functionWasCalled);
	}


	SECTION("ShouldDisplaySubmenu")
	{
		menu.addSubmenu('s', "Test submenu");
		menu.handleKey('s');

		string expectedOutput("Test Menu > Test submenu\n"
			"========================\n"
			"b. Go back to Test Menu.\n"
			"------------------------\n");
		REQUIRE(expectedOutput == outputStream.str());
	}

	SECTION("ShouldDisplaySubmenuAndHandleKeyForSubmenu")
	{
		menu.addSubmenu('s', "Test submenu", true);
		menu.handleKey('s');
		menu.handleKey('m');

		string expectedOutput("Test Menu > Test submenu\n"
			"========================\n"
			"b. Go back to Test Menu.\n"
			"m. Display menu.\n"
			"------------------------\n"
			"Test Menu > Test submenu\n"
			"========================\n"
			"b. Go back to Test Menu.\n"
			"m. Display menu.\n"
			"------------------------\n");
		REQUIRE(expectedOutput == outputStream.str());
	}

	SECTION("ShouldReturnToRootMenuFromSubmenu")
	{
		auto& testSubmenu = menu.addSubmenu('s', "Test submenu");
		testSubmenu.addSubmenu('r', "Doesn't matter");
		menu.handleKey('s');
		menu.handleKey('r');
		menu.handleKey('x');

		string expectedOutput("Test Menu > Test submenu\n"
			"========================\n"
			"b. Go back to Test Menu.\n"
			"r. Doesn't matter.\n"
			"------------------------\n"
			"Test Menu > Test submenu > Doesn't matter\n"
			"=========================================\n"
			"b. Go back to Test Menu > Test submenu.\n"
			"x. Return to root menu.\n"
			"-----------------------------------------\n"
			"Test Menu\n"
			"=========\n"
			"s. Test submenu.\n"
			"---------\n");
		REQUIRE(expectedOutput == outputStream.str());
	}
}
