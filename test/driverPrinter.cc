#include "../Printer.h"
#include <iostream>
int main(){
  std::cout << "Test driver for Printer" << std::endl;
  std::cout << "Test starts" << std::endl;
  Printer printer(2, 2, 3);
  
  std::cout << "Test ends" << std::endl;
  return 0;
}