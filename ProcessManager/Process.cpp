#include "StdAfx.h"
#include "Process.h"

Process::Process(void)
: _processID(0)
{
}

Process::Process(DWORD processID)
: _processID(processID)
{
}

void Process::Filename(const std::string& filename)
{
	_filename = filename;
}

void Process::Filename(char * pFilename)
{
	_filename = std::string(pFilename);
}

const std::string& Process::Filename() const
{
	return _filename;
}

void Process::ExeFile(const std::string& exeFile)
{
	_exeFile = exeFile;
}

void Process::ExeFile(char * pExeFile)
{
	_exeFile = std::string(pExeFile);
}

const std::string& Process::ExeFile() const
{
	return _exeFile;
}
