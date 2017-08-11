#include <console_menu/console_menu.h>

#include <algorithm>

#include <console_menu/submenu_item.h>

const char TITLE_SEPARATOR = '=';
const char MENU_SEPARATOR = '-';

using namespace std;

ConsoleMenu::ConsoleMenu(const string& menuTitle,
                         ostream& outputStream,
                         bool addDefaultDisplayMenuItem)
  : title{menuTitle}
  , outputStream{outputStream}
  , parentMenu{nullptr}
  , activeSubmenu{nullptr}
{
  if (addDefaultDisplayMenuItem)
    addMenuItem('m', "Display menu", [&](){display();});
}

void ConsoleMenu::display()
{
  if (activeSubmenu) {
    activeSubmenu->display();
  }
  else {
    auto displayTitle = getDisplayTitle();
    outputStream << displayTitle << endl;

    string titleSeparator = string().append(displayTitle.size(), TITLE_SEPARATOR);
    outputStream << titleSeparator << endl;

    for (auto& menuItem : menuItems) {
      outputStream << (*menuItem.get());
    }

    string menuSeparator = string().append(displayTitle.size(), MENU_SEPARATOR);
    outputStream << menuSeparator << endl;
  }
}

string ConsoleMenu::getDisplayTitle()
{
  if (parentMenu)
    return string(parentMenu->getDisplayTitle() + " > "+ title);
  else
    return title;
}

void ConsoleMenu::setTitle(const string& menuTitle)
{
  title = menuTitle;
}

void ConsoleMenu::addMenuItem(char key,
                              const string& description,
                              function<void()> function)
{
  auto menuItem = new MenuItem(key, description, function);
  addMenuItem(menuItem);
}

void ConsoleMenu::addMenuItem(MenuItem* menuItem)
{
  eraseMenuItemWithSameKey(menuItem->getKey());
  menuItems.emplace_back(menuItem);
  menuItems.sort([](const unique_ptr<MenuItem>& menuItem1, const unique_ptr<MenuItem>& menuItem2) {
      return menuItem1->getKey() < menuItem2->getKey();
    });
}

void ConsoleMenu::eraseMenuItemWithSameKey(char key)
{
  auto menuItem = findMenuItemWithKey(key);
  if (menuItem != menuItems.end()) {
    menuItems.erase(menuItem);
  }
}

list<unique_ptr<MenuItem>>::iterator ConsoleMenu::findMenuItemWithKey(char key)
{
  return find_if(menuItems.begin(), menuItems.end(), [&key](unique_ptr<MenuItem>& menuItem) {
      return menuItem->getKey() == key;
    });
}

ConsoleMenu& ConsoleMenu::addSubmenu(char key,
                                     const string& submenuTitle,
                                     bool addDefaultDisplayMenuItem)
{
  auto submenu = new SubmenuItem(key, submenuTitle, addDefaultDisplayMenuItem, *this);
  addMenuItem(submenu);

  if (parentMenu)
    addReturnToRoot(*submenu);

  addReturnToParent(*submenu);

  return *submenu;
}

ConsoleMenu::ConsoleMenu(const string& menuTitle,
                         ConsoleMenu& parentMenu,
                         bool addDefaultDisplayMenuItem)
  : ConsoleMenu(menuTitle,
                parentMenu.outputStream,
                addDefaultDisplayMenuItem)
{
  this->parentMenu = &parentMenu;
}

void ConsoleMenu::addSubmenuItem(char key, const string& submenuName, ConsoleMenu& submenu)
{
  addMenuItem(key, submenuName, [&](){
      activeSubmenu = &submenu;
      display();
    });
}

void ConsoleMenu::addReturnToRoot(ConsoleMenu& submenu)
{
  submenu.addMenuItem('x', "Return to root menu", [&submenu](){
      ConsoleMenu* parentMenu = submenu.parentMenu;
      while (parentMenu->parentMenu) {
        parentMenu->activeSubmenu = nullptr;
        parentMenu = parentMenu->parentMenu;
      }
      parentMenu->activeSubmenu = nullptr;
      parentMenu->display();
    });
}

void ConsoleMenu::addReturnToParent(ConsoleMenu& submenu)
{
  submenu.addMenuItem('b', "Go back to " + submenu.parentMenu->getDisplayTitle(), [&](){
      ConsoleMenu* parentMenu = submenu.parentMenu;
      parentMenu->activeSubmenu = nullptr;
      parentMenu->display();
    });
}

void ConsoleMenu::handleKey(char key)
{
  if (activeSubmenu) {
    activeSubmenu->handleKey(key);
  }
  else {
    auto menuItem = findMenuItemWithKey(key);

    if (menuItem != menuItems.end())
      (*menuItem)->performFunction();
    else
      outputStream << "No menu item matches the key '" << key << "'." << endl;
  }
}
