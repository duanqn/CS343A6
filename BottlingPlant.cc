#include "BottlingPlant.h"
#include "Truck.h"
#include "VendingMachine.h"
#include "MPRNG.h"
#include "config.h"
#include "Printer.h"

extern MPRNG g_random;
extern ConfigParms g_config;

BottlingPlant::BottlingPlant( Printer &prt, NameServer &nameServer, 
  unsigned int numVendingMachines, unsigned int maxShippedPerFlavour, 
  unsigned int maxStockPerFlavour, unsigned int timeBetweenShipments ) 
  : printer( prt ), server( nameServer), 
    numVendingMachines( numVendingMachines ), maxShippedPerFlavour( maxShippedPerFlavour ),
    maxStockPerFlavour( maxStockPerFlavour ), timeBetweenShipments( timeBetweenShipments )
    {
  // create a truck
  truck = new Truck( printer, server, *this, numVendingMachines, maxStockPerFlavour );
}

BottlingPlant::~BottlingPlant() {
  delete truck;
}


void BottlingPlant::main() {
  printer.print( Printer::Kind::BottlingPlant, 'S' );

  for ( ;; ) {
    // yield for TimeBetweenShipments times (not random)
    yield( timeBetweenShipments );

    // perform a production run of [0, MaxShippedPerFlavour] per flavour
    storage[ VendingMachine::Flavours::BluesBlackCherry] = ::g_random( maxShippedPerFlavour );
    storage[ VendingMachine::Flavours::ClassicCreamSoda] = ::g_random( maxShippedPerFlavour );
    storage[ VendingMachine::Flavours::RockRootBeer]     = ::g_random( maxShippedPerFlavour );
    storage[ VendingMachine::Flavours::JazzLime]         = ::g_random( maxShippedPerFlavour );

    printer.print( Printer::Kind::BottlingPlant, 'G',
      storage[ VendingMachine::Flavours::BluesBlackCherry] +
      storage[ VendingMachine::Flavours::ClassicCreamSoda] +
      storage[ VendingMachine::Flavours::RockRootBeer] +
      storage[ VendingMachine::Flavours::JazzLime] );

    // wait for the truck to get the shipment or for a terminate call
    _Accept( ~BottlingPlant ) {
      // wait for the truck to come by and tell it to shutdown
      shutdownFlag = true;
      _Accept( getShipment ) {
        printer.print( Printer::Kind::BottlingPlant, 'F' );
        return;
      }
    } 
    or _Accept( getShipment ) {
      printer.print( Printer::Kind::BottlingPlant, 'P' );
    }

  } // for

}

void BottlingPlant::getShipment( unsigned int cargo[] ) {
  if ( shutdownFlag ) {
    _Resume Shutdown() _At *truck;
    return;
  }

  cargo[ VendingMachine::Flavours::BluesBlackCherry]
    = storage[ VendingMachine::Flavours::BluesBlackCherry];

  cargo[ VendingMachine::Flavours::ClassicCreamSoda]
    = storage[ VendingMachine::Flavours::ClassicCreamSoda];

  cargo[ VendingMachine::Flavours::RockRootBeer]
    = storage[ VendingMachine::Flavours::RockRootBeer];

  cargo[ VendingMachine::Flavours::JazzLime]
    = storage[ VendingMachine::Flavours::JazzLime];
}
