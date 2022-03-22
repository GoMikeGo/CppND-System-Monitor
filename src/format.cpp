#include <string>

#include "format.h"

using std::string;

// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) { 
  string HH, MM, SS;
  HH = std::to_string(seconds / 3600);
  HH.insert(0, 2 - HH.length(), '0');
  seconds = seconds % 3600;
  MM = std::to_string(seconds / 60);
  MM.insert(0, 2 - MM.length(), '0');
  SS = std::to_string(seconds % 60);
  SS.insert(0, 2 - SS.length(), '0');

  return HH + ":" + MM + ":" + SS; 
}