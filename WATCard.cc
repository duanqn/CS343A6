#include "WATCard.h"

WATCard::WATCard(): m_money(0u), m_mutex(1), m_bench(0){}

void WATCard::deposit(unsigned int amount){
  m_mutex.P();  // acquire lock
  m_money += amount;
  if(!m_bench.empty()){
    m_bench.V();
    // do not V on mutex here
    // do a baton passing
  }
  else{
    m_mutex.V();
  }
}

void WATCard::withdraw(unsigned int amount){
  m_mutex.P();  // acquire lock
  while(m_money < amount){
    m_mutex.V();
    m_bench.P();  // wait on bench
    m_mutex.P();
  }
  assert(m_money >= amount);
  m_money -= amount;
  m_mutex.V();
}

unsigned int WATCard::getBalance(){
  m_mutex.P();
  unsigned int result = m_money;
  m_mutex.V();
  return result;
}
