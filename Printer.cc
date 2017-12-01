#include "Printer.h"

Printer::Printer(unsigned int numStudents, unsigned int numVendingMachines, unsigned int numCouriers):
  m_numStudents(numStudents),
  m_numMachines(numVendingMachines),
  m_numCouriers(numCouriers),
  m_totalColumns(6 + numStudents + numVendingMachines + numCouriers){
  // set up column position lookup table
  int pos = 0;
  m_startingPos[Printer::Kind::Parent] = pos++;
  m_startingPos[Printer::Kind::Groupoff] = pos++;
  m_startingPos[Printer::Kind::WATCardOffice] = pos++;
  m_startingPos[Printer::Kind::NameServer] = pos++;
  m_startingPos[Printer::Kind::Truck] = pos++;
  m_startingPos[Printer::Kind::BottlingPlant] = pos++;
  m_startingPos[Printer::Kind::Student] = pos;
  pos += m_numStudents;
  m_startingPos[Printer::Kind::Vending] = pos;
  pos += m_numMachines;
  m_startingPos[Printer::Kind::Courier] = pos;  // no longer use pos below this line
  // set up column name(prefix) lookup table
  m_colNames[Printer::Kind::Parent] = "Parent";
  m_colNames[Printer::Kind::Groupoff] = "Gropoff";
  m_colNames[Printer::Kind::WATCardOffice] = "WATOff";
  m_colNames[Printer::Kind::NameServer] = "Names";
  m_colNames[Printer::Kind::Truck] = "Truck";
  m_colNames[Printer::Kind::BottlingPlant] = "Plant";
  m_colNames[Printer::Kind::Student] = "Stud";
  m_colNames[Printer::Kind::Vending] = "Mach";
  m_colNames[Printer::Kind::Courier] = "Cour";
  // allocate memory for slots
  m_slots = new Info[m_totalColumns];
  // print header
  for(int i = 0; i < 6; ++i){
    std::cout << m_colNames[i] << "\t";
  }
  for(unsigned int i = 0; i < m_numStudents; ++i){
    std::cout << m_colNames[Printer::Kind::Student] << i << "\t";
  }
  for(unsigned int i = 0; i < m_numMachines; ++i){
    std::cout << m_colNames[Printer::Kind::Vending] << i << "\t";
  }
  for(unsigned int i = 0; i < m_numCouriers; ++i){
    std::cout << m_colNames[Printer::Kind::Courier] << i;
    if(i < m_numCouriers - 1){
      std::cout << "\t";  // eliminate trailing spaces
    }
  }
  std::cout << std::endl;
  for(unsigned int i = 0; i < m_totalColumns; ++i){
    std::cout << "*******"; // 7 asterisks
    if(i < m_totalColumns - 1){
      std::cout << "\t";      // eliminate trailing spaces
    }
  }
  std::cout << std::endl;
}

Printer::~Printer(){
  for(int i = 0; i < 23; ++i){
    std::cout << "*"; // use a loop here to explicitly indicate the number of asterisks
  }
  std::cout << std::endl;
  delete []m_slots;
}

int Printer::positionLookup(Kind type, int localID = 0){  // private
  switch(type){
    case Printer::Kind::Student:
      return m_startingPos[Printer::Kind::Student] + localID;
      // returned
    case Printer::Kind::Vending:
      return m_startingPos[Printer::Kind::Vending] + localID;
      // returned
    case Printer::Kind::Courier:
      return m_startingPos[Printer::Kind::Courier] + localID;
      //returned
    default:
      return m_startingPos[type];
  }
}

void Printer::flush(){  // private
  int lastValid = -1;
  for(unsigned int i = 0; i < m_totalColumns; ++i){
    if(m_slots[i].isValid == true){
      lastValid = (int)i;
    }
  }
  assert(lastValid >= 0);
  for(unsigned int i = 0; i < m_totalColumns; ++i){
    if(i > 0 && i <= lastValid){
      std::cout << "\t";
    }
    if(m_slots[i].isValid == true){
      m_slots[i].output();
    }
    m_slots[i].reset();
  }
  std::cout << std::endl;
}

void Printer::print(Printer::Kind kind, char state){
  // Possible callers: Parent, Groupoff, WATCardOffice, NameServer, Truck, BottlingPlant
  assert(kind == Printer::Kind::Parent
          || kind == Printer::Kind::Groupoff
          || kind == Printer::Kind::WATCardOffice
          || kind == Printer::Kind::NameServer
          || kind == Printer::Kind::Truck
          || kind == Printer::Kind::BottlingPlant);
  int pos = positionLookup(kind);
  if(m_slots[pos].isValid == true){  // already occupied
    flush();
  }
  m_slots[pos].isValid = true;
  m_slots[pos].label = state;
  m_slots[pos].args = 0;
}

void Printer::print(Printer::Kind kind, char state, int value1){
  // Possible callers: Parent, Groupoff, WATCardOffice, NameServer, Truck, BottlingPlant
  assert(kind == Printer::Kind::Parent
          || kind == Printer::Kind::Groupoff
          || kind == Printer::Kind::WATCardOffice
          || kind == Printer::Kind::NameServer
          || kind == Printer::Kind::Truck
          || kind == Printer::Kind::BottlingPlant);
  int pos = positionLookup(kind);
  if(m_slots[pos].isValid == true){  // already occupied
    flush();
  }
  m_slots[pos].isValid = true;
  m_slots[pos].label = state;
  m_slots[pos].args = 1;
  m_slots[pos].data1 = value1;
}

void Printer::print(Printer::Kind kind, char state, int value1, int value2){
  // Possible callers: Parent, Groupoff, WATCardOffice, NameServer, Truck, BottlingPlant
  assert(kind == Printer::Kind::Parent
          || kind == Printer::Kind::Groupoff
          || kind == Printer::Kind::WATCardOffice
          || kind == Printer::Kind::NameServer
          || kind == Printer::Kind::Truck
          || kind == Printer::Kind::BottlingPlant);
  int pos = positionLookup(kind);
  if(m_slots[pos].isValid == true){  // already occupied
    flush();
  }
  m_slots[pos].isValid = true;
  m_slots[pos].label = state;
  m_slots[pos].args = 1;
  m_slots[pos].data1 = value1;
  m_slots[pos].args = 2;
  m_slots[pos].data2 = value2;
}

void Printer::print(Kind kind, unsigned int localID, char state){
  // Possible callers: Student, Vending, Courier
  assert(kind == Printer::Kind::Student
          || kind == Printer::Kind::Vending
          || kind == Printer::Kind::Courier);
  int pos = positionLookup(kind, localID);
  if(m_slots[pos].isValid = true){
    flush();
  }
  m_slots[pos].isValid = true;
  m_slots[pos].label = state;
  m_slots[pos].args = 0;
}

void Printer::print(Kind kind, unsigned int localID, char state, int value1){
  // Possible callers: Student, Vending, Courier
  assert(kind == Printer::Kind::Student
          || kind == Printer::Kind::Vending
          || kind == Printer::Kind::Courier);
  int pos = positionLookup(kind, localID);
  if(m_slots[pos].isValid = true){
    flush();
  }
  m_slots[pos].isValid = true;
  m_slots[pos].label = state;
  m_slots[pos].args = 1;
  m_slots[pos].data1 = value1;
}

void Printer::print(Kind kind, unsigned int localID, char state, int value1, int value2){
  // Possible callers: Student, Vending, Courier
  assert(kind == Printer::Kind::Student
          || kind == Printer::Kind::Vending
          || kind == Printer::Kind::Courier);
  int pos = positionLookup(kind, localID);
  if(m_slots[pos].isValid = true){
    flush();
  }
  m_slots[pos].isValid = true;
  m_slots[pos].label = state;
  m_slots[pos].args = 1;
  m_slots[pos].data1 = value1;
  m_slots[pos].args = 2;
  m_slots[pos].data2 = value2;
}