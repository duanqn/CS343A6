#pragma once

#include "VendingMachine.h"

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
  unsigned int cargo[VendingMachine::Flavours::TotalFlavourNumber];

  inline unsigned int remainingSoda() {
    return  cargo[VendingMachine::Flavours::BluesBlackCherry] +
            cargo[VendingMachine::Flavours::ClassicCreamSoda] +
            cargo[VendingMachine::Flavours::RockRootBeer] +
            cargo[VendingMachine::Flavours::JazzLime];
  }

  public:
  Truck( Printer & prt, NameServer & nameServer, BottlingPlant & plant,
    unsigned int numVendingMachines, unsigned int maxStockPerFlavour );
};
