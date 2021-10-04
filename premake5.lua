workspace "ai"
	configurations { "debug", "release" }

	startproject "game"

project "game"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"

	architecture "x64"

	targetdir "bin"
	objdir "obj"

	files {
		"src/**.hpp",
		"src/**.cpp",
		"src/**.h",
		"src/**.c"
	}

	-- Link the provided version of SDL on Windows.
	filter "system:windows"
		includedirs {
			"sdl2/include"
		}

		links {
			"sdl2/lib/SDL2.lib",
			"sdl2/lib/SDL2main.lib",
		}

	-- Links to the version of SDL provided by the distro's package manager
	filter "system:linux"
		links {
			"SDL2"
		}

	filter "configurations:debug"
		symbols "on"
		defines { "DEBUG" }

	filter "configurations:release"
		optimize "on"
		defines { "RELEASE" }

	filter {"system:linux", "configurations:debug" }
		-- Generate a Vim tags file, so the "go to definition" feature
		-- of Vim can be used effectively.
		postbuildcommands { "ctags -R" }

	filter { "system:windows", "configurations:debug" }
		postbuildcommands { "copy sdl2\\lib\\debug\\SDL2.dll SDL2.dll" }

	filter { "system:windows", "configurations:release" }
		postbuildcommands { "copy sdl2\\lib\\release\\SDL2.dll SDL2.dll" }
