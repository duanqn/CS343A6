#include "../NameServer.h"
#include "../Printer.h"
#include "../Student.h"
#include "../Truck.h"
#include "../VendingMachine.h"

#include <iostream>
#include <sstream>

static const unsigned int numVendingMachines = 3;
static const unsigned int numStudents = 5;
static const unsigned int extraCalls = 10;
static NameServer* ns;
static std::ostringstream out;

// Student test methods
void Student::main() {
  out << "Student start" << '\n';
  for ( unsigned int i = 0; i < numStudents; i += 1 ) {
    ns->getMachine( i );
  }
  for ( unsigned int i = 0; i < extraCalls; i += 1 ) {
    ns->getMachine( i % numStudents );
  }
  out << "Student end" << '\n';
}

// Truck test methods
void Truck::main() {
  out << "Truck start" << '\n';
  VendingMachine* myVend = *(ns->getMachineList());
  myVend->restocked();
  out << "Truck end" << '\n';
}

// VendingMachine test methods
void VendingMachine::main() {}
void VendingMachine::restocked() {
  out << "\trestocked()!" << '\n';
}

// tester task
_Task tester {
    void main() {
    out << "VendingMachine start" << '\n';
    static VendingMachine vends[numVendingMachines];
    out << "made vends" << std::endl;
    for ( unsigned int i = 0; i < numVendingMachines; i += 1 ) {
      ns->VMregister( vends + i );
    }
    out << "VendingMachine end" << '\n';
  }
};

void uMain::main() {
  {
    out << "main() start" << '\n';
    Printer p( 1, 1, 1 );
    ns = new NameServer( p, numVendingMachines, numStudents );
    Truck t;
    tester vm;
    { Student s; }
    delete ns;
    out << "main() end" << '\n';
  }
  std::cout << out.str() << std::endl;
}
