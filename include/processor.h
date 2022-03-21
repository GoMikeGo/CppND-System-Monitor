#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();  // See src/processor.cpp

 private:
  long totalJiffies;
  long idleJiffies;
};

#endif