#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

// TODO: Return the system's CPU
Processor& System::Cpu() { return this->cpu; }

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() {  
  	vector<int> pids = LinuxParser::Pids();
  	for (int pid : pids) {
      Process process;
      if (pid == process.Pid()) {
        	processes.push_back(process);
      }
}
    for (auto& process : processes) {
    	process.CpuUtilization();
  	}
  
    std::sort(processes.begin(), processes.end());
  	return this->processes; 
}

// TODO: Return the system's kernel identifier (string)
std::string System::Kernel() { 
	this->kernel = LinuxParser::Kernel();
  	return this->kernel;
}

// TODO: Return the system's memory utilization
float System::MemoryUtilization() {
	this->memory_utilization = LinuxParser::MemoryUtilization();
  	return this->memory_utilization;
}

// TODO: Return the operating system name
std::string System::OperatingSystem() {
	this->os = LinuxParser::OperatingSystem();
  	return this->os;
}

// TODO: Return the number of processes actively running on the system
int System::RunningProcesses() {
	this->running_process = LinuxParser::RunningProcesses();
  	return this->running_process;
}

// TODO: Return the total number of processes on the system
int System::TotalProcesses() { 
	this->total_process = LinuxParser::TotalProcesses();
  	return this->total_process;
}

// TODO: Return the number of seconds since the system started running
long int System::UpTime() {
	this->up_time = LinuxParser::UpTime();
  	return this->up_time;
}