#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <unistd.h>
#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
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
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
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

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  float total_mem = 0, free_mem = 0;
  string line;
  string key;
  float value;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
  	while(std::getline(filestream, line)) {
    	std::istringstream stream(line);
     	while(stream >> key >> value) {
         	if(key == "MemTotal:") {
             	total_mem = value; 
            }
         	else if(key == "MemFree:") {
             	free_mem = value; 
            }      
        }
    }
  }
  // Assume that we don't care about the buffer and other memories
  float used_mem = total_mem - free_mem;
  float utilize_percentage = (used_mem/total_mem) * 100;
  return utilize_percentage; 
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { 
  long up_time = 0;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> up_time;
  }  
  return up_time; 
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
  // vector<string> cpuUtilization = LinuxParser::CpuUtilization();
  long totalJiffies = 0;
  // for (auto field : cpuUtilization) {
  //   totalJiffies += std::stol(field);
  // }
  totalJiffies = LinuxParser::UpTime() * sysconf(_SC_CLK_TCK); 
  return totalJiffies;
}

// TODO: Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid) { 
  vector<string> values{};
  long utime, stime, cutime, cstime, activeJiffies;
  string line, field;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    while (linestream >> field) {
    	values.push_back(field);
        }
    }
  
   utime = std::stol(values[13]);
   stime = std::stol(values[14]);
   cutime = std::stol(values[15]);
   cstime = std::stol(values[16]);
   activeJiffies = utime + stime + cutime + cstime;
   long active_time_sec = activeJiffies / sysconf(_SC_CLK_TCK);
  return active_time_sec; 
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
  std::vector<std::string> cpuUtilization = LinuxParser::CpuUtilization();
  long activeJiffies = std::stol(cpuUtilization[LinuxParser::kUser_]) +\
                       std::stol(cpuUtilization[LinuxParser::kNice_]) +\
                       std::stol(cpuUtilization[LinuxParser::kSystem_]) +\
                       std::stol(cpuUtilization[LinuxParser::kIRQ_]) +\
                       std::stol(cpuUtilization[LinuxParser::kSoftIRQ_]) +\
                       std::stol(cpuUtilization[LinuxParser::kSteal_]);
  return activeJiffies;
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  vector<string> cpuUtilization = LinuxParser::CpuUtilization();
  long idleJiffies = std::stol(cpuUtilization[LinuxParser::kIdle_]) +
                     std::stol(cpuUtilization[LinuxParser::kIOwait_]);

  return idleJiffies;
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  vector<string> cpuUtilization;
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    string cpu_id;
    while (linestream >> cpu_id) {
      cpuUtilization.push_back(cpu_id);
    }
  }
  return cpuUtilization;
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  string line;
  string key;
  int value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "processes") {
          return value;
        }
      }
    }
  }
  return 0; 
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  string line;
  string key;
  int value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "procs_running") {
          return value;
        }
      }
    }
  } 
  return 0; 
}

// TODO: Read and return the command associated with a process
string LinuxParser::Command(int pid) { 
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  string command;
  if (stream.is_open()) {
    std::string line;
    std::getline(stream, line);      
    	command = line;
	}  
  return command;
}

// TODO: Read and return the memory used by a process
string LinuxParser::Ram(int pid) { 
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  string ram;
  if (stream.is_open()) {
    string line, key, value;
    while(std::getline(stream, line)) {
      std::istringstream input_stream(line);
      while(input_stream >> key >> value) {
         	if (key == "VmSize:") {
             	ram = value;
          }
      }
    }
  }
  return ram;  
}

// TODO: Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) { 
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  string uid{"0"};
  if (stream.is_open()) {
    string line, key, value;
    while(std::getline(stream, line)) {
      std::istringstream input_stream(line);
      while(input_stream >> key >> value) {
         	if (key == "Uid:") {
             	uid = value;
          }
      }
    }
  }
  return uid;  
}

// TODO: Read and return the user associated with a process
string LinuxParser::User(int pid) { 
    string line, username, field;
    std::ifstream filestream(kProcDirectory + std::to_string(pid) + kPasswordPath);
    if (filestream.is_open()) {
      while (getline(filestream, line)) {
        std::istringstream linestream(line);
        // Split the line by colons
        while (linestream >> field) {
          // The username is the first field
          if (field == std::to_string(pid)) {
            // The next field is the username
            username = field;
            return username;
          }
        }
      }
    }
    return string(); 
}

// TODO: Read and return the uptime of a process
long LinuxParser::UpTime(int pid) { 
  vector<string> values;
  long up_time;
  string line, field;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    while (linestream >> field) {
    	values.push_back(field);
    }
  }
  up_time = std::stol(values[13]);
  long up_time_sec = up_time / sysconf(_SC_CLK_TCK);
  return up_time_sec; 
}
