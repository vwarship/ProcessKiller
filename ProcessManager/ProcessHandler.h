#pragma once
#include <iostream>
#include <vector>
#include "Process.h"
#include <Tlhelp32.h>

class Blacklist;

class ProcessHandler
{
public:
	ProcessHandler(void);
	~ProcessHandler(void);

	void WriteProcessesInfoToFile(char* pFilename);
	void KillProcesses(const Blacklist& blacklist);

	std::vector<Process> GetCurrentProcesses();

private:
	void InitPROCESSENTRY32(PROCESSENTRY32& processEntry);
	void EnumProcesses(HANDLE hSnapshot, std::vector<Process>& processes);
	Process GetProcess(PROCESSENTRY32& processEntry);
	void KillProcess(const Process& process);

};
