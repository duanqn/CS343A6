#include "WATCardOffice.h"

WATCardOffice::WATCardOffice( Printer &prt, Bank &bank, unsigned int numCouriers ):
m_printer(&prt),
m_bank(&bank),
m_numCourier(numCouriers){
  m_courierList = new Courier[m_numCourier];
}

WATCardOffice::~WATCardOffice(){
  delete []m_courierList;
}

WATCardOffice::create( unsigned int sid, unsigned int amount ){

}