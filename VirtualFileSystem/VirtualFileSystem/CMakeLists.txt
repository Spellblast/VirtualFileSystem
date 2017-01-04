cmake_minimum_required(VERSION 2.8.4)

project(VirtualFileSystem)
set(
source_list
  ../File.cpp
  ../File.h
  ../FileSystem.cpp
  ../FileSystem.h
  ../LinuxFile.cpp
  ../LinuxFile.h
  ../VirtualFileSystem.cpp
  ../WindowsFile.cpp
  ../WindowsFile.h
  ../stdafx.cpp
  ../stdafx.h
  ../targetver.h
)
add_definitions(-D_DEBUG)
add_executable(VirtualFileSystem ${source_list})
