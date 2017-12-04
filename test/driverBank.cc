#include "../Bank.h"
#include "../MPRNG.h"

#include <iostream>
#include <sstream>

#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))

static const unsigned int money = 10;
static const unsigned int maxWithdraw = 5;
static const unsigned int maxDeposit = 3;
static const unsigned int numStuds = 3;
static Bank* b;
MPRNG g_random( getpid() );

_Task Withdrawer {
    const unsigned int studId;

    void main() {
      std::cerr << "Withdrawer " << studId << " start" << std::endl;
      unsigned int totalWithdraw = money;
      while ( totalWithdraw > 0 ) {
        // withdraw some money
        unsigned int val = g_random( 1, MIN(totalWithdraw, maxWithdraw) );
        std::cerr << "\tWithdrawer " << studId << " withdrew: $" << val << std::endl;
        b->withdraw( studId, val );
        totalWithdraw -= val;
      }
      std::cerr << "Withdrawer " << studId << " end" << std::endl;
    } // main

  public:
    static unsigned int idcount;
    Withdrawer() : studId( idcount ) { idcount += 1; }
};
unsigned int Withdrawer::idcount = 0;


_Task Depositer {
    const unsigned int studId;

    void main() {
      std::cerr << "Depositer " << studId << " start" << std::endl;
      unsigned int totalDeposit = money;
      while ( totalDeposit > 0 ) {
        // deposit some money
        unsigned int val = g_random( 1, MIN(totalDeposit, maxDeposit) );
        std::cerr << "\tDepositer " << studId << " deposited: $" << val << std::endl;
        b->deposit( studId, val );
        totalDeposit -= val;
      }
      std::cerr << "Depositer " << studId << " end" << std::endl;
    } // main

  public:
    static unsigned int idcount;
    Depositer() : studId( idcount ) { idcount += 1; }
};
unsigned int Depositer::idcount = 0;

void uMain::main() {
  std::cerr << "main() start" << std::endl;
  b = new Bank( numStuds );
  {
    Withdrawer w[numStuds];
    Depositer d[numStuds];
  }
  delete b;
  std::cerr << "main() end" << std::endl;
}