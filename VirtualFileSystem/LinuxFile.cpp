#include "LinuxFile.h"
#if defined(PLATFORM_Linux)
#include <fcntl.h>
#include <sys/stat.h>


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
		result = O_WRONLY | O_CREAT | _O_TRUNC;
		break;
	case Mode::READWRITE:
		result = O_RDWR | O_CREAT | _O_TRUNC;
		break;
	default:
		throw "Unkown mode";
	}
	m_fd = open(m_fileName, result, 0644);
}

size_t LinuxFile::Read(uint8_t* buffer, size_t size)
{
	if (m_fd == 0)
	{
		throw "Invalid Handle!";
	}

	size_t bytes_read;

	size = sizeof(buffer);
	bytes_read = read(m_fd, buffer, nbytes);
	return bytes_read;
}

size_t LinuxFile::Write(const uint8_t* buffer, size_t size)
{
	if (m_fd == 0)
	{
		throw "Invalid Handle!";
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
