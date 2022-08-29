#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;

Process::Process(int pid) : pid_(pid) {}

// DONE: Return this process's ID
int Process::Pid() { return pid_; }

// DONE: Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(Process::Pid()); }

// DONE: Return this process's CPU utilization
float Process::CpuUtilization() { 
  float totalTime = static_cast<float> (LinuxParser::ActiveJiffies(Process::Pid()) / sysconf(_SC_CLK_TCK));
  float seconds = LinuxParser::UpTime() - LinuxParser::UpTime(Process::Pid());
  cpuUsage_ =  totalTime / seconds;
  return cpuUsage_;
}

float Process::cpuUsage() const { 
  return cpuUsage_;
}

// DONE: Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(Process::Pid()).substr(0, 7); }

// DONE: Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime() - LinuxParser::UpTime(Process::Pid()); }

// DONE: Return the command that generated this process
string Process::Command() { return LinuxParser::Command(Process::Pid()); }

// DONE: Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const { 
  return a.Process::cpuUsage() < this->Process::cpuUsage(); 
}