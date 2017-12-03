#pragma once

#include "Printer.h"
#include "NameServer.h"
#include "WATCardOffice.h"
#include "Groupoff.h"

_Monitor Printer;
_Task NameServer;
_Task WATCardOffice;
_Task Groupoff;

_Task Student {
  private:
    static const int WATCARD_INIT_BALANCE = 5;
    static const int MIN_PURCHASE = 1;
    Printer *m_printer;
    NameServer *m_server;
    WATCardOffice *m_office;
    Groupoff *m_groupoff;
    unsigned int m_id;
    unsigned int m_maxPurchases;
    void main();
  public:
    Student( Printer &prt, NameServer &nameServer, WATCardOffice &cardOffice, Groupoff &groupoff,
             unsigned int id, unsigned int maxPurchases );

#ifdef DEBUG
  // a default ctor exclusively for unit tests to make dummy objects
  Student() {}
#endif
};

