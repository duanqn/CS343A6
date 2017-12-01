#include "../MPRNG.h"
#include "../config.h"
#include "../BottlingPlant.h"
#include "../NameServer.h"
#include "../Truck.h"
#include <iostream>

_Monitor Printer {};

MPRNG g_random( getpid() );
ConfigParms g_config;

static BottlingPlant* bp;

void Truck::main() {
  std::cout << "start tester " << std::endl;

  unsigned int cargo[4];
  for ( ;; ) {
    try { _Enable {
      bp->getShipment( cargo );
      std::cout << "continue tester " << std::endl;
    }}
    catch ( BottlingPlant::Shutdown ) {
      std::cout << "end tester " << std::endl;
      return;
    }
    std::cout << cargo[0] << ' '
              << cargo[1] << ' '
              << cargo[2] << ' '
              << cargo[3] << std::endl;
  }
}
Truck::Truck( Printer & prt, NameServer & nameServer, BottlingPlant & plant,
  unsigned int numVendingMachines, unsigned int maxStockPerFlavour ) 
  : printer( prt ), server( nameServer), plant( plant ), 
    numVendingMachines( numVendingMachines ), maxStockPerFlavour( maxStockPerFlavour) 
    {}

void NameServer::main() {}

void uMain::main() {
  Printer p;
  NameServer ns;

  bp = new BottlingPlant( p, ns, 4, 4, 4, 4 );
  yield( 30 );
  delete bp;
   
}
