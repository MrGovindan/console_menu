#include <console_menu/console_menu.h>

#include <algorithm>

// -------------------------------------------------------------------------------------------------

const char TITLE_SEPERATOR = '=';
const char MENU_SEPERATOR = '-';

// -------------------------------------------------------------------------------------------------

ConsoleMenu::ConsoleMenu(const std::string& menuTitle,
                         std::ostream& outputStream,
                         bool addDefaultDisplayMenuItem)
  : outputStream{outputStream}
  , parentMenu{nullptr}
  , activeSubmenu{nullptr}
{
  setTitle(menuTitle);
  if (addDefaultDisplayMenuItem)
    addMenuItem('m', "Display menu", [&](){display();});
}

// -------------------------------------------------------------------------------------------------

void ConsoleMenu::display()
{
  if (activeSubmenu) {
    activeSubmenu->display();
  }
  else {
    outputStream << menuTitle << std::endl;
    outputStream << titleSeperator << std::endl;

    for (auto& menuItem : menuItems) {
      outputStream << menuItem.key << ". " << menuItem.description << "." << std::endl;
    }

    outputStream << menuSeperator << std::endl;
  }
}

// -------------------------------------------------------------------------------------------------

void ConsoleMenu::setTitle(const std::string& menuTitle)
{
  this->menuTitle = menuTitle;
  titleSeperator.clear();
  titleSeperator.append(this->menuTitle.size(), TITLE_SEPERATOR);
  menuSeperator.clear();
  menuSeperator.append(this->menuTitle.size(), MENU_SEPERATOR);
}

// -------------------------------------------------------------------------------------------------

void ConsoleMenu::addMenuItem(char key,
                              const std::string& description,
                              std::function<void()> function)
{
  auto existingMenuItem = findExistingMenuItem(key);

  if (existingMenuItem != menuItems.end()) {
    existingMenuItem->description = description;
    existingMenuItem->function = function;
  }

  else {
    menuItems.emplace_back(MenuItem{key, description, function});
    std::sort(menuItems.begin(),
              menuItems.end(),
              [](MenuItem& item1, MenuItem& item2){ return item1.key < item2.key; });
  }
}

// -------------------------------------------------------------------------------------------------

std::vector<MenuItem>::iterator ConsoleMenu::findExistingMenuItem(char key)
{
  return std::find_if(menuItems.begin(),
                      menuItems.end(),
                      [&key](MenuItem& menuItem) {return menuItem.key == key;});
}

// -------------------------------------------------------------------------------------------------

ConsoleMenu& ConsoleMenu::addSubmenu(char key,
                                     const std::string& submenuTitle,
                                     bool addDefaultDisplayMenuItem)
{
  auto menu = new ConsoleMenu(submenuTitle, *this, addDefaultDisplayMenuItem);
  submenus.emplace_back(std::move(std::unique_ptr<ConsoleMenu>(menu)));
  auto& submenuReference = *(submenus.back().get());

  addSubmenuItem(key, submenuTitle, submenuReference);

  if (parentMenu)
    addReturnToRoot(submenuReference);

  addReturnToParent(submenuReference);

  return submenuReference;
}

// -------------------------------------------------------------------------------------------------

ConsoleMenu::ConsoleMenu(const std::string& menuTitle,
                         ConsoleMenu& parentMenu,
                         bool addDefaultDisplayMenuItem)
  : ConsoleMenu(parentMenu.menuTitle + " > " + menuTitle,
                parentMenu.outputStream,
                addDefaultDisplayMenuItem)
{
  this->parentMenu = &parentMenu;
}

// -------------------------------------------------------------------------------------------------

void ConsoleMenu::addSubmenuItem(char key, const std::string& submenuName, ConsoleMenu& submenu)
{
  addMenuItem(key, submenuName, [&](){
      activeSubmenu = &submenu;
      display();
    });
}

// -------------------------------------------------------------------------------------------------

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

// -------------------------------------------------------------------------------------------------

void ConsoleMenu::addReturnToParent(ConsoleMenu& submenu)
{
  submenu.addMenuItem('b', "Go back to " + menuTitle, [&](){
      ConsoleMenu* parentMenu = submenu.parentMenu;
      parentMenu->activeSubmenu = nullptr;
      parentMenu->display();
    });
}

// -------------------------------------------------------------------------------------------------

void ConsoleMenu::handleKey(char key)
{
  if (activeSubmenu) {
    activeSubmenu->handleKey(key);
  }
  else {
    auto menuItem = std::find_if(menuItems.begin(),
                                 menuItems.end(),
                                 [&](MenuItem& menuItem) { return menuItem.key == key; });

    if (menuItem != menuItems.end())
      menuItem->function();
    else
      outputStream << "No menu item matches the key '" << key << "'." << std::endl;
  }
}

// -------------------------------------------------------------------------------------------------
