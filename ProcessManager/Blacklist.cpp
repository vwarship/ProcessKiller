#include "StdAfx.h"
#include "Blacklist.h"
#include "FileHandler.h"

void Blacklist::ReadConfigureFile(const char* filename)
{
	std::vector<Process> processes;

	FileHandler fh;
	fh.Open(filename, FileHandler::FILE_READ);
	fh.Read(processes);
	fh.Close();

	SetProcesses(processes);
}

Blacklist Blacklist::GetBlacklist()
{
	//char filenames[][MAX_PATH] = {
	//	"f:\\Projects\\C++\\ACE\\BIN\\Ace.exe",
	//	"f:\\Projects\\C++\\ACE\\BIN\\DispatchingEngine.exe",
	//	"f:\\matlab\\bin\\win32\\matlab.exe",
	//};

	char exeFiles[][MAX_PATH] = {
		"Ace.exe",
		"DispatchingEngine.exe",
		"matlab.exe",
	};

	Blacklist blacklist;

	for (int i=0; i<3; ++i)
	{
		Process process;
		//process.Filename(filenames[i]);
		process.ExeFile(exeFiles[i]);

		blacklist.Add(process);
	}

	return blacklist;
}
