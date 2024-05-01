#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"

using std::string;
using std::to_string;
using std::vector;

// TODO: Return this process's ID
int Process::Pid() { 
	vector<int> pids = LinuxParser::Pids();
    int current_pid = getpid();
    for (int pid : pids) {
    if (pid == current_pid) {
      this->process_id = pid;
      return process_id;
    }
  }
  return -1;
}

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() {
  long active_jiffies = LinuxParser::ActiveJiffies(Pid());
  long uptime = LinuxParser::UpTime(Pid());
  float cpu_utilization = (float)(active_jiffies) / uptime;
  this->cpuUtilization = cpu_utilization;
  return cpuUtilization;
}

// TODO: Return the command that generated this process
string Process::Command() {
	this->command = LinuxParser::Command(process_id);
  	return this->command;
}

// TODO: Return this process's memory utilization
string Process::Ram() {
	this->ram = LinuxParser::Command(process_id);
  	return this->ram;
}

// TODO: Return the user (name) that generated this process
string Process::User() { 
  	this->user = LinuxParser::Uid(process_id);
  	return this->user;
}

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() {
	this->up_time = LinuxParser::UpTime(process_id);
  	return this->up_time;
}

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& other_process) const { 
  	return process_id < other_process.process_id; 
}