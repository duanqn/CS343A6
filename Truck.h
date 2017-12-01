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
};
