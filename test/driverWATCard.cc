#include "../WATCard.h"
#include <iostream>
#include <uCobegin.h>
int main(){
  WATCard card1, card2;
  std::cerr << "Test for WATCard" << std::endl;
  std::cerr << "Test begins" << std::endl;
  uProcessor p[3];
  COBEGIN
    BEGIN
      card1.deposit(10);
      std::cerr << "Thread 1 deposits 10 into card 1" << std::endl;
      card1.withdraw(5);
      std::cerr << "Thread 1 withdraws 5 from card 1" << std::endl;
      card2.withdraw(3);
      std::cerr << "Thread 1 withdraws 3 from card 2" << std::endl;
    END
    BEGIN
      card1.withdraw(1);
      std::cerr << "Thread 2 withdraws 1 from card 1" << std::endl;
      card2.deposit(2);
      std::cerr << "Thread 2 deposits 2 into card 2" << std::endl;
      card1.withdraw(2);
      std::cerr << "Thread 2 withdraws 2 from card 1" << std::endl;
    END
    BEGIN
      card2.deposit(2);
      std::cerr << "Thread 3 deposits 2 into card 2" << std::endl;
      card1.withdraw(2);
      std::cerr << "Thread 3 withdraws 2 from card 1" << std::endl;
    END
    BEGIN
      card2.withdraw(1);
      std::cerr << "Thread 4 withdraws 1 from card 2" << std::endl;
    END
  COEND
  std::cerr << "Test ends" << std::endl;
  return 0;
}