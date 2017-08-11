#include <console_menu/submenu_item.h>

#include <iostream>
using namespace std;

SubmenuItem::SubmenuItem(char key,
                         const std::string& menuTitle,
                         bool addDefaultDisplayMenuItem,
                         ConsoleMenu& parent)
  : MenuItem{key, menuTitle, [&](){
    parent.activeSubmenu = (ConsoleMenu*) this;
    parent.display();
  }}
  , ConsoleMenu{menuTitle, parent, addDefaultDisplayMenuItem}
{
}

// make -C /home/jesse/Documents/console_menu/build/ -j1 default_target && /home/jesse/Documents/console_menu/build/console_menu/test_console_menu
