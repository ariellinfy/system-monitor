#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key, value = "unknown";
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

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string line;
  string os, version, kernel = "unknown";
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// DONE: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  string line;
  string key, value{0};
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "processes") {
          return stoi(value);
        }
      }
    }
  }
  return stoi(value);
}

// DONE: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  string line;
  string key, value{0};
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "procs_running") {
          return stoi(value);
        }
      }
    }
  }
  return stoi(value); 
}

// DONE: Read and return the system uptime
long LinuxParser::UpTime() { 
  string line;
  string uptime{0};
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime;
  }
  
  return stoi(uptime);
}

// DONE: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  string line;
  string key, value;
  float memTotal{0}, memFree{0};
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "MemTotal:") {
		  memTotal = stof(value);
        } else if (key == "MemFree:") {
          memFree = stof(value);
        }
      }
    }
  }
  return (memTotal - memFree) / memTotal; 
}

// DONE: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  string line;
  string value;
  vector<string> cpuData{};
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    while(linestream >> value) {
      if (value != "cpu") {
        cpuData.emplace_back(value);
      }
    }
  }
  return cpuData; 
}

// DONE: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
  return LinuxParser::ActiveJiffies() + LinuxParser::IdleJiffies(); 
}

// DONE: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
//   Idle = kIdle_ + kIOwait_
  vector<string> cpuData = LinuxParser::CpuUtilization();
  return stof(cpuData[kIdle_]) + stof(cpuData[kIOwait_]); 
}

// DONE: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
//   NonIdle = kUser_ + kNice_ + kSystem_ + kIRQ_ + kSoftIRQ_ + kSteal_
  vector<string> cpuData = LinuxParser::CpuUtilization();
  return stof(cpuData[kUser_]) + stof(cpuData[kNice_]) + stof(cpuData[kSystem_]) + stof(cpuData[kIRQ_]) + stof(cpuData[kSoftIRQ_]) + stof(cpuData[kSteal_]); 
}

// DONE: Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid) { 
  string line, value;
  long utime{0}, stime{0}, cutime{0}, cstime{0};
  int const utime_idx = 14;
  int const stime_idx = 15;
  int const cutime_idx = 16;
  int const cstime_idx = 17;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    for (int i = 1; i <= cstime_idx; i++) {
      linestream >> value;
      switch (i) {
        case utime_idx:
          utime = stof(value);
          break;
      case stime_idx:
          stime = stof(value);
          break;
      case cutime_idx:
          cutime = stof(value);
          break;
      case cstime_idx:
          cstime = stof(value);
          break;
      default:
      	break;
      }
    }
  }
  return utime + stime + cutime + cstime;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// DONE: Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) { 
  string line;
  string key, value = "unknown";
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
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

// DONE: Read and return the user associated with a process
string LinuxParser::User(int pid) { 
  string line;
  string user = "unknown", id, x;
  string uid = LinuxParser::Uid(pid);
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> user >> x >> id) {
        if (id == uid) {
		  return user;
        }
      }
    }
  }
  return user;
}

// DONE: Read and return the memory used by a process
string LinuxParser::Ram(int pid) { 
  string line;
  string key, value = "0";
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key;
      if (key == "VmSize:") {
        linestream >> value;
      }
    }
  }
  // convert KB to MB
  return to_string((int)(stof(value) / 1024));
}

// DONE: Read and return the uptime of a process
long LinuxParser::UpTime(int pid) { 
  string line;
  string value{0};
  int const st_idx = 22;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    for (int i = 1; i <= st_idx; i++) {
      linestream >> value;
    }
  }
  return stof(value) / sysconf(_SC_CLK_TCK);
}

// DONE: Read and return the command associated with a process
string LinuxParser::Command(int pid) { 
  string line;
  string command;
  std::ifstream stream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> command;
  }
  return command;
}