#include <console_menu/submenu_item.h>

SubmenuItem::SubmenuItem(char key,
                         const std::string& menuTitle,
                         bool addDefaultDisplayMenuItem,
                         ConsoleMenu& parent)
  : MenuItem{key, menuTitle, [&]() {
      parent.activeSubmenu = (ConsoleMenu*) this;
      parent.display();
    }}
  , ConsoleMenu{menuTitle, parent, addDefaultDisplayMenuItem}
{}
