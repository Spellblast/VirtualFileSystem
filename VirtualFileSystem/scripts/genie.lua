solution "VirtualFileSystem"
	configurations {
		"Debug",
		"Release",
		"linux"
	}

	platforms {
		"x32",
		"x64",
		"Native",
	}


location ".."
targetdir "../bin"
debugdir "../bin"
startproject "example1"

PROJ_DIR =path.getabsolute("..")
flags { "NoExceptions", "NoRTTI", "NoPCH" }

    configuration { "Release" }
    	flags { "Optimize", "OptimizeSpeed", "NoEditAndContinue", "No64BitChecks" }
		defines { "NDEBUG", "PLATFORM_Linux" }
		objdir (path.join(PROJ_DIR,  "/release"))
    configuration { "Debug" }
		flags {"Symbols" }
		defines { "DEBUG", "PLATFORM_Linux" }
		objdir (path.join(PROJ_DIR,  "/debug"))

	configuration { "linux" }
		defines{"LINUX"}
		buildoptions {
			"-std=c++14"
		} 		


	configuration { "gmake" }
		buildoptions { 
			"-msse4.1",
			"-fPIC",
			"-std=c++14"
		}

    configuration {}

project "VirtualFileSystem"
	
	kind "ConsoleApp"
	language "C++"
	

files {
	path.join(PROJ_DIR, "*.cpp"),
	path.join(PROJ_DIR, "*.h"),

}

	includedirs{
path.join(PROJ_DIR, "include"),
}
