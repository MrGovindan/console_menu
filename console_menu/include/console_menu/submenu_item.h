#ifndef SUBMENU_ITEM_H
#define SUBMENU_ITEM_H

#include <string>

#include "menu_item.h"
#include "console_menu.h"

class SubmenuItem : public MenuItem, public ConsoleMenu
{
 public:
  SubmenuItem(char key,
              const std::string& menuTitle,
              bool addDefaultDisplayMenuItem,
              ConsoleMenu& parent);

 private:
  ~SubmenuItem() = default;
  SubmenuItem(const SubmenuItem&) = delete;
  SubmenuItem operator=(const SubmenuItem&) = delete;
};

#endif // SUBMENU_ITEM_H
