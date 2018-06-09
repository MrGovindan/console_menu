#include <console_menu/ConsoleMenu.h>

#include <algorithm>

#include <console_menu/SubmenuItem.h>

const char TITLE_SEPARATOR = '=';
const char MENU_SEPARATOR = '-';

using namespace std;

ConsoleMenu::ConsoleMenu(const string& menu_title,
                         ostream& output_stream,
                         bool add_display_menu_item)
  : menu_title{menu_title}
  , output_stream{output_stream}
  , parent_menu{nullptr}
  , active_submenu{nullptr}
{
  if (add_display_menu_item)
    addMenuItem('m', "Display menu", [&] { display(); });
}

void ConsoleMenu::display()
{
  if (active_submenu) {
    active_submenu->display();
  }
  else {
    auto display_title = getDisplayTitle();
    output_stream << display_title << endl;

    string title_separator = string().append(display_title.size(), TITLE_SEPARATOR);
    output_stream << title_separator << endl;

    for (auto& menu_item : menu_items)
      output_stream << (*menu_item.get());

    string menu_separator = string().append(display_title.size(), MENU_SEPARATOR);
    output_stream << menu_separator << endl;
  }
}

string ConsoleMenu::getDisplayTitle() const
{
  if (parent_menu)
    return string(parent_menu->getDisplayTitle() + " > " + menu_title);
  else
    return menu_title;
}

void ConsoleMenu::setTitle(const string& menu_title)
{
  this->menu_title = menu_title;
}

void ConsoleMenu::addMenuItem(char key, const string& description, function<void()> action)
{
  addMenuItem(new MenuItem(key, description, action));
}

void ConsoleMenu::addMenuItem(MenuItem* menu_item)
{
  eraseMenuItemWithKey(menu_item->getKey());
  menu_items.emplace_back(menu_item);
  menu_items.sort(
      [](const unique_ptr<MenuItem>& menu_item1, const unique_ptr<MenuItem>& menu_item2) {
        return menu_item1->getKey() < menu_item2->getKey();
      });
}

void ConsoleMenu::eraseMenuItemWithKey(char key)
{
  auto menu_item = findMenuItemWithKey(key);
  if (menuItemWasFound(menu_item)) {
    menu_items.erase(menu_item);
  }
}

list<unique_ptr<MenuItem>>::iterator ConsoleMenu::findMenuItemWithKey(char key)
{
  return find_if(menu_items.begin(), menu_items.end(), [&key](unique_ptr<MenuItem>& menu_item) {
    return menu_item->getKey() == key;
  });
}

ConsoleMenu&
ConsoleMenu::addSubmenu(char key, const string& submenu_title, bool add_display_menu_item)
{
  auto submenu = new SubmenuItem(key, submenu_title, add_display_menu_item, *this);
  addMenuItem(submenu);

  if (parent_menu)
    addReturnToRoot(*submenu);

  addReturnToParent(*submenu);

  return *submenu;
}

ConsoleMenu::ConsoleMenu(const string& menu_title,
                         ConsoleMenu& parent_menu,
                         bool add_display_menu_item)
  : ConsoleMenu(menu_title, parent_menu.output_stream, add_display_menu_item)
{
  this->parent_menu = &parent_menu;
}

void ConsoleMenu::addSubmenuItem(char key, const string& submenu_title, ConsoleMenu& submenu)
{
  addMenuItem(key, submenu_title, [&]() {
    active_submenu = &submenu;
    display();
  });
}

void ConsoleMenu::addReturnToRoot(ConsoleMenu& submenu)
{
  submenu.addMenuItem('x', "Return to root menu", [&submenu]() {
    auto parent_menu = submenu.parent_menu;
    while (parent_menu->parent_menu) {
      parent_menu->active_submenu = nullptr;
      parent_menu = parent_menu->parent_menu;
    }
    parent_menu->active_submenu = nullptr;
    parent_menu->display();
  });
}

void ConsoleMenu::addReturnToParent(ConsoleMenu& submenu)
{
  submenu.addMenuItem('b', "Go back to " + submenu.parent_menu->getDisplayTitle(), [&]() {
    auto parent_menu = submenu.parent_menu;
    parent_menu->active_submenu = nullptr;
    parent_menu->display();
  });
}

void ConsoleMenu::handleKey(char key)
{
  if (active_submenu) {
    active_submenu->handleKey(key);
  }
  else {
    auto menu_item = findMenuItemWithKey(key);

    if (menuItemWasFound(menu_item))
      (*menu_item)->performAction();
    else
      output_stream << "No menu item matches the key '" << key << "'." << endl;
  }
}

bool ConsoleMenu::menuItemWasFound(std::list<std::unique_ptr<MenuItem>>::iterator& iterator) const
{
  return iterator != menu_items.end();
}
