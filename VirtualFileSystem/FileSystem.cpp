#if defined(PLATFORM_Windows)
#include "WindowsFile.h"
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#elif defined(PLATFORM_Linux)
#include "LinuxFile.h"
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#endif
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
#if defined(PLATFORM_Windows)
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
#elif defined(PLATFORM_Linux)
	DIR *dir;
	struct dirent *ent;
	struct stat st;

	dir = opendir(directory.c_str());
	if(dir == NULL)
	{
		std::cout << "Error could not open directory " + directory << std::endl;
		return;
	}
	while ((ent = readdir(dir)) != NULL) {
		std::string file_name = ent->d_name;
		std::string full_file_name = directory + "/" + file_name;

		if (file_name[0] == '.')
			continue;

		if (stat(full_file_name.c_str(), &st) == -1)
			continue;

		const bool is_directory = (st.st_mode & S_IFDIR) != 0;
		if (is_directory)
		{
			MountDirectory(directory + "/" + file_name);
		}else
		{
			m_Files.push_back({ directory ,file_name, ExtractExtension(file_name) });
		}
	}
	closedir(dir);

#endif
}


std::string FileSystem::GetPhysicalFilePath(const std::string& filename) const
{
	auto it = m_Files.begin();
	std::vector<std::string> PossibleFiles;
	while (it != m_Files.end())
	{
		it = find_if(it, m_Files.end(), [file = filename](FileEntry e) -> bool {return strncmp(e.m_Name.c_str(), file.c_str(), file.length()) == 0; });
		if (it != m_Files.end())
		{
			FileEntry e = *it;
#if defined(PLATFORM_Windows)
			PossibleFiles.push_back(e.m_Directory + "\\" + filename);
#elif defined(PLATFORM_Linux)
			PossibleFiles.push_back(e.m_Directory + "/" + filename);
#endif
			++it;
		}
		
	}
	
	if (PossibleFiles.size() > 0)
	{
		if (PossibleFiles.size() == 1) return PossibleFiles[0];

		std::cout << "More then one file found with the name " << filename << std::endl;
		std::cout << "select the correct one :" << std::endl;
		for (size_t i = 0; i < PossibleFiles.size(); ++i)
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
		if (myStream >> myNumber && myNumber < (int)PossibleFiles.size())
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
#if defined(PLATFORM_Windows)
			file_table.push_back(e.m_Directory + "\\" + e.m_Name);
#elif defined(PLATFORM_Linux)
			file_table.push_back(e.m_Directory + "/" + e.m_Name);
#endif
			++it;
		}
	}
}

void FileSystem::GetFilesWithExtension(std::vector<std::string>& file_table, const std::string& extension) const
{
	auto it = m_Files.begin();
	while (it != m_Files.end())
	{
		it = find_if(it, m_Files.end(), [extension = extension](FileEntry e) -> bool {return strncmp(e.m_Extension.c_str(),extension.c_str(),extension.length()) == 0; });
		if (it != m_Files.end())
		{
			FileEntry e = *it;
#if defined(PLATFORM_Windows)
			file_table.push_back(e.m_Directory + "\\" + e.m_Name);
#elif defined(PLATFORM_Linux)
			file_table.push_back(e.m_Directory + "/" + e.m_Name);
#endif		
			++it;
		}
	}
}

std::unique_ptr<File> FileSystem::GetFile(const std::string& filename) const
{
	auto physicalFilePath = GetPhysicalFilePath(filename);
	if (physicalFilePath != "")
	{
#if defined(PLATFORM_Windows)
		return std::unique_ptr<File>(new WindowsFile(physicalFilePath));
#elif defined(PLATFORM_Linux)
		return std::unique_ptr<File>(new LinuxFile(physicalFilePath));
#endif
	}
	return std::unique_ptr<File>();
}

std::string FileSystem::ExtractExtension(const std::string& filename)
{
	std::size_t found = filename.find_last_of(".");
	return filename.substr(found);
}
