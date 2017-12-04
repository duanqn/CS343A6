#include "Bank.h"

Bank::Bank( unsigned int numStudents )
  : numStudents( numStudents )
    {
  accountBalances = new unsigned int[numStudents];
  for ( unsigned int i = 0; i < numStudents; i += 1 ) {
    accountBalances[i] = 0;
  }

  withdrawBenchs = new uCondition*[numStudents];
  for ( unsigned int i = 0; i < numStudents; i += 1 ) {
    withdrawBenchs[i] = NULL;
  }
}

Bank::~Bank() {
  delete[] accountBalances;

  for ( unsigned int i = 0; i < numStudents; i += 1) {
    if ( withdrawBenchs[i] != NULL ) {
      delete withdrawBenchs[i];
    }
  }
  delete[] withdrawBenchs;
}

void Bank::deposit( unsigned int id, unsigned int amount ) {
  accountBalances[id] += amount;
  conditionalSignal( id );    // signal if a call is waiting
}

void Bank::withdraw( unsigned int id, unsigned int amount ) {
  if ( accountBalances[id] < amount ) {
    wait( id, amount );       // wait until the money is available
    conditionalSignal( id );  // could be more calls waiting for money
  }
  accountBalances[id] -= amount;
}
