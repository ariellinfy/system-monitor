#include <string>

#include "format.h"

using std::string;
using std::to_string;

// DONE: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) { 
  long h, m, s;
  string hours, mins, secs;
  
  h = seconds / 3600;
  if (h < 10) {
    hours = "0" + to_string(h);
  } else {
  	hours = to_string(h);
  }
  
  seconds = seconds % 3600;
  m = seconds / 60;
  if (m < 10) {
  	mins = "0" + to_string(m);
  } else {
  	mins = to_string(m);
  }
  
  s = seconds % 60;
  if (s < 10) {
  	secs = "0" + to_string(s);
  } else {
  	secs = to_string(s);
  }
  
  return hours + ":" + mins + ":" + secs; 
}