#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::string;
using std::vector;


// DONE: Return the operating system name
string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

// DONE: Return the system's kernel identifier (string)
string System::Kernel() { return LinuxParser::Kernel(); }

// DONE: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// DONE: Return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// DONE: Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// DONE: Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// DONE: Return the number of seconds since the system started running
long int System::UpTime() { return LinuxParser::UpTime(); }

// DONE: Return a container composed of the system's processes
vector<Process>& System::Processes() { 
  processes_.clear();

  for (auto pid : LinuxParser::Pids()) {
  	Process process(pid);
    process.CpuUtilization();
    processes_.emplace_back(process);
  }
  
  std::sort(processes_.begin(), processes_.end());  
  return processes_; 
}