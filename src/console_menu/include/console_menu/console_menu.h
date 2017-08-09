#ifndef CONSOLE_MENU_H
#define CONSOLE_MENU_H

#include <sstream>
#include <string>
#include <functional>
#include <vector>
#include <list>
#include <memory>

struct MenuItem
{
  char key;
  std::string description;
  std::function<void()> function;
};

class ConsoleMenu
{
 private: // fields
  std::string menuTitle;
  std::string titleSeperator;
  std::string menuSeperator;
  std::ostream& outputStream;
  std::vector<MenuItem> menuItems;
  std::list<std::unique_ptr<ConsoleMenu>> submenus;
  ConsoleMenu* parentMenu;
  ConsoleMenu* activeSubmenu;

 private: // methods
  ConsoleMenu(const ConsoleMenu&) = delete;
  ConsoleMenu& operator=(const ConsoleMenu&) = delete;

  ConsoleMenu(const std::string& menuTitle,
              ConsoleMenu& parentMenu,
              bool addDefaultDisplayMenuItem);

  void addSubmenuItem(char key, const std::string& submenuName, ConsoleMenu& submenu);
  void addReturnToRoot(ConsoleMenu& submenu);
  void addReturnToParent(ConsoleMenu& submenu);

 public: // methods
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
};

#endif // CONSOLE_MENU_H
