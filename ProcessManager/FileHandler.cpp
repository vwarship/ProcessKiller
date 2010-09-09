#include "StdAfx.h"
#include "FileHandler.h"
#include <stdio.h>
#include "Process.h"

const char* FileHandler::FILE_READ = "r";
const char* FileHandler::FILE_WRITE = "w";

FileHandler::FileHandler()
: _File(NULL)
{
}

FileHandler::~FileHandler(void)
{
	Close();
}

void FileHandler::Open(const char* filename, const char* mode)
{
	_filename = std::string(filename);
	Close();
	_File = fopen(_filename.c_str(), mode);
}

void FileHandler::Read(std::vector<Process>& processes)
{
	processes.clear();

	if (_File)
	{
		long file_size = GetFileSize();
		if (file_size <= 0)
			return;

		char* buf = (char*)malloc(file_size + 1);
		memset(buf, 0, file_size+1);
		fread(buf, 1, file_size, _File);

		const std::string str(buf);
		free(buf);

		std::string::size_type prev = 0;
		std::string::size_type pos = 0;
		while (true)
		{
			pos = str.find_first_of("\r\n", pos);

			Process process;
			if (pos == std::string::npos)
			{
				std::string exe_file(str.substr(prev));
				if (!exe_file.empty())
				{
					process.ExeFile(exe_file);
					processes.push_back(process);
				}
				break;
			}

			if (pos > prev)
			{
				process.ExeFile(str.substr(prev, pos-prev));
				processes.push_back(process);
			}

			++pos;
			prev = pos;
		}
	}
}

long FileHandler::GetFileSize()
{
	long position = ftell(_File);
	fseek(_File, 0L, SEEK_END);

	long file_size = ftell(_File);
	fseek(_File, position, SEEK_SET);

	return file_size;
}

void FileHandler::Write(const Process& process)
{
	if (_File)
	{
		std::string processData(process.ExeFile());
		processData.append("\t");
		processData.append(process.Filename());
		processData.append("\n");
		fwrite(processData.c_str(), 1, processData.length(), _File);
	}
}

void FileHandler::Close()
{
	if (_File)
	{
		fclose(_File);
		_File = NULL;
	}
}
