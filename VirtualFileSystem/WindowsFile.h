#pragma once
#include "File.h"
#include <windows.h>
#include <string>

class WindowsFile : public File
{
public:
	WindowsFile(const std::string & fileName);
	~WindowsFile();

//	size_t GetByteIndex() override;
	bool Open(Mode mode) override;
	size_t Read(uint8_t * buffer, size_t size) override;
	size_t Write(const uint8_t * buffer, size_t size) override;
	void Close() override;

protected:
	std::string m_fileName;

private:
	HANDLE m_hFile;
	
};

