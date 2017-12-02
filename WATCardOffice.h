#pragma once 

_Monitor Printer;
_Monitor Bank;
class WATCard;

_Task WATCardOffice {
    struct Job {                              // marshalled arguments and return future
        Args args;                            // call arguments (YOU DEFINE "Args")
        WATCard::FWATCard result;             // return future
        Job( Args args ) : args( args ) {}
    };
    _Task Courier { ... };                    // communicates with bank

    void main();
    Printer *m_printer;
    Bank *m_bank;
    unsigned int m_numCourier;
    Courier *m_courierList;
  public:
    _Event Lost {};                           // lost WATCard
    WATCardOffice( Printer &prt, Bank &bank, unsigned int numCouriers );
    ~WATCardOffice();
    WATCard::FWATCard create( unsigned int sid, unsigned int amount );
    WATCard::FWATCard transfer( unsigned int sid, unsigned int amount, WATCard *card );
    Job *requestWork();
};

