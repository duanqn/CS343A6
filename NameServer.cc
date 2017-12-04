#include "NameServer.h"
#include "Printer.h"

NameServer::NameServer( Printer& prt, 
  unsigned int numVendingMachines, unsigned int numStudents )
  : printer( prt ), numVendingMachines( numVendingMachines ),
    numStudents( numStudents ) 
    {
  studentMachineTable = new unsigned int[numStudents];
  machineList = new VendingMachine*[numVendingMachines];
}

NameServer::~NameServer() {
  delete[] studentMachineTable;
  delete[] machineList;
}

void NameServer::main() {
  printer.print( Printer::Kind::NameServer, 'S' );

  // register each of the machine's address
  for ( unsigned int mach = 0; mach < numVendingMachines; mach += 1 ) {
    _Accept( VMregister ) {
      machineList[mach] = machAddr;
      printer.print( Printer::Kind::NameServer, 'R', mach );
    }
  }

  // give the truck the list
  _Accept( getMachineList )

  // distributethe students across vending machines in round-robin fashion
  for ( unsigned int stud = 0, mach = 0;
        stud < numStudents;
        stud += 1, mach = (mach + 1) % numVendingMachines ) {
    _Accept( getMachine ) {
      studentMachineTable[ chair.front() ] = mach;
      printer.print( Printer::Kind::NameServer, 'N', stud, mach );
      chair.signalBlock();
    }

  } // for 

  for ( ;; ) {
    _Accept( getMachine ) {
      // give them the next machine, wrap around if necessary 
      unsigned int studId = chair.front();
      unsigned int newMach = (studentMachineTable[ studId ] + 1) % numVendingMachines;
      studentMachineTable[ studId ] = newMach;
      printer.print( Printer::Kind::NameServer, 'N', studId, newMach );
      chair.signalBlock();
    }
    or _Accept( ~NameServer ) {
      break;
    }

  } // for

  printer.print( Printer::Kind::NameServer, 'F' );
}

void NameServer::VMregister( VendingMachine* vendingmachine ) {
  machAddr = vendingmachine;
}

VendingMachine* NameServer::getMachine( unsigned int id ) {
  chair.wait( id );
  return machineList[ studentMachineTable[id] ];
}

VendingMachine** NameServer::getMachineList() {
  return machineList;
}
