#ifndef SYSTEM_PARSER_H
#define SYSTEM_PARSER_H

#include <fstream>
#include <regex>
#include <string>

namespace LinuxParser {
// Paths
const std::string kOSPath{"/etc/os-release"};
const std::string kProcDirectory{"/proc/"};
const std::string kVersionFilename{"/version"};
const std::string kStatFilename{"/stat"};
const std::string kUptimeFilename{"/uptime"};
const std::string kMeminfoFilename{"/meminfo"};
const std::string kStatusFilename{"/status"};
const std::string kPasswordPath{"/etc/passwd"};
const std::string kCmdlineFilename{"/cmdline"};
const std::string kCpuinfoFilename{"/cpuinfo"};

// System
std::string OperatingSystem();
std::string Kernel();
int TotalProcesses();
int RunningProcesses();  
long UpTime();
float MemoryUtilization();

// CPU
enum CPUStates {
  kUser_ = 0,
  kNice_,
  kSystem_,
  kIdle_,
  kIOwait_,
  kIRQ_,
  kSoftIRQ_,
  kSteal_,
  kGuest_,
  kGuestNice_
};
std::vector<std::string> CpuUtilization();
long Jiffies();
long ActiveJiffies();
long ActiveJiffies(int pid);
long IdleJiffies();

// Processes
std::vector<int> Pids();
std::string Uid(int pid);
std::string User(int pid);
std::string Ram(int pid);
long int UpTime(int pid);  
std::string Command(int pid);

};  // namespace LinuxParser

#endif