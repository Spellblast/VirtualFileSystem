#include "WindowsFile.h"
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <iostream>
#include <algorithm>
#include <sstream>
#include "FileSystem.h"


FileSystem::FileSystem()
{
}


FileSystem::~FileSystem()
{
}

void FileSystem::MountDirectory(const std::string& directory)
{
	WIN32_FIND_DATAA findFileData;
	HANDLE hFind;

	hFind = FindFirstFileA((directory + "\\*").c_str(), &findFileData);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		std::cout << "Unable to open directory " << directory << "! (" << std::hex << GetLastError() << ")" <<std::endl;
		return;
	}

	do
	{
		if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			//Store the directory and mount it again
			if (strcmp(findFileData.cFileName,".") == 0 || strcmp(findFileData.cFileName,"..") == 0)
			{
				continue;
			}	
			MountDirectory(directory + "\\" + findFileData.cFileName);
		}
		else
		{
			//Store the file
			//std::cout << directory + "\\" + findFileData.cFileName << std::endl;
			m_Files.push_back({ directory ,findFileData.cFileName, ExtractExtension(findFileData.cFileName) });
		}
	} while (FindNextFileA(hFind, &findFileData) != 0);
	
	
	FindClose(hFind);
}


std::string FileSystem::GetPhysicalFilePath(const std::string& filename) const
{
	auto it = m_Files.begin();
	std::vector<std::string> PossibleFiles;
	while (it != m_Files.end())
	{
		it = find_if(it, m_Files.end(), [file = filename](FileEntry e) -> bool {return e.m_Name == file; });
		if (it != m_Files.end())
		{
			FileEntry e = *it;
			PossibleFiles.push_back(e.m_Directory + "\\" + filename);
			++it;
		}
		
	}
	
	if (PossibleFiles.size() > 0)
	{
		if (PossibleFiles.size() == 1) return PossibleFiles[0];

		std::cout << "More then one file found with the name " << filename << std::endl;
		std::cout << "select the correct one :" << std::endl;
		for (int i = 0; i < PossibleFiles.size(); ++i)
		{
						
			std::cout << "[" << i << "] " << PossibleFiles[i] << std::endl;
		}
	}
	else
	{
		return "File not found!";
	}
	std::string input = "";
	int myNumber = 0;
	while (true) {
		std::cout << "Please enter a valid number: ";
		getline(std::cin, input);

		// This code converts from string to number safely.
		std::stringstream myStream(input);
		if (myStream >> myNumber && myNumber < PossibleFiles.size())
			break;
		std::cout << "Invalid number, please try again" << std::endl;
	}
	std::cout << "You entered: " << myNumber << std::endl << std::endl;
	return PossibleFiles[myNumber];

}

void FileSystem::GetFilesInDirectory(std::vector<std::string>& file_table, const std::string& directory) const
{
	auto it = m_Files.begin();
	while (it != m_Files.end())
	{
		it = find_if(it, m_Files.end(), [directory = directory](FileEntry e) -> bool {return e.m_Directory.find(directory); });
		if (it != m_Files.end())
		{
			FileEntry e = *it;
			file_table.push_back(e.m_Directory + "\\" + e.m_Name);
			++it;
		}
	}
}

void FileSystem::GetFilesWithExtension(std::vector<std::string>& file_table, const std::string& extension) const
{
	auto it = m_Files.begin();
	while (it != m_Files.end())
	{
		it = find_if(it, m_Files.end(), [extension = extension](FileEntry e) -> bool {return e.m_Extension == extension; });
		if (it != m_Files.end())
		{
			FileEntry e = *it;
			file_table.push_back(e.m_Directory + "\\" + e.m_Name);
			++it;
		}
	}
}

std::unique_ptr<File> FileSystem::GetFile(const std::string& filename) const
{
	auto physicalFilePath = GetPhysicalFilePath(filename);
	if (physicalFilePath != "")
	{
		return std::unique_ptr<File>(new WindowsFile(physicalFilePath));
	}
	return std::unique_ptr<File>();
}

std::string FileSystem::ExtractExtension(const std::string& filename)
{
	std::size_t found = filename.find_last_of(".");
	return filename.substr(found);
}
