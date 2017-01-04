solution "VirtualFileSystem"
configurations{
	"Debug",
	"Release",
}

platforms{
	"x32",
	"x64",
	"Native",
}

language "C++"
location ".."

PROJ_DIR = path.getabsolute("..")

project "VirtualFileSystem"
	kind "ConsoleApp"
	
	debugdir (path.join(PROJ_DIR, "Debug"))
	targetdir (path.join(PROJ_DIR, "Build"))
	defines{"PLATFORM_Linux"}
	flags{"Unicode"}	
	
	files{
		path.join(PROJ_DIR, "*.cpp"),
		path.join(PROJ_DIR, "*.h"),		
	}
	
	configuration {"gmake"}
		buildoptions_cpp{
			"-std=c++14"
		}
	excludes {
		path.join(PROJ_DIR, "Debug*"), 
		}
	
	
	
