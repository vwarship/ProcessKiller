#pragma once
#include <iostream>
#include <vector>
#include "Process.h"

class Blacklist
{
public:
	void ReadConfigureFile(const char* filename);
	void Add(const Process& process)
	{ _processes.push_back(process); }

	const std::vector<Process>& GetProcesses() const
	{ return _processes; }

	static Blacklist GetBlacklist();

	void SetProcesses(const std::vector<Process>& processes)
	{ _processes = processes; }

private:
	std::vector<Process> _processes;

};
