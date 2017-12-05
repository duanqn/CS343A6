#pragma once

#include "WATCard.h"
#include "MPRNG.h"

_Monitor Printer;
extern MPRNG g_random;

_Task Groupoff {
    void main();
    Printer& printer;
    const unsigned int numStudents;
    const unsigned int sodaCost;
    const unsigned int groupoffDelay;
    WATCard::FWATCard* giftCards = NULL;
    WATCard::FWATCard* nextCard = NULL;
    
    inline void randomize( unsigned int seq[] ) {
      for ( unsigned int i = 0; i < numStudents; i += 1 ) {
        seq[i] = i;
      }

      // use Richard Durstenfeld's version of the Fisher–Yates shuffle
      for ( unsigned int i = numStudents - 1; i > 0; i -= 1 ) {
        int r = g_random( i );
        // swap index r and i
        int tmp = seq[i];
        seq[i] = seq[r];
        seq[r] = tmp;
      }
    }
    
  public:
    Groupoff( Printer &prt, unsigned int numStudents, unsigned int sodaCost, unsigned int groupoffDelay );
    ~Groupoff();
    WATCard::FWATCard giftCard();
};

