#include "../Groupoff.h"
#include "../MPRNG.h"
#include "../Printer.h"

#include <uFuture.h>
#include <iostream>
#include <sstream>

static const unsigned int numStudents = 7;
static const unsigned int sodaCost = 5;
static unsigned int depositTotal = 0;
static WATCard::FWATCard fwatCard[numStudents];
static std::ostringstream out;
MPRNG g_random( getpid() );

// WATCard test methods
WATCard::WATCard() {}
void WATCard::deposit( unsigned int val ) {
  depositTotal += val;
}
void WATCard::withdraw( unsigned int val ) {
  depositTotal -= val;
}
unsigned int WATCard::getBalance() {
  return depositTotal;
}

void uMain::main() {
  {
    out << "main() start" << '\n';
    Printer p( 1, 1, 1 );
    Groupoff g( p, numStudents, sodaCost, 1 );
    for ( unsigned int i = 0; i < numStudents; i += 1 ) {
      fwatCard[i] = g.giftCard();
    }
    yield( 1 );
    for ( unsigned int i = 0; i < numStudents; i += 1 ) {
      if ( fwatCard[i].available() ) {
        out << "\tgot it! it contains $" << sodaCost << '\n';
        fwatCard[i]()->withdraw( sodaCost );
        out << "\tfreeing card " << i << '\n';
        delete fwatCard[i]();
        fwatCard[i].reset();
      } else {
        out << "\tdidn't get it.." << '\n';
        yield( 1 );
      }
    }
    out << "main() end" << '\n';
  }
  
  std::cout << out.str() << std::endl;
}
