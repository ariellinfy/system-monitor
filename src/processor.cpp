#include "processor.h"
#include "linux_parser.h"

// DONE: Return the aggregate CPU utilization
// Calcuation reference: https://stackoverflow.com/questions/23367857/accurate-calculation-of-cpu-usage-given-in-percentage-in-linux
float Processor::Utilization() { 
  float utilization{0};
  
  long nonIdle = LinuxParser::ActiveJiffies();
  long idle = LinuxParser::IdleJiffies();
  
  long prevTotal{prevNonIdle + prevIdle};
  long total{nonIdle + idle};
  
  long totalDuration{total - prevTotal};
  long idleDuration{idle - prevIdle};
  
  utilization = static_cast<float>(totalDuration - idleDuration) / totalDuration;
  
  prevNonIdle = nonIdle;
  prevIdle = idle;
  
  return utilization; 
}