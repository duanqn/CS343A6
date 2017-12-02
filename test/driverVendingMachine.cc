#include "../VendingMachine.h"
#include "../NameServer.h"
#include "../Student.h"
#include "../WATCard.h"
#include "../MPRNG.h"
#include "../Printer.h"

#include <iostream>
#include <sstream>

static VendingMachine* v;
static WATCard wc;
static unsigned int balance = 20;
static const int numtimes = 20;
static std::ostringstream out;

// NameServer test methods
void NameServer::main() {}
void NameServer::VMregister( VendingMachine* vm ) {}

// WATCard test methods
WATCard::WATCard() {}
void WATCard::withdraw( unsigned int amount ) {
  balance -= amount;
  out << "withdrew " << amount
            << " balance now: " << balance << '\n';
}
unsigned int WATCard::getBalance() {
  out << "getBalance() = " << balance << '\n';
  return balance;
}

// Student test methods
void Student::main() {
  out << "Student start " << '\n';
  for ( int i = 0; i < numtimes; i += 1 ) {
    try { _Enable {
      out << "buy()" << '\n';
      out << "balance: " << balance << '\n';
      out << "Stock: " << v->inventory()[0] << '\n';
      v->restocked();
      v->buy( VendingMachine::Flavours::BluesBlackCherry, wc );
      out << "\tPurchase!" << '\n';
    }}
    catch ( VendingMachine::Free ) {
      out << "\tFree!" << '\n';

    }
    catch ( VendingMachine::Funds ) {
      out << "\tFunds!" << '\n';
      balance += 13;
    }
    catch ( VendingMachine::Stock ) {
      out << "\tStock!" << '\n';
      v->inventory()[0] += 10;
      v->restocked();
    }
  }
  out << "Student end " << '\n';
}

MPRNG g_random( getpid() );

void uMain::main() {
  {
    out << "main() start " << '\n';
    Printer p( 1, 1, 1 );
    NameServer ns;
    v = new VendingMachine( p, ns, 0, 3, 5 );
    { Student s; }
    delete v;
    out << "main() end " << '\n';
  }
  std::cout << out.str() << std::endl;
}
