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
			"lib/x64/",
			"$(AMDAPPSDKROOT)/lib/x86_64"
		}

		includedirs {
			"lib/include",
			"$(CUDA_PATH)/include",
			"$(AMDAPPSDKROOT)/include"
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
			libdirs {
				"$(AMDAPPSDKROOT)/lib/x86_64/sdk"
			}

			links {"GL", "GLEW", "SDL2", "OpenCL"}

		configuration {"gmake", "windows"}
			links {"mingw32", "imagehlp", "SDL2main", "SDL2", "glew32s", "opengl32", "opencl"}

			postbuildcommands {
				"copy \"lib\\x64\\SDL2.dll\" \"build\""
			}

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
