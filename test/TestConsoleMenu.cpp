#include <sstream>

#include <gtest/gtest.h>

#include <console_menu/ConsoleMenu.h>

using std::ostringstream;
using std::string;

TEST(ConsoleMenu, ShouldDisplayMenuWithDefaultMenuItem)
{
  ostringstream outputStream;
  ConsoleMenu menu("Test Menu", outputStream, true);

  menu.display();

  string expectedMenu("Test Menu\n"
                      "=========\n"
                      "m. Display menu.\n"
                      "---------\n");

  ASSERT_EQ(expectedMenu, outputStream.str());
}

TEST(ConsoleMenu, ShouldDisplayMenuWithChangedTitle)
{
  ostringstream outputStream;
  ConsoleMenu menu("Test Menu", outputStream, true);
  menu.setTitle("Another Title");

  menu.display();

  string expectedMenu("Another Title\n"
                      "=============\n"
                      "m. Display menu.\n"
                      "-------------\n");

  ASSERT_EQ(expectedMenu, outputStream.str());
}

TEST(ConsoleMenu, ShouldDisplayMenuWithNoMenuItems)
{
  ostringstream outputStream;
  ConsoleMenu menu("Test Menu", outputStream);

  menu.display();

  string expectedMenu("Test Menu\n"
                      "=========\n"
                      "---------\n");

  ASSERT_EQ(expectedMenu, outputStream.str());
}

TEST(ConsoleMenu, ShouldDisplayMenuWithAddedMenuItem)
{
  ostringstream outputStream;
  ConsoleMenu menu("Test Menu", outputStream, false);

  menu.addMenuItem('c', "Check", []() {});
  menu.display();

  string expectedMenu("Test Menu\n"
                      "=========\n"
                      "c. Check.\n"
                      "---------\n");

  ASSERT_EQ(expectedMenu, outputStream.str());
}

TEST(ConsoleMenu, ShouldHandleKeyForDefaultMenuItem)
{
  ostringstream outputStream;
  ConsoleMenu menu("Test Menu", outputStream, true);

  menu.handleKey('m');

  string expectedMenu("Test Menu\n"
                      "=========\n"
                      "m. Display menu.\n"
                      "---------\n");

  ASSERT_EQ(expectedMenu, outputStream.str());
}

TEST(ConsoleMenu, ShouldDisplayMessageWhenHandlingAKeyThatHasNoAssociatedMenuItem)
{
  ostringstream outputStream;
  ConsoleMenu menu("Test Menu", outputStream);

  menu.handleKey('m');

  string expectedMenu("No menu item matches the key 'm'.\n");
  ASSERT_EQ(expectedMenu, outputStream.str());
}

TEST(ConsoleMenu, ShouldCallFunctionPassedInAMenuItem)
{
  ostringstream outputStream;
  ConsoleMenu menu("Test Menu", outputStream);

  bool functionWasCalled = false;
  menu.addMenuItem('c', "Check key press", [&]() { functionWasCalled = true; });

  menu.handleKey('c');

  ASSERT_TRUE(functionWasCalled);
}

TEST(ConsoleMenu, ShouldDisplayMenuWithMultipleMenuItemsAdded)
{
  ostringstream outputStream;
  ConsoleMenu menu("Test Menu", outputStream, true);
  menu.addMenuItem('c', "Test function", [&]() {});

  menu.display();

  string expectedMenu("Test Menu\n"
                      "=========\n"
                      "c. Test function.\n"
                      "m. Display menu.\n"
                      "---------\n");
  ASSERT_EQ(expectedMenu, outputStream.str());
}

TEST(ConsoleMenu, ShouldDisplaySubmenu)
{
  ostringstream outputStream;
  ConsoleMenu menu("Test Menu", outputStream);
  menu.addSubmenu('s', "Test submenu");
  menu.handleKey('s');

  string expectedMenu("Test Menu > Test submenu\n"
                      "========================\n"
                      "b. Go back to Test Menu.\n"
                      "------------------------\n");
  ASSERT_EQ(expectedMenu, outputStream.str());
}

TEST(ConsoleMenu, ShouldDisplaySubmenuAndHandleKeyForSubmenu)
{
  ostringstream outputStream;
  ConsoleMenu menu("Test Menu", outputStream);
  menu.addSubmenu('s', "Test submenu", true);
  menu.handleKey('s');
  menu.handleKey('m');

  string expectedMenu("Test Menu > Test submenu\n"
                      "========================\n"
                      "b. Go back to Test Menu.\n"
                      "m. Display menu.\n"
                      "------------------------\n"
                      "Test Menu > Test submenu\n"
                      "========================\n"
                      "b. Go back to Test Menu.\n"
                      "m. Display menu.\n"
                      "------------------------\n");
  ASSERT_EQ(expectedMenu, outputStream.str());
}

TEST(ConsoleMenu, ShouldReturnToRootMenuFromSubmenu)
{
  ostringstream outputStream;
  ConsoleMenu menu("Test Menu", outputStream);
  auto& testSubmenu = menu.addSubmenu('s', "Test submenu");
  testSubmenu.addSubmenu('r', "Doesn't matter");
  menu.handleKey('s');
  menu.handleKey('r');
  menu.handleKey('x');

  string expectedMenu("Test Menu > Test submenu\n"
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
  ASSERT_EQ(expectedMenu, outputStream.str());
}

TEST(ConsoleMenu, ShouldReturnToRootMenuFromMultipleSubmenusDown)
{
  ostringstream outputStream;
  ConsoleMenu menu("Test Menu", outputStream);
  auto& submenu1 = menu.addSubmenu('s', "Test submenu1");
  submenu1.addSubmenu('r', "Test submenu2");

  menu.display();
  menu.handleKey('s');
  menu.handleKey('r');
  menu.handleKey('x');

  string expectedMenu("Test Menu\n"
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
  ASSERT_EQ(expectedMenu, outputStream.str());
}

TEST(ConsoleMenu, ShouldReturnToParentMenuFromSubmenu)
{
  ostringstream outputStream;
  ConsoleMenu menu("Test Menu", outputStream);
  auto& submenu1 = menu.addSubmenu('s', "Test submenu1");
  submenu1.addSubmenu('r', "Test submenu2");

  menu.display();
  menu.handleKey('s');
  menu.handleKey('r');
  menu.handleKey('b');
  menu.handleKey('b');

  string expectedMenu("Test Menu\n"
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
  ASSERT_EQ(expectedMenu, outputStream.str());
}

TEST(ConsoleMenu, ShouldDisplayAddedSubmenus)
{
  ostringstream outputStream;
  ConsoleMenu menu("Test Menu", outputStream);
  menu.addSubmenu('s', "Test submenu1");
  menu.addSubmenu('r', "Test submenu2");

  menu.display();
  menu.handleKey('s');

  string expectedMenu("Test Menu\n"
                      "=========\n"
                      "r. Test submenu2.\n"
                      "s. Test submenu1.\n"
                      "---------\n"
                      "Test Menu > Test submenu1\n"
                      "=========================\n"
                      "b. Go back to Test Menu.\n"
                      "-------------------------\n");
  ASSERT_EQ(expectedMenu, outputStream.str());
}

TEST(ConsoleMenu, ShouldOverrideExistingMenuItemWithSameKey)
{
  ostringstream outputStream;
  ConsoleMenu menu("Test Menu", outputStream);
  menu.addMenuItem('s', "menu item 1", []() {});
  menu.addMenuItem('s', "menu item 2", []() {});

  menu.display();

  string expectedMenu("Test Menu\n"
                      "=========\n"
                      "s. menu item 2.\n"
                      "---------\n");
  ASSERT_EQ(expectedMenu, outputStream.str());
}

TEST(ConsoleMenu, ShouldOverrideExistingSubmenuWithSameKey)
{
  ostringstream outputStream;
  ConsoleMenu menu("Test Menu", outputStream);
  menu.addSubmenu('s', "submenu item 1");
  menu.addSubmenu('s', "submenu item 2");

  menu.display();

  string expectedMenu("Test Menu\n"
                      "=========\n"
                      "s. submenu item 2.\n"
                      "---------\n");
  ASSERT_EQ(expectedMenu, outputStream.str());
}
