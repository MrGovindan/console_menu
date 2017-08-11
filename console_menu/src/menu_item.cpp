#include <console_menu/menu_item.h>

MenuItem::MenuItem(char key,
                   const std::string& description,
                   std::function<void()> function)
  : key{key}
  , description{description}
  , function{function}
{
}

char MenuItem::getKey() const
{
  return key;
}

const std::string& MenuItem::getDescription() const
{
  return description;
}

void MenuItem::performFunction()
{
  function();
}
