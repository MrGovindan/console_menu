#include <iostream>

#include <console_menu/console_menu.h>

using namespace std;

class Adder
{
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
  ConsoleMenu mathMenu("Math Menu", cout);

  auto& adderMenu = mathMenu.addSubmenu('a', "Adder");
  Adder adder;
  adderMenu.addMenuItem('2',
                        "Add two values together",
                        std::bind(&Adder::addTwoValues, &adder));
  adderMenu.addMenuItem('2',
                        "Add two values together",
                        std::bind(&Adder::addTwoValues, &adder));

  auto& complexMenu = mathMenu.addSubmenu('c', "Complex Operations");
  complexMenu.addSubmenu('d', "Division", true);

  auto& multiplicationMenu = complexMenu.addSubmenu('m', "Multiplication");
  multiplicationMenu.addMenuItem('m', "Multiply two values", [&mathMenu](){
      multiplyTwoValues();
      mathMenu.display();
    });

  bool appRunning = true;
  mathMenu.addMenuItem('x', "Exit", [&appRunning](){appRunning = false;});

  mathMenu.display();

  char key;
  while (appRunning) {
    cin >> key;
    mathMenu.handleKey(key);
  }
}
