#include "../Printer.h"
#include <iostream>
int main(){
  std::cerr << "Test driver for Printer" << std::endl;
  std::cerr << "Test starts" << std::endl;
  {
    Printer printer(2, 2, 3);
    printer.print(Printer::Kind::Parent, 'S');
    printer.print(Printer::Kind::Groupoff, 'S');
    printer.print(Printer::Kind::Parent, 'D', 1, 3);
    printer.print(Printer::Kind::Student, 0, 'S');
    printer.print(Printer::Kind::Vending, 1, 'S', 2);
    printer.print(Printer::Kind::Vending, 1, 'r');
    printer.print(Printer::Kind::Vending, 1, 'R');
  }
  std::cerr << "Test ends" << std::endl;
  return 0;
}