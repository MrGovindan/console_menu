#ifndef SUBMENU_ITEM_H
#define SUBMENU_ITEM_H

#include "ConsoleMenu.h"
#include "MenuItem.h"

class SubmenuItem : public MenuItem, public ConsoleMenu {
public:
  SubmenuItem(char key,
              const std::string& menuTitle,
              bool addDefaultDisplayMenuItem,
              ConsoleMenu& parent);

  SubmenuItem(const SubmenuItem&) = delete;
  SubmenuItem operator=(const SubmenuItem&) = delete;

private:
  ~SubmenuItem() override = default;
};

#endif // SUBMENU_ITEM_H
