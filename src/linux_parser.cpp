#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>
#include <asm/param.h> 

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

bool LinuxParser::AllDigit(string &str) {
  return  std::all_of(str.begin(), str.end(), isdigit);
}

string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (AllDigit(filename)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  int total_mem, free_mem;
  string line, key;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()) {
    {
        std::getline(stream, line);
        std::istringstream linestream(line);
        linestream >> key >> total_mem;
    }
    {
        std::getline(stream, line);
        std::istringstream linestream(line);
        linestream >> key >> free_mem;
    }
  }
    
    float pcnt = (float) (total_mem - free_mem) / (float) total_mem;
    
    return pcnt; 
}

// Read and return the system uptime
long LinuxParser::UpTime() {
  string line, key;
  long t_system, t_idle;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> key >> t_system >> t_idle;
  }
  
  return t_system + t_idle; 
}

// Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
  return ActiveJiffies() + IdleJiffies();
}

// Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid) {
  string line, key;
  vector<string> procInfo(kCstime_ + 1, "");
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    int i = 1;
    while (i <= kCstime_ && linestream >> procInfo[i]) 
      i++;
    
  }
  return (AllDigit(procInfo[kUtime_]) ? stol(procInfo[kUtime_]) : 0)
       + (AllDigit(procInfo[kStime_]) ? stol(procInfo[kStime_]) : 0)
       + (AllDigit(procInfo[kCutime_]) ? stol(procInfo[kCutime_]) : 0)
       + (AllDigit(procInfo[kCstime_]) ? stol(procInfo[kCstime_]) : 0);
}

// Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  vector<string> jiffies = LinuxParser::CpuUtilization();
  return (AllDigit(jiffies[kUser_]) ? stol(jiffies[kUser_]) : 0) 
       + (AllDigit(jiffies[kNice_]) ? stol(jiffies[kNice_]) : 0)
       + (AllDigit(jiffies[kSystem_]) ? stol(jiffies[kSystem_]) : 0)
       + (AllDigit(jiffies[kIRQ_]) ? stol(jiffies[kIRQ_]) : 0)
       + (AllDigit(jiffies[kSoftIRQ_]) ? stol(jiffies[kSoftIRQ_]) : 0)
       + (AllDigit(jiffies[kSteal_]) ? stol(jiffies[kSteal_]) : 0);
}

// Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  vector<string> jiffies = LinuxParser::CpuUtilization();
  return (AllDigit(jiffies[kIdle_]) ? stol(jiffies[kIdle_]) : 0)
       + (AllDigit(jiffies[kIOwait_]) ? stol(jiffies[kIOwait_]) : 0);
}

// Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  string line, key;
  vector<string> jiffies(kGuestNice_ + 1);
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> key >> jiffies[kUser_]
                      >> jiffies[kNice_]
                      >> jiffies[kSystem_]
                      >> jiffies[kIdle_]
                      >> jiffies[kIOwait_]
                      >> jiffies[kIRQ_]
                      >> jiffies[kSoftIRQ_]
                      >> jiffies[kSteal_]
                      >> jiffies[kGuest_]
                      >> jiffies[kGuestNice_];
  }
  return jiffies; 
}

// Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  string line;
  string key;
  int value = 0;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "processes") {
          return value;
        }
      }
    }
  }
  return value;
}

// Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string line;
  string key;
  int value = 0;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "procs_running") {
          return value;
        }
      }
    }
  }
  return value;
}

// Read and return the command associated with a process
string LinuxParser::Command(int pid) { 
  string line, key;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (filestream.is_open())
    std::getline(filestream, line);
   
 return line; 
}

// Read and return the memory used by a process
string LinuxParser::Ram(int pid) { 
  string line;
  string key;
  string value = "";
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "VmSize:") {
          return std::to_string((AllDigit(value) ? stoi(value) : 0)/1000);
        }
      }
    }
  }
  return value;
}

// Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) { 
  string line;
  string key;
  string value = "";
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "Uid:") {
          return value;
        }
      }
    }
  }
  return value;
}

// Read and return the user associated with a process
string LinuxParser::User(int pid) { 
  string line;
  string key;
  string value;
  string userID = LinuxParser::Uid(pid);
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::replace(line.begin(), line.end(), 'x', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (value == userID) 
          return key;
      }
    }
  }
  return key;
}

// Read and return the uptime of a process
// IN SECONDS
long LinuxParser::UpTime(int pid) {
  string line, key;
  string value = "0";
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    int i = 1;
    while (i <= kStarttime_ && linestream >> value) {
      if (i == kStarttime_)
        return LinuxParser::UpTime() - (AllDigit(value) ? std::stol(value) : 0) / sysconf(_SC_CLK_TCK);
      i++;
    }
  }
  return std::stol(value);
}
