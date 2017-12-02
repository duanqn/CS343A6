#pragma once 

_Monitor Printer;
_Task NameServer;
class WATCard;

_Task VendingMachine {
 public:
  enum Flavours { BluesBlackCherry,
                  ClassicCreamSoda,
                  RockRootBeer,
                  JazzLime,
                  TotalFlavourNumber }; // flavours of soda (YOU DEFINE)
  _Event Free {}; // free, advertisement
  _Event Funds {}; // insufficient funds
  _Event Stock {}; // out of stock for particular flavour

 private:
  void main();

  typedef struct StudentOrder {
    Flavours flavour;
    WATCard* card;
    enum Response { Free, Funds, Stock, Purchase };
    Response response;
  } StudentOrder;

  Printer& printer;
  NameServer& server;
  const unsigned int id;
  const unsigned int sodaCost;
  const unsigned int maxStockPerFlavour;
  unsigned int sodaStock[4];
  uCondition chair;
  StudentOrder studentOrder;

 public:
  VendingMachine( Printer & prt, NameServer & nameServer, unsigned int id, 
    unsigned int sodaCost, unsigned int maxStockPerFlavour );
  void buy( Flavours flavour, WATCard & card );
  unsigned int * inventory();
  void restocked();
  _Nomutex unsigned int cost();
  _Nomutex unsigned int getId();

#ifdef DEBUG
  // a default ctor exclusively for unit tests to make dummy objects
  VendingMachine() :
    printer( *static_cast<Printer*>(NULL) ),
    server( *static_cast<NameServer*>(NULL) ),
    id( 0 ),
    sodaCost( 0 ),
    maxStockPerFlavour( 0 )
    {}
#endif
};

