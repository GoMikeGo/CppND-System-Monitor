#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int ID) {
  procID = ID;
  command = LinuxParser::Command(ID);
  userName = LinuxParser::User(ID);
  CPUpcnt = CpuUtilization();
  memSize = LinuxParser::Ram(ID);
  uptime = LinuxParser::UpTime(ID);
}

// Return this process's ID
int Process::Pid() {
  return procID; 
}

// Return this process's CPU utilization
float Process::CpuUtilization() {
  CPUpcnt = (float)(LinuxParser::ActiveJiffies(procID) / sysconf(_SC_CLK_TCK)) / (float)LinuxParser::UpTime(procID);
  return CPUpcnt; 
}

// Return the command that generated this process
string Process::Command() {
  return command.length() > 40 ? command.substr(0, 40) + "..." : command; 
}

// TODO: Return this process's memory utilization
string Process::Ram() { 
  return memSize;
}

// Return the user (name) that generated this process
string Process::User() {
  return userName; 
}

// Return the age of this process (in seconds)
long int Process::UpTime() { 
  return uptime; 
}

// Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const { 
  return this->CPUpcnt < a.CPUpcnt; 
}
