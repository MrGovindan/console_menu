#include <sstream>

#include <gtest/gtest.h>

#include <console_menu/console_menu.h>

using namespace std;

// -------------------------------------------------------------------------------------------------

TEST(ConsoleMenu, DisplayDefaultMenu)
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

// -------------------------------------------------------------------------------------------------

TEST(ConsoleMenu, AbleToChangeMenuTitle)
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

// -------------------------------------------------------------------------------------------------

TEST(ConsoleMenu, DisplayEmptyMenu)
{
  ostringstream outputStream;
  ConsoleMenu menu("Test Menu", outputStream);

  menu.display();

  string expectedMenu("Test Menu\n"
                      "=========\n"
                      "---------\n");

  ASSERT_EQ(expectedMenu, outputStream.str());
}

// -------------------------------------------------------------------------------------------------

TEST(ConsoleMenu, AbleToAddAMenuItemByComponents)
{
  ostringstream outputStream;
  ConsoleMenu menu("Test Menu", outputStream, false);

  menu.addMenuItem('c', "Check", [](){});
  menu.display();

  string expectedMenu("Test Menu\n"
                      "=========\n"
                      "c. Check.\n"
                      "---------\n");

  ASSERT_EQ(expectedMenu, outputStream.str());
}

// -------------------------------------------------------------------------------------------------

TEST(ConsoleMenu, AbleToAddAWholeMenuItem)
{
  ostringstream outputStream;
  ConsoleMenu menu("Test Menu", outputStream, false);

  MenuItem menuItem{'c', "Check", [](){}};
  menu.addMenuItem(menuItem);
  menu.display();

  string expectedMenu("Test Menu\n"
                      "=========\n"
                      "c. Check.\n"
                      "---------\n");

  ASSERT_EQ(expectedMenu, outputStream.str());
}

// -------------------------------------------------------------------------------------------------

TEST(ConsoleMenu, HandlesKeyForDefaultDisplayMenuItem)
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

// -------------------------------------------------------------------------------------------------

TEST(ConsoleMenu, HandlesKeyThatHasNoAssociatedMenuItem)
{
  ostringstream outputStream;
  ConsoleMenu menu("Test Menu", outputStream);

  menu.handleKey('m');

  string expectedMenu("No menu item matches the key 'm'.\n");
  ASSERT_EQ(expectedMenu, outputStream.str());
}

// -------------------------------------------------------------------------------------------------

TEST(ConsoleMenu, HandlesKeyForAddedMenuItem)
{
  ostringstream outputStream;
  ConsoleMenu menu("Test Menu", outputStream);

  bool functionWasCalled = false;
  menu.addMenuItem('c', "Check key press", [&](){
      functionWasCalled = true;
    });

  menu.handleKey('c');

  ASSERT_TRUE(functionWasCalled);
}

// -------------------------------------------------------------------------------------------------

TEST(ConsoleMenu, DisplaysMultipleMenuItemsAlphabetically)
{
  ostringstream outputStream;
  ConsoleMenu menu("Test Menu", outputStream, true);
  menu.addMenuItem('c', "Test function", [&](){});

  menu.display();

  string expectedMenu("Test Menu\n"
                      "=========\n"
                      "c. Test function.\n"
                      "m. Display menu.\n"
                      "---------\n");
  ASSERT_EQ(expectedMenu, outputStream.str());
}

// -------------------------------------------------------------------------------------------------

TEST(ConsoleMenu, TestSubMenuItem)
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

// -------------------------------------------------------------------------------------------------

TEST(ConsoleMenu, TestSubMenuDisplayMenuItem)
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

// -------------------------------------------------------------------------------------------------

TEST(ConsoleMenu, TestSubMenuReturnToRootMenu)
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

// -------------------------------------------------------------------------------------------------

TEST(ConsoleMenu, TestSubMenuReturnToRootMenuFromManySubmenusDown)
{
  ostringstream outputStream;
  ConsoleMenu menu("Test Menu", outputStream);
  auto& submenu1 = menu.addSubmenu('s', "Test submenu1");
  ConsoleMenu& submenu2 = submenu1.addSubmenu('r', "Test submenu2");

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

// -------------------------------------------------------------------------------------------------

TEST(ConsoleMenu, TestSubMenuGoToParentMenu)
{
  ostringstream outputStream;
  ConsoleMenu menu("Test Menu", outputStream);
  auto& submenu1 = menu.addSubmenu('s', "Test submenu1");
  ConsoleMenu& submenu2 = submenu1.addSubmenu('r', "Test submenu2");

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

// -------------------------------------------------------------------------------------------------

TEST(ConsoleMenu, TestMultipleSubMenus)
{
  ostringstream outputStream;
  ConsoleMenu menu("Test Menu", outputStream);
  auto& submenu1 = menu.addSubmenu('s', "Test submenu1");
  auto& submenu2 = menu.addSubmenu('r', "Test submenu2");

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

// -------------------------------------------------------------------------------------------------
