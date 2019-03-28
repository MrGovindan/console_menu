#include <console_menu/MenuItem.h>

MenuItem::MenuItem(char key, std::string description, std::function<void()> function)
  : key{key}
  , description{std::move(description)}
  , action{std::move(function)}
{
}

char MenuItem::getKey() const
{
  return key;
}

void MenuItem::performAction()
{
  action();
}

std::ostream& operator<<(std::ostream& outputStream, const MenuItem& menuItem)
{
  outputStream << menuItem.key << ". " << menuItem.description << "." << std::endl;
  return outputStream;
}
