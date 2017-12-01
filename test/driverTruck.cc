#include "../Truck.h"
#include "../MPRNG.h"
#include "../BottlingPlant.h"
#include "../NameServer.h"
#include "../VendingMachine.h"
#include "../config.h"

#include <iostream>

_Monitor Printer {};

void NameServer::main() {}
static int numMachine = 3;
static VendingMachine** machines;
VendingMachine** NameServer::getMachineList() {
  std::cout << "getMachineList() " << std::endl;

  machines = new VendingMachine*[numMachine];
  for ( int i = 0; i < numMachine; i += 1 ){
    machines[i] = new VendingMachine();
  }
  return machines;
}

void BottlingPlant::main() {}
void BottlingPlant::getShipment( unsigned int cargo[] ) {
  std::cout << "getShipment() " << std::endl
            << "  cargo: "
            << cargo[0] << ' '
            << cargo[1] << ' '
            << cargo[2] << ' '
            << cargo[3] << ' ' << std::endl;

  static int count = 0;
  count += 1;
  if ( count == 5 ) {
    std::cout << "throw Shutdown() " << std::endl;
    throw Shutdown();
  }

  cargo[0] = 3;
  cargo[1] = 3;
  cargo[2] = 3;
  cargo[3] = 3;
}

void VendingMachine::main() {}

unsigned int* VendingMachine::inventory() {
  static unsigned int inventory[] = {2, 4, 8, 16};
  std::cout << "inventory() " << std::endl

            << "inventory: "
            << inventory[0] << ' '
            << inventory[1] << ' '
            << inventory[2] << ' '
            << inventory[3] << ' ' << std::endl;
  
  return inventory;
}

MPRNG g_random( getpid() );

ConfigParms g_config;

void uMain::main() {
  std::cout << "main() start " << std::endl;

  ::g_config.maxStockPerFlavour = 20;
  Printer p;
  NameServer ns;
  BottlingPlant bp;
  { Truck t( p, ns, bp, numMachine, ::g_config.maxStockPerFlavour ); }

  for ( int i = 0; i < numMachine; i += 1 ) {
    delete machines[i];
  }
  delete machines;

  std::cout << "main() end " << std::endl;
}

