#pragma once 

#include "VendingMachine.h"

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
  unsigned int storage[ VendingMachine::Flavours::TotalFlavourNumber ];
  Truck* truck;

  public:
  _Event Shutdown {};                       // shutdown plant
  BottlingPlant( Printer &prt, NameServer &nameServer, unsigned int numVendingMachines,
               unsigned int maxShippedPerFlavour, unsigned int maxStockPerFlavour,
               unsigned int timeBetweenShipments );
  ~BottlingPlant();
  void getShipment( unsigned int cargo[] );
};

