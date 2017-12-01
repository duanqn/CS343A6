#pragma once

_Monitor Printer;
_Task NameServer;
_Task BottlingPlant;
_Task VendingMachine;

_Task Truck {
  void main();
  Printer& printer;
  NameServer& server;
  BottlingPlant& plant;
  const unsigned int numVendingMachines;
  const unsigned int maxStockPerFlavour;
  VendingMachine** machineList = NULL;
  unsigned int vmIndex = 0;

  public:
  Truck( Printer & prt, NameServer & nameServer, BottlingPlant & plant,
    unsigned int numVendingMachines, unsigned int maxStockPerFlavour );

#ifdef DEBUG
  // a default ctor exclusively for unit tests to make dummy objects
  Truck() :
    printer( *static_cast<Printer*>(NULL) ),
    server( *static_cast<NameServer*>(NULL) ),
    plant( *static_cast<BottlingPlant*>(NULL) ),
    numVendingMachines( 0 ),
    maxStockPerFlavour( 0 )
    {}
#endif
};
