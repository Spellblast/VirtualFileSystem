#pragma once
#include "File.h"
#include <string>

class LinuxFile :
	public File
{
public:
	LinuxFile(const std::string & fileName);
	~LinuxFile();

	bool Open(Mode mode) override;
	size_t Read(uint8_t* buffer, size_t size) override;
	size_t Write(const uint8_t* buffer, size_t size) override;
	void Close() override;

protected:
	std::string m_fileName;

private:
	int m_fd;
};

