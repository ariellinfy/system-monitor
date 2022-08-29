#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <vector>

#include "process.h"
#include "processor.h"

class System {
 public:
  std::string OperatingSystem();
  std::string Kernel();
  Processor& Cpu();
  float MemoryUtilization();
  int TotalProcesses();
  int RunningProcesses();
  long UpTime();
  std::vector<Process>& Processes();
  
 private:
  Processor cpu_ = {};
  std::vector<Process> processes_ = {};
};

#endif