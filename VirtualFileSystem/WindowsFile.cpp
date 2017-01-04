#include "WindowsFile.h"
#include "File.h"
#if defined(PLATFORM_Windows)
#include <windows.h>
#include <tchar.h>



WindowsFile::WindowsFile(const std::string & fileName) : m_fileName(fileName), m_hFile(INVALID_HANDLE_VALUE)
{
}


WindowsFile::~WindowsFile()
{
}


bool WindowsFile::Open(Mode mode)
{
	DWORD access;
	DWORD creation_Disposition;

	switch (mode)
	{
	case Mode::READ:
		access = GENERIC_READ;
		creation_Disposition = OPEN_ALWAYS;
		break;
	case Mode::WRITE:
		access = GENERIC_WRITE;
		creation_Disposition = CREATE_ALWAYS;
		break;
	case Mode::READWRITE:
		access = GENERIC_READ | GENERIC_WRITE;
		creation_Disposition = OPEN_EXISTING;
		break;
	default:
		throw "Unkown mode";
	}
	m_hFile = CreateFileA(m_fileName.c_str(), access, 0, NULL, creation_Disposition,FILE_ATTRIBUTE_NORMAL, NULL);

	return m_hFile != INVALID_HANDLE_VALUE;
}

size_t WindowsFile::Read(uint8_t* buffer, size_t size)
{
	DWORD  dwBytesRead = 0;
	//OVERLAPPED ol = { 0 };
	BOOL bErrorFlag = FALSE;



	if (m_hFile == INVALID_HANDLE_VALUE)
	{
		throw "Invalid Handle!";
	}

	bErrorFlag = ReadFile(m_hFile, buffer, size, &dwBytesRead, NULL);

	if (bErrorFlag == FALSE)
	{
		throw std::exception("Unable to read file!");
	}

	return dwBytesRead;
}

size_t WindowsFile::Write(const uint8_t* buffer, size_t size)
{

	DWORD dwBytesWritten = 0;
	BOOL bErrorFlag = FALSE;


	if (m_hFile == INVALID_HANDLE_VALUE)
	{
		throw "Invalid Handle!";
	}

	bErrorFlag = WriteFile(
		m_hFile,           // open file handle
		buffer,      // start of data to write
		size,  // number of bytes to write
		&dwBytesWritten, // number of bytes that were written
		NULL);

	if (bErrorFlag == FALSE)
	{
		throw "Unable to write to file!";
	}

	return dwBytesWritten;
}

void WindowsFile::Close()
{
	if (m_hFile != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hFile);
		m_hFile = INVALID_HANDLE_VALUE;
	}
	else
	{
		throw "Invalid Handle!";
	}
	
}
#endif

