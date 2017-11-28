#ifndef PARENT_H_
#define PARENT_H_

_Task Parent{
  private:
  void main();
  Printer *m_printer; // handle to printer
  Bank *m_bank;       // handle to bank
  unsigned int m_numStudents; // number of students
  unsigned int m_delay; // yield m_delay times before sending a gift
  
  public:
  Parent(Printer &printer, Bank &bank, unsigned int numStudents, unsigned int parentalDelay);
};

#endif  // PARENT_H_