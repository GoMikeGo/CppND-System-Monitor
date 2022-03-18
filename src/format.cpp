#include <string>

#include "format.h"

using std::string;

// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) { 
  long hh, mm, ss;
  string HH, MM, SS;
  hh = seconds / 3600;
  HH = hh > 9 ? std::to_string(hh) : "0" + std::to_string(hh);
  seconds = seconds % 3600;
  mm = seconds / 60;
  MM = mm > 9 ? std::to_string(mm) : "0" + std::to_string(mm);
  ss = seconds % 60;
  SS = ss > 9 ? std::to_string(ss) : "0" + std::to_string(ss);
 
  return HH + ":" + MM + ":" + SS; 
}