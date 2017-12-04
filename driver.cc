// Command syntax:
//   $ soda [ config-file [ Seed ] ]
// all parameters are optional. Defaults: config-file=>soda.config, seed=>pid
//

#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;          // direct access to std
#include <cstdlib>            // exit

#include "MPRNG.h"
#include "config.h"
#include "Printer.h"
#include "Bank.h"
#include "Parent.h"
#include "WATCard.h"
#include "WATCardOffice.h"
#include "Groupoff.h"
#include "NameServer.h"
#include "VendingMachine.h"
#include "BottlingPlant.h"
#include "Student.h"

bool convert( int &val, char *buffer ) {    // convert C string to integer
  std::stringstream ss( buffer );     // connect stream and buffer
  string temp;
  ss >> dec >> val;         // convert integer from buffer
  return ! ss.fail() &&       // conversion successful ?
    ! ( ss >> temp );       // characters after conversion all blank ?
} // convert


void usage( char *argv[] ) {
  cerr  << "Usage: " << argv[0]
        << " [ config-file [ seed (> 0) ] ]" 
        << endl;
  exit( EXIT_FAILURE );       // TERMINATE
} // usage

MPRNG g_random( getpid() );
ConfigParms g_config;

void uMain::main() {
  istream *infile = NULL;       // default value

  switch ( argc ) {
  case 3:
    {
      unsigned int seed;
      int tmpseed;
      if ( ! convert( tmpseed, argv[2] ) || tmpseed <= 0 ) { // invalid integer ?
        usage( argv );
        // exit
      } else {
        seed = (unsigned int) tmpseed;
      }
      g_random.set_seed( seed );
      std::cerr << "seed: " << seed << std::endl;
    }
    // FALL THROUGH
  case 2:
    processConfigFile( argv[1], g_config );
    break;
  case 1:           // all defaults
    processConfigFile( "soda.config", g_config );
    break;
  default:            // wrong number of options
    usage( argv );
  } // switch


  VendingMachine** vendingMachines;
  Printer       printer( g_config.numStudents, g_config.numVendingMachines, g_config.numCouriers );
  Bank             bank( g_config.numStudents );
  {
    // creates in order, the printer, bank, parent, WATCard office, groupoff, 
    //   name server, vending machines, bottling plant, and students
    Parent       parent( printer, bank, g_config.numStudents, g_config.parentalDelay );
    WATCardOffice watco( printer, bank, g_config.numCouriers );
    Groupoff   groupoff( printer, g_config.numStudents, g_config.sodaCost, g_config.groupoffDelay );
    NameServer   server( printer, g_config.numVendingMachines, g_config.numStudents );

    vendingMachines = new VendingMachine*[g_config.numVendingMachines];
    for ( unsigned int id = 0; id < g_config.numVendingMachines; id += 1 )
      vendingMachines[id] = new 
         VendingMachine( printer, server, id, g_config.sodaCost, g_config.maxStockPerFlavour );

    BottlingPlant plant( printer, server, g_config.numVendingMachines, g_config.maxShippedPerFlavour, 
                         g_config.maxStockPerFlavour, g_config.timeBetweenShipments );

    Student** students = new Student*[g_config.numStudents];
    for ( unsigned int id = 0; id < g_config.numStudents; id += 1 )
      students[id] = new 
                Student( printer, server, watco, groupoff, id, g_config.maxPurchases );
    for ( unsigned int id = 0; id < g_config.numStudents; id += 1 )
      delete students[id];
    delete[] students;
  }
  // delete the bottling plant before deleting the vending machines to allow 
  //   the truck to complete its final deliveries to the vending machines
  for ( unsigned int id = 0; id < g_config.numVendingMachines; id += 1 )
    delete vendingMachines[id];
  delete[] vendingMachines;
  
  if ( infile ) delete infile;    // close file, do not delete cin!
}

