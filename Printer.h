#ifndef PRINTER_H_
#define PRINTER_H_
#include <cstring>
#include <assert.h>
#include <iostream>

_Monitor Printer{
  private:
  unsigned int m_numStudents;
  unsigned int m_numMachines;
  unsigned int m_numCouriers;
  unsigned int m_totalColumns;
  struct Info{
    bool isValid;
    char label;
    char args;  // use char to save memory
                // valid values: 0,1,2
    int data1;
    int data2;
    
    Info(){
      reset();
    }
    void reset(){
      isValid = false;
      label = '\0';
      args = 0;
      data1 = 0;
      data2 = 0;
    }
    void output(){
      assert(isValid == true);  // must be valid
      switch(args){
        case 2:
          std::cout << label << data1 << "," << data2;
          break;
        case 1:
          std::cout << label << data1;
          break;
        case 0:
          std::cout << label;
          break;
        default:
          assert(false);  // UNEXPECTED ERROR
      }
    }
  };
  
  void flush(); // print a line
  
  public:
  enum Kind{Parent = 0, Groupoff, WATCardOffice, NameServer, Truck, BottlingPlant, Student, Vending, Courier, TotalKinds};
  
  private:
  // some routines and variables need definition of Kind
  // the string to print for each column
  std::string m_colNames[Kind::TotalKinds];
  int m_startingPos[Kind::TotalKinds];  // the column of the 1st element of each kind
  Info *m_slots;
  int positionLookup(Kind type, unsigned int localID); // look up the column
  
  public:
  Printer(unsigned int numStudents, unsigned int numVendingMachines, unsigned int numCouriers);
  ~Printer();
  void print(Kind kind, char state);
  void print(Kind kind, char state, int value1);
  void print(Kind kind, char state, int value1, int value2);
  void print(Kind kind, unsigned int localID, char state);
  void print(Kind kind, unsigned int localID, char state, int value1);
  void print(Kind kind, unsigned int localID, char state, int value1, int value2);
};
#endif  // PRINTER_H_
