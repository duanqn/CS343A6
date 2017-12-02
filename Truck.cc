#include "Truck.h"
#include "MPRNG.h"
#include "config.h"
#include "VendingMachine.h"
#include "NameServer.h"
#include "BottlingPlant.h"
#include "Printer.h"

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
  printer.print( Printer::Kind::Truck, 'S' );

  for ( ;; ) {
    // yeild [1, 10] times
    yield( ::g_random( 1, 10 ) );

    try { _Enable {
      //obtain a new shipment of soda
      plant.getShipment( cargo ); 
      printer.print( Printer::Kind::Truck, 'P', remainingSoda() );
    }}
    catch ( BottlingPlant::Shutdown ) {
      // just terminate and wait for deletion 
      printer.print( Printer::Kind::Truck, 'F' );
      return;
    }

    VM: for ( unsigned int cycleCount = 0 ;; vmIndex = ( vmIndex + 1 ) % numVendingMachines, cycleCount += 1 ) {
      // check if we have run out of cargo stock
      if ( remainingSoda() == 0 || cycleCount == numVendingMachines )
    break VM;

      printer.print( Printer::Kind::Truck, 'd', vmIndex, remainingSoda() );

      // get the inventory for this vending machine
      unsigned int* vmStock = machineList[vmIndex]->inventory();

      // keep track of soda slots not replenished for the printer
      unsigned int numNotReplenished = 0;
      for ( unsigned int flavourIndex = 0;
            flavourIndex < VendingMachine::Flavours::TotalFlavourNumber;
            flavourIndex += 1 ) {
        // determine how much to deliver 
        unsigned int delivery = ::g_config.maxStockPerFlavour - vmStock[flavourIndex];
        if ( delivery > cargo[flavourIndex] ){
          // could not full restock, add difference to numNotReplenished
          numNotReplenished += delivery - cargo[flavourIndex];
          delivery = cargo[flavourIndex];
        }

        // deliver that amount and update cargo stock
        cargo[flavourIndex] -= delivery;
        vmStock[flavourIndex] += delivery;
      } // for

      printer.print( Printer::Kind::Truck, 'U', vmIndex, numNotReplenished );
      printer.print( Printer::Kind::Truck, 'D', vmIndex, remainingSoda() );
      machineList[vmIndex]->restocked();

    } // for VM

  }

}
