#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();

 private:
  long prevNonIdle{0};
  long prevIdle{0};
};

#endif