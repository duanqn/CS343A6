#pragma once

#include <uFuture.h>
#include <uSemaphore.h>

class WATCard{
  private:
    WATCard(const WATCard &) = delete;  // ban copy-constructor
    WATCard & operator = (const WATCard &) = delete;  // ban assign operator
    unsigned int m_money; // money in this WATCard
    uSemaphore m_mutex;
    uSemaphore m_bench; // wait for money
    int m_serialNumber;
  public:
    static int serialNumber;
    typedef Future_ISM<WATCard *> FWATCard; // future watcard pointer
    WATCard();
    ~WATCard();
    void deposit(unsigned int amount);  // add some money to WATCard
    void withdraw(unsigned int amount); // take some money out from WATCard
    unsigned int getBalance();  // get the current amount of money inside WATCard
};

