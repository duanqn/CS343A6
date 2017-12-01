#ifndef WATCARD_H_
#define WATCARD_H_
class WATCard{
  private:
  WATCard(const WATCard &) = delete;  // ban copy-constructor
  WATCard & operator = (const WATCard &) = delete;  // ban assign operator
  unsigned int m_money; // money in this WATCard
  //R-W
  public:
  typedef Future_ISM<WATCard *> FWATCard; // future watcard pointer
  WATCard();
  void deposit(unsigned int amount);  // add some money to WATCard
  void withdraw(unsigned int amount); // take some money out from WATCard
  unsigned int getBalance();  // get the current amount of money inside WATCard
};

#endif  // WATCARD_H_