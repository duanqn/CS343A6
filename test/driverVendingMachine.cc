#include "../VendingMachine.h"
#include "../Printer.h"
#include "../NameServer.h"
#include "../Student.h"
#include "../WATCard.h"
#include "../MPRNG.h"

#include <iostream>

static VendingMachine* v;
static WATCard wc;
static unsigned int balance = 20;
static const int numtimes = 10;

// NameServer test methods
void NameServer::main() {}
void NameServer::VMregister( VendingMachine* vm ) {}

// WATCard test methods
WATCard::WATCard() {}
void WATCard::withdraw( unsigned int amount ) {
  balance -= amount;
  std::cerr << "withdrew " << amount
            << " balance now: " << balance << std::endl;
}
unsigned int WATCard::getBalance() {
  std::cerr << "getBalance() = " << balance << std::endl;
  return balance;
}

// Student test methods
void Student::main() {
  std::cerr << "Student start " << std::endl;
  for ( int i = 0; i < numtimes; i += 1 ) {
    try { _Enable {
      std::cerr << "buy()" << std::endl;
      std::cerr << "balance: " << balance << std::endl;
      std::cerr << "Stock: " << v->inventory()[0] << std::endl;
      v->restocked();
      v->buy( VendingMachine::Flavours::BluesBlackCherry, wc );
      std::cerr << "\tPurchase!" << std::endl;
    }}
    catch ( VendingMachine::Free ) {
      std::cerr << "\tFree!" << std::endl;

    }
    catch ( VendingMachine::Funds ) {
      std::cerr << "\tFunds!" << std::endl;
      balance += 13;
    }
    catch ( VendingMachine::Stock ) {
      std::cerr << "\tStock!" << std::endl;
      v->inventory()[0] += 10;
      v->restocked();
    }
  }
  std::cerr << "Student end " << std::endl;
}

MPRNG g_random( getpid() );

void uMain::main() {
  std::cerr << "main() start " << std::endl;

  Printer p;
  NameServer ns;
  v = new VendingMachine( p, ns, 0, 3, 5 );
  { Student s; }
  delete v;

  std::cerr << "main() end " << std::endl;
}
