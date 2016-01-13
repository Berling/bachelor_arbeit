workspace "bachelor_arbeit"
	configurations {"debug", "release"}
	language "C++"
	flags {"FatalWarnings"}
	vectorextensions "SSE2"
	buildoptions "-std=c++14"
	defines {"GLM_FORCE_RADIANS"}

	configuration "linux"
		defines "LINUX"

	configuration "windows"
		defines  "WINDOWS"

	configuration "debug"
		defines { "DEBUG", "STACKTRACE" }
		flags "Symbols"
		optimize "Off"

	configuration "release"
		defines "NDEBUG"
		optimize "Full"

	configuration {"gmake", "linux"}

	configuration {"gmake", "windows"}
		libdirs {
			"lib/sdl2/x86_64-w64-mingw32/lib",
			"lib/sdl2_mixer/x86_64-w64-mingw32/lib"
		}

	project "vbte"
		kind "WindowedApp"
		targetdir "build"
		objdir "build/obj"

		includedirs {
			"include",
			"lib/gli/include",
			"lib/glm/include"
		}

		defines {
			"GLM_SWIZZLE",
			"GLEW_STATIC"
		}

		if _OPTIONS["cc"] == "clang" then
			toolset "clang"
			buildoptions "-stdlib=libc++"
			links {
				"c++",
				"c++abi"
			}
		end

		files "src/vbte/**.cpp"

		configuration {"gmake", "linux" }
			links {"GL", "GLEW", "SDL2"}

		project "vtdg"
			kind "ConsoleApp"
			targetdir "build"
			objdir "build/obj"

			includedirs {
				"include",
				"lib/gli/include",
				"lib/glm/include"
			}

			defines {
				"GLM_SWIZZLE",
				"GLEW_STATIC"
			}

			if _OPTIONS["cc"] == "clang" then
				toolset "clang"
				buildoptions "-stdlib=libc++"
				links {
					"c++",
					"c++abi"
				}
			end

			files "src/vtdg/**.cpp"

			configuration {"gmake", "linux" }
