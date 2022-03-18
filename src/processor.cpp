#include "processor.h"
#include "linux_parser.h"
#include <vector>
#include <string>

// Return the real time CPU utilization

float Processor::Utilization() { 
  long active = LinuxParser::ActiveJiffies();
  long idle = LinuxParser::IdleJiffies();
  float pcnt = float(active - activeJiffies) / float(active + idle - activeJiffies - idleJiffies) * 100.0;
  
  idleJiffies = idle;
  activeJiffies = active;

  return pcnt;
}