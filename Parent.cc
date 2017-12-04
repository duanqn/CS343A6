#include "Parent.h"
#include "MPRNG.h"
#include "Printer.h"
#include "Bank.h"
extern MPRNG g_random;

Parent::Parent(Printer &printer, Bank &bank, unsigned int numStudents, unsigned int parentalDelay):
m_printer(&printer),
m_bank(&bank),
m_numStudents(numStudents),
m_delay(parentalDelay){}

void Parent::main(){  // private
  m_printer->print(Printer::Kind::Parent, 'S'); // print: starting
  while(true){
    _Accept(~Parent){
      break;  // exit on destructor call
    }
    _Else{
      yield( m_delay );

      // generate the amount and target for a gift
      unsigned int money = g_random(1, 3);
      unsigned int student = g_random(0, m_numStudents - 1);

      // deposit the money into the bank
      m_printer->print(Printer::Kind::Parent, 'D', student, money); // print: giving money
      m_bank->deposit(student, money);
    }
  }
  m_printer->print(Printer::Kind::Parent, 'F'); // print: finished
}