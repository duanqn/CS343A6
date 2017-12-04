#include "VendingMachine.h"
#include "NameServer.h"
#include "WATCard.h"
#include "MPRNG.h"
#include "config.h"
#include "Printer.h"

extern MPRNG g_random;
extern ConfigParms g_config;

VendingMachine::VendingMachine( Printer & prt, NameServer & nameServer, 
  unsigned int id, unsigned int sodaCost, unsigned int maxStockPerFlavour )
  : printer( prt ), server( nameServer ), id( id ), sodaCost( sodaCost ),
    maxStockPerFlavour( maxStockPerFlavour ) {
  // register with the name server.
  server.VMregister( this );
  sodaStock[Flavours::BluesBlackCherry] = 0;
  sodaStock[Flavours::ClassicCreamSoda] = 0;
  sodaStock[Flavours::RockRootBeer]     = 0;
  sodaStock[Flavours::JazzLime]         = 0;
}

void VendingMachine::main() {
  printer.print( Printer::Kind::Vending, id, 'S' );

  for ( ;; ) {
    // a truck has come by to restock
    _Accept( inventory ) {
      // restocking is in progress, wait till completion
      printer.print( Printer::Kind::Vending, id, 'r' );
      _Accept( restocked ) {
        printer.print( Printer::Kind::Vending, id, 'R' );
      }
    } 

    // a student wants to buy soda
    or _Accept( buy ) {

      if ( sodaStock[studentOrder.flavour] < 1 ) {
        // out of stock for this flavour
        studentOrder.response = StudentOrder::Response::Stock;
      }

      else if ( ::g_random( 4 ) == 0 ) {
        // 1 in 5 chance succeded, student gets free soda
        sodaStock[studentOrder.flavour] -= 1;
        printer.print( Printer::Kind::Vending, id, 'B', 
          studentOrder.flavour, sodaStock[studentOrder.flavour] );
        studentOrder.response = StudentOrder::Response::Free;
      }

      else if ( studentOrder.card->getBalance() < sodaCost ) {
        // student has insufficient funds on their card
        studentOrder.response = StudentOrder::Response::Funds;
      }

      else {
        // debit the card, give the student the soda
        studentOrder.card->withdraw( sodaCost );
        sodaStock[studentOrder.flavour] -= 1;
        printer.print( Printer::Kind::Vending, id, 'B', 
          studentOrder.flavour, sodaStock[studentOrder.flavour] );
        studentOrder.response = StudentOrder::Response::Purchase;
      }

      // now wait for the student to read the response 
      chair.signalBlock();

    } // _Accept buy

    // dtor call put last to prevent student or truck from being left blocked
    or _Accept( ~VendingMachine ) {
      break;
    }
  } // for 
  
  printer.print( Printer::Kind::Vending, id, 'F' );
}

void VendingMachine::buy( Flavours flavour, WATCard & card ) {
  studentOrder.flavour = flavour;
  studentOrder.card = &card;
  chair.wait();

  // depending on the response resume an event at the student
  switch( studentOrder.response ) {
    case StudentOrder::Response::Free:
      _Resume Free();
      break;
    case StudentOrder::Response::Funds:
      _Resume Funds();
      break;
    case StudentOrder::Response::Stock:
      _Resume Stock();
      break;
    case StudentOrder::Response::Purchase:
      // order succeded, just return
      break;
  }

}

unsigned int* VendingMachine::inventory() { return sodaStock; }

void VendingMachine::restocked() {}

_Nomutex unsigned int VendingMachine::cost() { return sodaCost; }

_Nomutex unsigned int VendingMachine::getId() { return id; }
