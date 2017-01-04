#pragma once

#include <memory>
#include <vector>
#include "File.h"

class FileSystem
{
public:
	
	FileSystem();
	~FileSystem();
	

	void MountDirectory(const std::string & directory);
	std::string GetPhysicalFilePath(const std::string & filename) const;
	void GetFilesInDirectory(std::vector<std::string> & file_table, const std::string & directory) const;
	void GetFilesWithExtension(std::vector<std::string> & file_table, const std::string & extension) const;
	std::unique_ptr<File> GetFile(const std::string & filename) const;
	std::string ExtractExtension(const std::string & filename);

private :
	struct FileEntry
	{
		std::string m_Directory;
		std::string m_Name;
		std::string m_Extension;
	};

	std::vector<FileEntry> m_Files;
};

