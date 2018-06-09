#include <console_menu/MenuItem.h>

MenuItem::MenuItem(char key, const std::string& description, std::function<void()> function)
  : key{key}
  , description{description}
  , action{function}
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
