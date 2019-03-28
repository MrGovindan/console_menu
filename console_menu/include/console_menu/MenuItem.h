#ifndef MENU_ITEM_H
#define MENU_ITEM_H

#include <functional>
#include <ostream>
#include <string>

class MenuItem {
protected:
  char key;
  std::string description;
  std::function<void()> action;

public:
  MenuItem(char key, std::string description, std::function<void()> function);
  virtual ~MenuItem() = default;
  char getKey() const;
  void performAction();

  friend std::ostream& operator<<(std::ostream& outputStream, const MenuItem& item);

private:
  MenuItem(const MenuItem&) = delete;
  MenuItem operator=(const MenuItem&) = delete;
};

#endif // MENU_ITEM_H
