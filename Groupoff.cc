#include "Groupoff.h"
#include "Printer.h"

#include <uFuture.h>

Groupoff::Groupoff( Printer &prt, unsigned int numStudents, 
  unsigned int sodaCost, unsigned int groupoffDelay )
  : printer( prt ), numStudents( numStudents),
    sodaCost( sodaCost ), groupoffDelay( groupoffDelay )
    {
  giftCards = new WATCard::FWATCard[numStudents];
}

Groupoff::~Groupoff() {
  for ( unsigned int i = 0; i < numStudents; i += 1 ) {
    if ( giftCards[i].cancelled() )
      giftCards[i].reset();
    if ( giftCards[i].available() )
      delete giftCards[i];
  }
  delete[] giftCards;
}

void Groupoff::main() {
  printer.print( Printer::Kind::Groupoff, 'S' );

  // generate a random sequence of numbers for handing out cards
  unsigned int sequence[numStudents];
  randomize( sequence );

  // accept a call from each student to obtain a future giftCard
  for ( nextCard = giftCards; 
        nextCard != giftCards + numStudents; 
        nextCard += 1 ) {
    _Accept( giftCard )
  }

  // iterate numStudents times giving out cards 
  STUDS: for ( unsigned int i = 0; i < numStudents; i += 1 ) {
    yield( groupoffDelay );

    WATCard* wc = new WATCard();
    wc->deposit( sodaCost );
    printer.print( Printer::Kind::Groupoff, 'D', sodaCost );
    giftCards[ sequence[i] ].delivery( wc );
  
    // check if there has been a dtor call
    _Accept( ~Groupoff ) {
      break STUDS;
    }
    _Else {}

  } // for

  printer.print( Printer::Kind::Groupoff, 'F' );
}

WATCard::FWATCard Groupoff::giftCard() {
  return *nextCard;
}
