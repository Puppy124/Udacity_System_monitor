#include "processor.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
	float total = (float)LinuxParser::Jiffies();
  	float active = (float)LinuxParser::ActiveJiffies();
  	this->utilization = (active/total) * 100;
  	return this->utilization;
}