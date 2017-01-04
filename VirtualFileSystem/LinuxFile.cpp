#include "LinuxFile.h"
#if defined(PLATFORM_Linux)
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <iostream>


LinuxFile::LinuxFile(const std::string & fileName) : m_fileName(fileName), m_fd(0)
{
}


LinuxFile::~LinuxFile()
{
}

bool LinuxFile::Open(Mode mode)
{

	int result = 0;

	switch (mode)
	{
	case Mode::READ:
		result = O_RDONLY;
		break;
	case Mode::WRITE:
		result = O_WRONLY | O_CREAT | O_TRUNC;
		break;
	case Mode::READWRITE:
		result = O_RDWR | O_CREAT | O_TRUNC;
		break;
	default:
		//throw "Unkown mode";
		break;
	}
	m_fd = open(m_fileName.c_str(), result, 0644);
	if (m_fd == -1)
	{
		std::cout << "Couldn't open file " + m_fileName << std::endl;
		return false;
	}
	return true;
}

size_t LinuxFile::Read(uint8_t* buffer, size_t size)
{
	if (m_fd == 0)
	{
		std::cout << "Invalid Handle!" << std::endl;
	}

	size_t bytes_read;

	size = sizeof(buffer);
	bytes_read = read(m_fd, buffer, size);
	return bytes_read;
}

size_t LinuxFile::Write(const uint8_t* buffer, size_t size)
{
	if (m_fd == 0)
	{
		std::cout << "Invalid Handle!" << std::endl;
	}
	int result;
	result = write(m_fd, buffer, size);
	return result;
}

void LinuxFile::Close()
{
	
	close(m_fd);
}
#endif
