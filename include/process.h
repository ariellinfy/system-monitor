#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int pid);
  int Pid();
  std::string User();
  float CpuUtilization();
  float cpuUsage() const;
  std::string Ram();
  long int UpTime();
  std::string Command();
  bool operator<(Process const& a) const;

 private:
  int pid_;
  float cpuUsage_;
};

#endif