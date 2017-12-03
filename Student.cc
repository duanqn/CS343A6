#include "Student.h"
#include "MPRNG.h"
#include "config.h"
#include "VendingMachine.h"
#include <assert.h>

extern MPRNG g_random;

Student::Student(Printer &prt, NameServer &nameServer, WATCardOffice &cardOffice, Groupoff &groupoff, unsigned int id, unsigned int maxPurchases):
  m_printer(&prt),
  m_server(&nameServer),
  m_office(&cardOffice),
  m_groupoff(&groupoff),
  m_id(id),
  m_maxPurchases(maxPurchases){}

void Student::main(){  // private
  int numPurchase = g_random(Student::MIN_PURCHASE, m_maxPurchases);  // [1, m_maxPurchases]
  VendingMachine::Flavours favFlavour = static_cast<VendingMachine::Flavours>(g_random(0, 3)); // favourite flavour
  // TODO: replace 3 with VendingMachine::Flavours::TotalFlavourNumber
  m_printer->print(Printer::Kind::Student, m_id, 'S', static_cast<int>(favFlavour), numPurchase);

  WATCard::FWATCard myWatCardPtr = m_office->create(m_id, Student::WATCARD_INIT_BALANCE);
  // use try block on every access to *myWatCardPtr(), as a WATCardOffice::Lost exception may occur
  WATCard::FWATCard myGiftCardPtr = m_groupoff->giftCard();
  // reset this gift card after use
  VendingMachine * myMachine = m_server->getMachine(m_id);
  m_printer->print(Printer::Kind::Student, m_id, 'V', (int)(myMachine->getId()));
  for(int purchase = 0; purchase < numPurchase; ++purchase){
    // before buying each soda, a student yield random times in [1, 10]
    int yield_times = g_random(1, 10);
    yield(yield_times);

    bool purchaseFailed = false;
    char state = '\0';  // store the char used for output
    WATCard* availableCardPtr;
    // make a purchase
    do{
      purchaseFailed = false;
      try{
        // wait until either card is available
        // place gift card in the front
        _Select(myGiftCardPtr){
          availableCardPtr = myGiftCardPtr();
          state = 'G';
        }
        or _Select(myWatCardPtr){
          availableCardPtr = myWatCardPtr();
          state = 'B';
        }
        myMachine->buy(favFlavour, *availableCardPtr);
        m_printer->print(Printer::Kind::Student, state, static_cast<int>(favFlavour), availableCardPtr->getBalance());
        if(state == 'G'){
          // purchased with gift card
          delete myGiftCardPtr();
          myGiftCardPtr.reset();
        }
      } // try
      _CatchResume(VendingMachine::Free &e){
        // purchase is successful
        // change the state character
        if(state == 'G'){
          state = 'a';
        }
        else if(state == 'B'){
          state = 'A';
        }
        else{
          assert(false);  // state illegal, fail fast
        }
        // yield 4 times
        yield(4);
      }
      catch(WATCardOffice::Lost &e){
        // request another future WatCard
        // redo select
        // do not yield

        // print message: WatCard lost
        m_printer->print(Printer::Kind::Student, 'L');
        purchaseFailed = true;
      }
      catch(VendingMachine::Funds &e){
        // do transfer (change myWatCardPtr)
        // redo select
        // do not yield
        purchaseFailed = true;
      }
      catch(VendingMachine::Stock &e){
        // get another machine (change myMachine)
        // redo select (should be instantly)
        // do not yield
        // print message: select machine
        purchaseFailed = true;
      }
    }while(purchaseFailed);
  }


  // delete WatCard
  if(myWatCardPtr.available()){
    try{
      availableCardPtr = myWatCardPtr();
      assert(availableCardPtr != nullptr);
      delete availableCardPtr;
      availableCardPtr = nullptr;
    }
    catch(WATCardOffice::Lost){
      // we are just cleaning, we don't need to apply for a new card
      // but we must deal with this exception
      // to prevent deleting a card twice
      // lost cards are deleted by WATCardOffice
      // still may print a message for the incident
      m_printer->print(Printer::Kind::Student, 'L');
    }
  }
  // delete GiftCard
  if(myGiftCardPtr.available()){
    availableCardPtr = myGiftCardPtr();
    assert(availableCardPtr != nullptr);
    delete availableCardPtr;
    availableCardPtr = nullptr;
  }
  m_printer->print(Printer::Kind::Student, 'F');
}
