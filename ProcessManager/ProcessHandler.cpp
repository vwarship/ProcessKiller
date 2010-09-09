#include "StdAfx.h"
#include "ProcessHandler.h"
#include <vector>
#include <Psapi.h>
#include <AtlBase.h> 
#include <AtlConv.h>
#include "FileHandler.h"
#include "Blacklist.h"

#pragma comment(lib, "psapi.lib")

ProcessHandler::ProcessHandler(void)
{
}

ProcessHandler::~ProcessHandler(void)
{
}

void ProcessHandler::WriteProcessesInfoToFile(char* pFilename)
{
	std::vector<Process> processes = GetCurrentProcesses();
	FileHandler fh;
	fh.Open(pFilename, FileHandler::FILE_WRITE);
	std::vector<Process>::iterator iter = processes.begin();
	for (; iter != processes.end(); ++iter)
	{
		fh.Write(*iter);
	}
	fh.Close();
}

std::vector<Process> ProcessHandler::GetCurrentProcesses()
{
	std::vector<Process> processes;

	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnapshot != INVALID_HANDLE_VALUE)
	{
		EnumProcesses(hSnapshot, processes);

		//CloseToolhelp32Snapshot(hSnapshot);
		CloseHandle(hSnapshot);
	}

	return processes;
}

void ProcessHandler::InitPROCESSENTRY32(PROCESSENTRY32& processEntry)
{
	ZeroMemory(&processEntry, sizeof(PROCESSENTRY32));
	processEntry.dwSize = sizeof(PROCESSENTRY32);
}

void ProcessHandler::EnumProcesses(HANDLE hSnapshot, std::vector<Process>& processes)
{
	PROCESSENTRY32 processEntry;
	InitPROCESSENTRY32(processEntry);

	if (Process32First(hSnapshot, &processEntry))
	{
		Process process = GetProcess(processEntry);
		processes.push_back(process);

		InitPROCESSENTRY32(processEntry);

		while (Process32Next(hSnapshot, &processEntry))
		{
			process = GetProcess(processEntry);
			processes.push_back(process);

			InitPROCESSENTRY32(processEntry);
		}
	}
}

Process ProcessHandler::GetProcess(PROCESSENTRY32& processEntry)
{
	Process process(processEntry.th32ProcessID);

	USES_CONVERSION;
	process.ExeFile(T2A(processEntry.szExeFile));

	HANDLE hProcess = OpenProcess(
		PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,
		FALSE, processEntry.th32ProcessID );

	TCHAR pFilename[MAX_PATH];
	if (hProcess)
	{
		if (GetModuleFileNameEx(hProcess, NULL, pFilename, MAX_PATH))
		{
			//printf("<%ls> %ls \n", processEntry.szExeFile, pFilename);
			process.Filename(T2A(pFilename));
		}
		CloseHandle(hProcess);
	}

	return process;
}

void ProcessHandler::KillProcesses(const Blacklist& blacklist)
{
	std::vector<Process> processes = blacklist.GetProcesses();

	for (std::vector<Process>::iterator iter = processes.begin();
		 iter != processes.end(); 
		 ++iter)
	{
		KillProcess(*iter);
	}
}

void ProcessHandler::KillProcess(const Process& process)
{
	std::vector<Process> processes = GetCurrentProcesses();
	std::vector<Process>::iterator iter = processes.begin();
	for (; iter != processes.end(); ++iter)
	{
		if (_stricmp(iter->ExeFile().c_str(), process.ExeFile().c_str()) == 0 ||
			_stricmp(iter->Filename().c_str(), process.Filename().c_str()) == 0)
		{
			HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, iter->ProcessID());
			if (hProcess)
			{
				TerminateProcess(hProcess, 0);
			}
		}
	}
}

