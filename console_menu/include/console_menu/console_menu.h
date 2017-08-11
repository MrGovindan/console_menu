#ifndef CONSOLE_MENU_H
#define CONSOLE_MENU_H

#include <sstream>
#include <string>
#include <functional>
#include <vector>
#include <list>
#include <memory>

#include "menu_item.h"

class SubmenuItem;

class ConsoleMenu
{
  friend class SubmenuItem;

 private:
  std::string title;
  std::ostream& outputStream;
  std::list<std::unique_ptr<MenuItem>> menuItems;
  ConsoleMenu* parentMenu;
  ConsoleMenu* activeSubmenu;

 public:
  ConsoleMenu(const std::string& menuTitle,
              std::ostream& outputStream,
              bool addDefaultDisplayMenuItem = false);

  virtual ~ConsoleMenu() = default;

  void display();
  void setTitle(const std::string& menuTitle);

  void addMenuItem(char key, const std::string& description, std::function<void()> function);

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
  void addMenuItem(MenuItem* menuItem);
  void addReturnToRoot(ConsoleMenu& submenu);
  void addReturnToParent(ConsoleMenu& submenu);

  std::string getDisplayTitle();
};

#endif // CONSOLE_MENU_H
