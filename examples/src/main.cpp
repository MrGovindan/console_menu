#include <iostream>

#include <console_menu/ConsoleMenu.h>

using namespace std;

class Adder {
public:
  void addTwoValues()
  {
    int value1;
    int value2;
    cout << "Enter value1: ";
    cin >> value1;

    cout << "Enter value2: ";
    cin >> value2;
    cout << "The sum of " << value1 << " and " << value2 << " is " << value1 + value2 << endl;
  }
};

void multiplyTwoValues()
{
  int value1;
  int value2;
  cout << "Enter value1: ";
  cin >> value1;

  cout << "Enter value2: ";
  cin >> value2;
  cout << "The product of " << value1 << " and " << value2 << " is " << value1 * value2 << endl;
}

int main()
{
  ConsoleMenu math_menu("Math Menu", cout);

  auto& adder_menu = math_menu.addSubmenu('a', "Adder");

  // Adding a class member variable as an menu action
  Adder adder;
  adder_menu.addMenuItem('2', "Add two values together", std::bind(&Adder::addTwoValues, &adder));

  auto& complex_menu = math_menu.addSubmenu('c', "Complex Operations");

  // Adding a lambda function as a menu action
  auto& multiplication_menu = complex_menu.addSubmenu('m', "Multiplication");
  multiplication_menu.addMenuItem('m', "Multiply two values", [&math_menu]() {
    multiplyTwoValues();
    math_menu.display();
  });

  bool app_running = true;
  math_menu.addMenuItem('x', "Exit", [&app_running]() { app_running = false; });

  math_menu.display();

  char key;
  while (app_running) {
    cin >> key;
    math_menu.handleKey(key);
  }
}
