#pragma once

#include <queue>
#include "WATCard.h"


_Monitor Printer;
_Monitor Bank;
class WATCard;

_Task WATCardOffice {
  private:
    struct Job {                              // marshalled arguments and return future
      static unsigned int count;
      unsigned int id;    // for the convenience of debugging
      unsigned int studentID;
      unsigned int amount;
      WATCard * former; // pass in nullptr for create()
      WATCard::FWATCard result;             // return future
      Job( unsigned int _sid, unsigned int _amount, WATCard *_card = nullptr ):
        studentID(sid), amount(_amount), former(_card){
        id = count++;
      }
    };
    _Task Courier { // nested
      private:
        Job *m_currentJob;  // couriers are responsible for deleting jobs
        WATCardOffice *m_office;  // registration purpose
        Bank *m_bank; // registration purpose
        Printer *m_printer;
        unsigned int m_id;
        void main();
      public:
        Courier(WATCardOffice *office, Bank *bank, Printer *printer, unsigned int id):
          m_currentJob(nullptr), m_office(office), m_bank(bank), m_printer(printer), m_id(id){}
        ~Courier(){
          if(currentJob != nullptr){
            delete currentJob;
          }
        }
    };                    // communicates with bank
    
    
    Printer *m_printer;
    Bank *m_bank;
    unsigned int m_numCourier;
    Courier **m_courierList;
    std::queue<Job *> m_jobQueue; // stores pointers to jobs
    
    uCondition m_unemployed;
    void main();
  public:
    _Event Lost {};                           // lost WATCard
    WATCardOffice( Printer &prt, Bank &bank, unsigned int numCouriers );
    ~WATCardOffice();
    WATCard::FWATCard create( unsigned int sid, unsigned int amount );
    WATCard::FWATCard transfer( unsigned int sid, unsigned int amount, WATCard *card );
    Job *requestWork();
};

