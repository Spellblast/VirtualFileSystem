// VirtualFileSystem.cpp : Defines the entry point for the console application.

#include "stdafx.h"
#include "FileSystem.h"
#include "File.h"
#include <iostream>
#include <cassert>
#include <string>


int main()
{
	FileSystem system;

	system.MountDirectory("D:\\TestDirectory");
	system.MountDirectory("D:\\AnotherTestDirectory");

	auto fullpath = system.GetPhysicalFilePath("file.txt");
	std::cout << fullpath << std::endl;

	fullpath = system.GetPhysicalFilePath("unique.txt");
	std::cout << fullpath << std::endl;

	std::cout << std::endl;
	std::vector<std::string> file_table;

	//Should return all files in C:\\TestDirectory\\Some\\Directory
	// and C:\\AnotherTestDirectory\\Some\\Directory
	system.GetFilesInDirectory(file_table, "Some\\Directory");

	for (auto & path : file_table)
	{
		std::cout << path << std::endl;
	}

	std::cout << std::endl;
	file_table.resize(0);

	//Should return all .txt files in C:\\TestDirectory and C:\\AnotherTestDirectory
	system.GetFilesWithExtension(file_table, ".txt");

	for (auto & path : file_table)
	{
		std::cout << path << std::endl;
	}
	std::cout << std::endl;

	std::unique_ptr<File> input_file = system.GetFile("input.txt");
	std::unique_ptr<File> output_file = system.GetFile("output.txt");


	input_file->Open(File::Mode::READ);
	output_file->Open(File::Mode::WRITE);

	size_t read_byte_count, written_byte_count;
	std::vector<uint8_t> buffer;

	constexpr int buffer_size = 1024;

	buffer.resize(buffer_size);

	do
	{
		read_byte_count = input_file->Read(buffer.data(), buffer.size());
		written_byte_count = output_file->Write(buffer.data(), read_byte_count);

		assert(written_byte_count == read_byte_count);

	} while (read_byte_count > 0);

	std::cout << "Copied file content from " << system.GetPhysicalFilePath("input.txt") << " to " << system.GetPhysicalFilePath("output.txt") << std::endl;
	input_file->Close();
	output_file->Close();

    return 0;
}

