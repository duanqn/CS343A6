#include "../MPRNG.h"
#include "../config.h"
#include "../BottlingPlant.h"
#include "../NameServer.h"
#include "../Truck.h"
#include "../Printer.h"

#include <iostream>
#include <sstream>

static BottlingPlant* bp;
static std::ostringstream out;

// Truck test methods
void Truck::main() {
  out << "start tester " << '\n';

  unsigned int cargo[4];
  for ( ;; ) {
    try { _Enable {
      bp->getShipment( cargo );
      out << "continue tester " << '\n';
    }}
    catch ( BottlingPlant::Shutdown ) {
      out << "end tester " << '\n';
      return;
    }
    out << cargo[0] << ' '
              << cargo[1] << ' '
              << cargo[2] << ' '
              << cargo[3] << '\n';
  }
}
Truck::Truck( Printer & prt, NameServer & nameServer, BottlingPlant & plant,
  unsigned int numVendingMachines, unsigned int maxStockPerFlavour ) 
  : printer( prt ), server( nameServer), plant( plant ), 
    numVendingMachines( numVendingMachines ), maxStockPerFlavour( maxStockPerFlavour) 
    {}

// NameServer test methods
void NameServer::main() {}

MPRNG g_random( getpid() );

void uMain::main() {
  {
    Printer p( 1, 1, 1 );
    NameServer ns;

    bp = new BottlingPlant( p, ns, 4, 4, 4, 4 );
    yield( 30 );
    delete bp;
  }
  std::cout << out.str() << std::endl;
}
