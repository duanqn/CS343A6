#pragma once

_Monitor Bank {
  private:
    const unsigned int numStudents;
    unsigned int* accountBalances;
    uCondition** withdrawBenchs;

    inline void conditionalSignal( unsigned int id ) {
      if ( withdrawBenchs[id] != NULL 
         && withdrawBenchs[id]->empty() == false
         && (unsigned int)withdrawBenchs[id]->front() <= accountBalances[id] ) {
        // if there is someone waiting and there is now enough money, signal
        withdrawBenchs[id]->signal();
      }
    }

    inline void wait( unsigned int id, unsigned int amount ) {
      // wait until there is enough money to withdraw
      if ( withdrawBenchs[id] == NULL ) {
        withdrawBenchs[id] = new uCondition();
      }
      withdrawBenchs[id]->wait( amount );
    }

  public:
    Bank( unsigned int numStudents );
    ~Bank();
    void deposit( unsigned int id, unsigned int amount );
    void withdraw( unsigned int id, unsigned int amount );

};

