#include "processor.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
	float idle = (float)LinuxParser::IdleJiffies();
  	float active = (float)LinuxParser::ActiveJiffies();
  	float duration = active + idle;
  	this->utilization = (active/duration);
  	return this->utilization;
}