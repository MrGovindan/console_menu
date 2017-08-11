#ifndef MENU_ITEM_H
#define MENU_ITEM_H

#include <string>
#include <functional>

class MenuItem
{
 protected:
  char key;
  std::string description;
  std::function<void()> function;

 public:
  MenuItem(char key,
           const std::string& description,
           std::function<void()> function);
  virtual ~MenuItem() = default;
  char getKey() const;
  const std::string& getDescription() const;
  void performFunction();

 private:
  MenuItem(const MenuItem&) = delete;
  MenuItem operator=(const MenuItem&) = delete;
};



#endif // MENU_ITEM_H
