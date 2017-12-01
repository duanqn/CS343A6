#pragma once 

#include "config.h"

_Monitor Printer;
_Task NameServer;
_Task Truck;

_Task BottlingPlant {
  void main();
  Printer& printer;
  NameServer& server;
  const unsigned int numVendingMachines;
  const unsigned int maxShippedPerFlavour;
  const unsigned int maxStockPerFlavour;
  const unsigned int timeBetweenShipments;
  bool shutdownFlag = false;
  unsigned int storage[ ::NUM_FLAVOURS ];
  Truck* truck;

  public:
  _Event Shutdown {};                       // shutdown plant
  BottlingPlant( Printer &prt, NameServer &nameServer, unsigned int numVendingMachines,
               unsigned int maxShippedPerFlavour, unsigned int maxStockPerFlavour,
               unsigned int timeBetweenShipments );
  ~BottlingPlant();
  void getShipment( unsigned int cargo[] );

#ifdef DEBUG
  // a default ctor exclusively for unit tests to make dummy objects
  BottlingPlant() :
    printer( *static_cast<Printer*>(NULL) ),
    server( *static_cast<NameServer*>(NULL) ),
    numVendingMachines( 0 ),
    maxShippedPerFlavour( 0 ),
    maxStockPerFlavour( 0 ),
    timeBetweenShipments( 0 ) 
    {}
#endif
};

