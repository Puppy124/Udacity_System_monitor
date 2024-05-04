#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid) : process_id(pid) {}

// TODO: Return this process's ID
int Process::Pid() { 
  return this->process_id;;
}

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() {
  long active_jiffies = LinuxParser::ActiveJiffies(process_id);
  long uptime = LinuxParser::ActiveJiffies();
  this->cpuUtilization = (float)(active_jiffies / uptime);
//   cached_active_tick = active_jiffies;
//   cached_ystem_tick = uptime;
  return this->cpuUtilization;
}

// TODO: Return the command that generated this process
string Process::Command() {
	this->command = LinuxParser::Command(process_id);
  	return this->command;
}

// TODO: Return this process's memory utilization
string Process::Ram() {
	this->ram = LinuxParser::Ram(process_id);
  	return this->ram;
}

// TODO: Return the user (name) that generated this process
string Process::User() { 
  	this->user = LinuxParser::User(process_id);
  	return this->user;
}

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() {
	this->up_time = LinuxParser::UpTime(process_id);
  	return this->up_time;
}

// TODO: Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& other_process) const { 
  	return process_id < other_process.process_id; 
}