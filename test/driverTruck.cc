#include "../Truck.h"
#include "../MPRNG.h"
#include "../BottlingPlant.h"
#include "../NameServer.h"
#include "../VendingMachine.h"
#include "../config.h"
#include "../Printer.h"

#include <iostream>
#include <sstream>

static int numMachine = 3;
static VendingMachine** machines;
static std::ostringstream out;

// NameServer test methods
void NameServer::main() {}
VendingMachine** NameServer::getMachineList() {
  out << "getMachineList() " << '\n';

  machines = new VendingMachine*[numMachine];
  for ( int i = 0; i < numMachine; i += 1 ){
    machines[i] = new VendingMachine();
  }
  return machines;
}

// BottlingPlant test methods
void BottlingPlant::main() {}
void BottlingPlant::getShipment( unsigned int cargo[] ) {
  out << "getShipment() " << '\n'
            << "  cargo: "
            << cargo[0] << ' '
            << cargo[1] << ' '
            << cargo[2] << ' '
            << cargo[3] << ' ' << '\n';

  static int count = 0;
  count += 1;
  if ( count == 5 ) {
    out << "throw Shutdown() " << '\n';
    throw Shutdown();
  }

  cargo[0] = 3;
  cargo[1] = 3;
  cargo[2] = 3;
  cargo[3] = 3;
}
BottlingPlant::~BottlingPlant() {}

// VendingMachine test methods
void VendingMachine::main() {}
unsigned int* VendingMachine::inventory() {
  static unsigned int inventory[] = {2, 4, 8, 16};
  out << "inventory() " << '\n'

            << "inventory: "
            << inventory[0] << ' '
            << inventory[1] << ' '
            << inventory[2] << ' '
            << inventory[3] << ' ' << '\n';
  
  return inventory;
}
void VendingMachine::restocked() {}


MPRNG g_random( getpid() );
ConfigParms g_config;

void uMain::main() {
  out << "main() start " << '\n';
  ::g_config.maxStockPerFlavour = 20;
  
  {
    Printer p( 1, 1, 1 );
    NameServer ns;
    BottlingPlant bp;
    { Truck t( p, ns, bp, numMachine, ::g_config.maxStockPerFlavour ); }

    for ( int i = 0; i < numMachine; i += 1 ) {
      delete machines[i];
    }
    delete machines;
  }

  out << "main() end " << '\n';
  std::cout << out.str() << std::endl;
}

