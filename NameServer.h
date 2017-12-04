#pragma once

_Monitor Printer;
_Task VendingMachine;

_Task NameServer {
    void main();
    Printer& printer;
    const unsigned int numVendingMachines;
    const unsigned int numStudents;
    unsigned int* studentMachineTable;
    VendingMachine** machineList;
    VendingMachine* machAddr;
    uCondition chair;

  public:
    NameServer( Printer& prt, unsigned int numVendingMachines, unsigned int numStudents );
    ~NameServer();
    void VMregister( VendingMachine* vendingmachine );
    VendingMachine* getMachine( unsigned int id );
    VendingMachine** getMachineList();
  
#ifdef DEBUG
    NameServer() :
      printer( *static_cast<Printer*>(NULL) ),
      numVendingMachines( 0 ),
      numStudents( 0 )
      {}
#endif
};

