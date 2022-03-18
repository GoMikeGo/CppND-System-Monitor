#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
   
 public:
  Process(int ID); //contructor
  
  int Pid();                               // See src/process.cpp
  std::string User();                      // See src/process.cpp
  std::string Command();                   // See src/process.cpp
  float CpuUtilization();                  // See src/process.cpp
  std::string Ram();                       // See src/process.cpp
  long int UpTime();                       // See src/process.cpp
  bool operator<(Process const& a) const;  // See src/process.cpp
  bool operator>(Process const& a) const;  // See src/process.cpp

 private:
  int procID;
  std::string userName;
  std::string command;
  float CPUpcnt;
  std::string memSize;
  long int uptime;
 };

#endif