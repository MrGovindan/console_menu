#ifndef SUBMENU_ITEM_H
#define SUBMENU_ITEM_H

#include <string>

#include "ConsoleMenu.h"
#include "MenuItem.h"

class SubmenuItem : public MenuItem, public ConsoleMenu {
public:
  SubmenuItem(char key, std::string menuTitle, bool addDefaultDisplayMenuItem, ConsoleMenu& parent);

private:
  ~SubmenuItem() = default;
  SubmenuItem(const SubmenuItem&) = delete;
  SubmenuItem operator=(const SubmenuItem&) = delete;
};

#endif // SUBMENU_ITEM_H
