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
};

#endif // SUBMENU_ITEM_H
