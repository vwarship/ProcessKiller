#pragma once
#include <iostream>
#include <string>

class Process
{
public:
	Process(void);
	Process(DWORD processID);

	DWORD ProcessID() { return _processID; }
	void Filename(const std::string& filename);
	void Filename(char * pFilename);
	const std::string& Filename() const;

	void ExeFile(const std::string& exeFile);
	void ExeFile(char * pExeFile);
	const std::string& ExeFile() const;

private:
	DWORD _processID;
	std::string _filename;
	std::string _exeFile;

};
