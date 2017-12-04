#include "WATCardOffice.h"
#include "Bank.h"
#include "Printer.h"
#include <assert.h>
#include "MPRNG.h"

extern MPRNG g_random;
unsigned int WATCardOffice::Job::count = 0;

WATCardOffice::WATCardOffice( Printer &prt, Bank &bank, unsigned int numCouriers ):
m_printer(&prt),
m_bank(&bank),
m_numCourier(numCouriers){
  m_courierList = new Courier*[m_numCourier];
  for(unsigned int i = 0; i < m_numCourier; ++i){
    m_courierList[i] = new Courier(this, m_bank, m_printer, i);
  }
  Job::count = 0;
}

WATCardOffice::~WATCardOffice(){
  assert(m_jobQueue.empty()); // we shouldn't have jobs left
  for(unsigned int i = 0; i < m_numCourier; ++i){
    delete m_courierList[i];  // really need to ensure no courier is blocking anywhere
  }
  delete []m_courierList;
}

WATCard::FWATCard WATCardOffice::create( unsigned int sid, unsigned int amount ){
  Job *job = new Job(sid, amount, nullptr);
  m_jobQueue.push(job);
  m_printer->print(Printer::Kind::WATCardOffice, 'C', sid, amount);
  return job->result;
}

WATCard::FWATCard WATCardOffice::transfer( unsigned int sid, unsigned int amount, WATCard *card ){
  Job *job = new Job(sid, amount, card);
  m_jobQueue.push(job);
  m_printer->print(Printer::Kind::WATCardOffice, 'T', sid, amount);
  return job->result;
}

WATCardOffice::Job* WATCardOffice::requestWork(){
  // mutual exclusion done by Task mechanism
  if(m_jobQueue.empty()){
    return nullptr; // we cannot always block couriers on requestWork()
                    // if they block here we can't safely delete them
                    // so we might need to release them even if there is no job
  }
  Job *next = m_jobQueue.front();
  m_jobQueue.pop();
  m_printer->print(Printer::Kind::WATCardOffice, 'W');
  return next;
}

void WATCardOffice::main(){ // server, scheduling
  m_printer->print(Printer::Kind::WATCardOffice, 'S');
  while(true){
    _Accept(~WATCardOffice){
      while(!m_unemployed.empty()){
        m_unemployed.signalBlock(); // quit waiting! no more job today
      }
      break;
    } or _When(!m_jobQueue.empty()) _Accept(requestWork){
      
    } or _Accept(create, transfer){
      if(!m_unemployed.empty()){
        m_unemployed.signalBlock();
      }
    }
  }
}

void WATCardOffice::Courier::main(){
  WATCard * card = nullptr;
  m_printer->print(Printer::Kind::Courier, m_id, 'S');
  while(true){
    _Accept(~Courier){
      break;
    } _Else {
      m_currentJob = m_office->requestWork();
      if(m_currentJob == nullptr){
        break;  // no more job, exit
      }
      m_printer->print(Printer::Kind::Courier, m_id, 't', m_currentJob->studentID, m_currentJob->amount);
      // print: start funds transfer
      m_bank->withdraw(m_currentJob->studentID, m_currentJob->amount);  // may wait at bank
      if(m_currentJob->former == nullptr){  // no former card
        card = new WATCard; // create a new card
      }
      else{
        card = m_currentJob->former;
      }
      card->deposit(m_currentJob->amount);  // put in the money
      unsigned int rand = g_random(0, 5); // 1 in 6 chance to lose card
      if(rand == 0){
        // card lost
        // insert exception
        // delete card
        assert(card != nullptr);
        delete card;
        m_printer->print(Printer::Kind::Courier, m_id, 'L', m_currentJob->studentID); // print: lost card
        (m_currentJob->result).exception(new WATCardOffice::Lost);
      }
      else{
        m_printer->print(Printer::Kind::Courier, m_id, 'T', m_currentJob->studentID, m_currentJob->amount);
        // print: complete funds transfer
        assert(card != nullptr);  // otherwise, an unexpected error occurs
        (m_currentJob->result).delivery(card);
      }
      delete m_currentJob;  // couriers are responsible for deleting jobs
      m_currentJob = nullptr;
    }
  }
  m_printer->print(Printer::Kind::Courier, m_id, 'F');
}