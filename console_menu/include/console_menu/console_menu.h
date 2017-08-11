#ifndef CONSOLE_MENU_H
#define CONSOLE_MENU_H

#include <sstream>
#include <string>
#include <functional>
#include <vector>
#include <list>
#include <memory>

#include "menu_item.h"
#include "submenu_item.h"

class ConsoleMenu
{
 private:
  std::string title;
  std::string titleSeperator;
  std::string menuSeperator;
  std::ostream& outputStream;
  std::list<std::unique_ptr<MenuItem>> menuItems;
  ConsoleMenu* parentMenu;
  ConsoleMenu* activeSubmenu;

 public:
  ConsoleMenu(const std::string& menuTitle,
              std::ostream& outputStream,
              bool addDefaultDisplayMenuItem = false);

  void display();
  void setTitle(const std::string& menuTitle);

  void addMenuItem(char key, const std::string& description, std::function<void()> function);
  void addMenuItem(MenuItem& menuItem);

  ConsoleMenu& addSubmenu(char key,
                          const std::string& submenuTitle,
                          bool addDefaultDisplayMenuItem = false);

  void handleKey(char key);

 private:
  ConsoleMenu(const ConsoleMenu&) = delete;
  ConsoleMenu& operator=(const ConsoleMenu&) = delete;

  ConsoleMenu(const std::string& menuTitle,
              ConsoleMenu& parentMenu,
              bool addDefaultDisplayMenuItem);

  void eraseMenuItemWithSameKey(char key);
  std::list<std::unique_ptr<MenuItem>>::iterator findMenuItemWithKey(char key);

  void addSubmenuItem(char key, const std::string& submenuName, ConsoleMenu& submenu);
  void addReturnToRoot(ConsoleMenu& submenu);
  void addReturnToParent(ConsoleMenu& submenu);

  std::string getDisplayTitle();
};

#endif // CONSOLE_MENU_H
