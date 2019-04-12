#ifndef CONSOLE_MENU_H
#define CONSOLE_MENU_H

#include <functional>
#include <list>
#include <memory>
#include <sstream>
#include <string>

#include "MenuItem.h"

class SubmenuItem;

class ConsoleMenu {
  friend class SubmenuItem;

private:
  std::string menu_title;
  std::ostream& output_stream;
  std::list<std::unique_ptr<MenuItem>> menu_items;
  ConsoleMenu* parent_menu;
  ConsoleMenu* active_submenu;

public:
  ConsoleMenu(std::string menuTitle,
              std::ostream& output_stream,
              bool add_display_menu_item = false);

  ConsoleMenu(const ConsoleMenu&) = delete;
  ConsoleMenu& operator=(const ConsoleMenu&) = delete;

  virtual ~ConsoleMenu() = default;

  void display();

  void setTitle(std::string menu_title);

  void addMenuItem(char key, const std::string& description, std::function<void()> action);

  ConsoleMenu& addSubmenu(char key,
                          const std::string& submenu_title,
                          bool add_default_display_menu_item = false);

  void handleKey(char key);

private:
  ConsoleMenu(const std::string& menu_title,
              ConsoleMenu& parent_menu,
              bool add_default_display_menu_item);

  void eraseMenuItemWithKey(char key);
  std::list<std::unique_ptr<MenuItem>>::iterator findMenuItemForKey(char key);

  void addSubmenuItem(char key, const std::string& submenu_title, ConsoleMenu& submenu);
  void addMenuItem(MenuItem* menu_item);
  void addReturnToRoot(ConsoleMenu& submenu);
  void addReturnToParent(ConsoleMenu& submenu);
  bool menuItemWasFound(std::list<std::unique_ptr<MenuItem>>::iterator& iterator) const;

  std::string getDisplayTitle() const;
};

#endif // CONSOLE_MENU_H
