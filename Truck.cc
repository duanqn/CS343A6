#include "Truck.h"
#include "MPRNG.h"
#include "config.h"
#include "VendingMachine.h"
#include "NameServer.h"
#include "BottlingPlant.h"

extern MPRNG g_random;
extern ConfigParms g_config;

Truck::Truck( Printer & prt, NameServer & nameServer, BottlingPlant & plant,
  unsigned int numVendingMachines, unsigned int maxStockPerFlavour ) 
  : printer( prt ), server( nameServer), plant( plant ), 
    numVendingMachines( numVendingMachines ), maxStockPerFlavour( maxStockPerFlavour) 
    {
  // obtain the location of each vending machine
  machineList = server.getMachineList();
}

void Truck::main() {
  unsigned int cargo[ ::NUM_FLAVOURS ];

  for ( ;; ) {
    // yeild [1, 10] times
    yield( ::g_random( 1, 10 ) );

    try { _Enable {
      //obtain a new shipment of soda
      plant.getShipment( cargo ); 
    }}
    catch ( BottlingPlant::Shutdown ) {
      // just terminate and wait for deletion 
      return;
    }

    VM: for ( unsigned int cycleCount = 0 ;; vmIndex = ( vmIndex + 1 ) % numVendingMachines, cycleCount += 1 ) {
      // check if we have run out of cargo stock
      if ( ( cargo[ VendingMachine::Flavours::BluesBlackCherry] == 0
          && cargo[ VendingMachine::Flavours::ClassicCreamSoda] == 0
          && cargo[ VendingMachine::Flavours::RockRootBeer]     == 0
          && cargo[ VendingMachine::Flavours::JazzLime]         == 0 )
          || cycleCount == numVendingMachines )
    break VM;

      // get the inventory for this vending machine
      unsigned int* vmStock = machineList[vmIndex]->inventory();

      for ( unsigned int flavourIndex = 0; flavourIndex < ::NUM_FLAVOURS; flavourIndex += 1 ) {
        // determine how much to deliver 
        unsigned int delivery = ::g_config.maxStockPerFlavour - vmStock[flavourIndex];
        if ( delivery > cargo[flavourIndex] )
          delivery = cargo[flavourIndex];

        // deliver that amount and update cargo stock
        cargo[flavourIndex] -= delivery;
        vmStock[flavourIndex] += delivery;
        machineList[vmIndex]->restocked();
      } // for

    } // for VM

  }

}
