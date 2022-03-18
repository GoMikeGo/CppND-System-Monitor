#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();  // See src/processor.cpp

 private:
  long activeJiffies = 0;
  long idleJiffies = 0;
};

#endif