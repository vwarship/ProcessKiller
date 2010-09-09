#pragma once
#include <iostream>
#include <string>
#include <vector>

class Process;

class FileHandler
{
public:
	FileHandler();
	~FileHandler(void);

	void Open(const char* filename, const char* mode);
	void Read(std::vector<Process>& processes);
	void Write(const Process& process);
	void Close();

private:
	long FileHandler::GetFileSize();

public:
	static const char* FILE_READ;
	static const char* FILE_WRITE;

private:
	std::string _filename;
	FILE* _File;

};
