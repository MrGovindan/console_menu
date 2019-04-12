#include <console_menu/SubmenuItem.h>

SubmenuItem::SubmenuItem(char key,
                         const std::string& menuTitle,
                         bool addDefaultDisplayMenuItem,
                         ConsoleMenu& parent)
  : MenuItem{key,
             menuTitle,
             [&]() {
               parent.active_submenu = static_cast<ConsoleMenu*>(this);
               parent.display();
             }}
  , ConsoleMenu{menuTitle, parent, addDefaultDisplayMenuItem}
{
}
