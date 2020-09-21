workspace "JadeEngine"
    architecture "x64"

    configurations { 
        "Debug", 
        "Release",
        "Dist"
    }

    startproject "JadeEditor"

-- This is a helper variable, to concatenate the sys-arch
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "JadeEngine/vendor/GLFW/include"
IncludeDir["Glad"] = "JadeEngine/vendor/glad/include"
IncludeDir["ImGui"] = "JadeEngine/vendor/imguiVendor"
IncludeDir["glm"] = "JadeEngine/vendor/glmVendor"
IncludeDir["stb"] = "JadeEngine/vendor/stb"
IncludeDir["entt"] = "JadeEngine/vendor/enttVendor/single_include"
IncludeDir["Box2D"] = "JadeEngine/vendor/box2DVendor/include"
IncludeDir["Json"] = "JadeEngine/vendor/nlohmann-json/single_include"
IncludeDir["Mono"] = "JadeEngine/vendor/monoVendor/include/mono-2.0"

include "JadeEngine/vendor/GLFW"
include "JadeEngine/vendor/glad"
include "JadeEngine/vendor/imguiVendor"
include "JadeEngine/vendor/box2DVendor"

project "JadeEngine"
    location "JadeEngine"
    kind "SharedLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "off"

    fullOutputDir = "bin/" .. outputdir .. "/%{prj.name}"
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files {
        "%{prj.name}/Jade.h",
		"%{prj.name}/cpp/**.cpp",
		"%{prj.name}/include/**.h",
		"%{prj.name}/vendor/glmVendor/glm/**.hpp",
		"%{prj.name}/vendor/glmVendor/glm/**.inl",
        "%{prj.name}/vendor/nlohmann-json/single_include/**.hpp"
	}

    disablewarnings { 
        "4251" 
    }

	defines {
        "_CRT_SECURE_NO_WARNINGS",
        "NOMINMAX",
        "_JADE_DLL",
        "ENTT_API_EXPORT",
        "GLFW_DLL"
	}

	includedirs {
        "%{prj.name}/include",
        "%{prj.name}/vendor",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
        "%{IncludeDir.Box2D}",
		"%{IncludeDir.glm}",
        "%{IncludeDir.stb}",
        "%{IncludeDir.entt}",
        "%{IncludeDir.Json}",
        "%{IncludeDir.Mono}"
	}

	links {
        "JadeScriptRuntime",
		"GLFW",
        "opengl32.lib",
        "Glad",
        "Box2D",
        "JadeEngine/vendor/monoVendor/lib/mono-2.0-sgen.lib",
	}

    filter { "system:windows", "configurations:Debug" }
        buildoptions "/MDd"        

    filter { "system:windows", "configurations:Release" }
        buildoptions "/MD"

	filter "system:windows"
		systemversion "latest"

		defines {
			"GLFW_INCLUDE_NONE",
            "JSON_MultipleHeaders"
		}

    postbuildcommands {
        "mkdir \"$(SolutionDir)%{fullOutputDir}\\..\\JadeEditor\"",
        "copy /y \"$(SolutionDir)%{fullOutputDir}\\JadeEngine.dll\" \"$(SolutionDir)%{fullOutputDir}\\..\\JadeEditor\\JadeEngine.dll\"",
        "copy /y \"$(SolutionDir)JadeEngine\\vendor\\monoVendor\\bin\\mono-2.0-sgen.dll\" \"$(SolutionDir)%{fullOutputDir}\\..\\JadeEditor\\mono-2.0-sgen.dll\"",
        "copy /y \"$(SolutionDir)JadeEngine\\vendor\\GLFW\\bin\\%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}\\GLFW\\GLFW.dll\" \"$(SolutionDir)%{fullOutputDir}\\..\\JadeEditor\\GLFW.dll\""
    }
    
	filter "configurations:Debug"
		defines {
			"_JADE_DEBUG",
			"_JADE_ENABLE_ASSERTS"
		}
		runtime "Debug"
		symbols "on"


	filter "configurations:Release"
		defines "_JADE_RELEASE"
		runtime "Release"
		optimize "on"


	filter "configurations:Dist"
		defines "_JADE_DIST"
		runtime "Release"
        optimize "on"
        


project "JadeEditor"
    location "JadeEditor"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"	
    staticruntime "off"

    fullOutputDir = "bin/" .. outputdir .. "/%{prj.name}"
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files {
        "%{prj.name}/include/**.h",
        "%{prj.name}/cpp/**.cpp"
    }

    disablewarnings { 
        "4251" 
    }

    includedirs {
        "JadeEngine",
        "JadeEngine/include",
        "JadeEngine/vendor",
        "%{prj.name}/include",
        "%{IncludeDir.glm}",
        "%{IncludeDir.entt}",
        "%{IncludeDir.Glad}",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.Box2D}",
        "%{IncludeDir.Json}",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.Mono}",
    }

    links {
        "JadeEngine",
        "ImGui",
		"GLFW",
        "opengl32.lib",
        "Glad",
        "Box2D",
        "JadeScriptCompiler",
        "JadeEngine/vendor/monoVendor/lib/mono-2.0-sgen.lib",
    }

    filter { "system:windows", "configurations:Debug" }
        buildoptions "/MDd"        

    filter { "system:windows", "configurations:Release" }
        buildoptions "/MD"

    filter "system:windows"
        systemversion "latest"		

        postbuildcommands {
            "xcopy /s /e /q /y /i \"$(SolutionDir)\\%{prj.name}\\assets\" \"$(SolutionDir)\\%{fullOutputDir}\\assets\" > nul",
        }

        defines {
            "_JADE_PLATFORM_WINDOWS"
        }

    filter "configurations:Debug"
        defines {
			"_JADE_DEBUG",
			"_JADE_ENABLE_ASSERTS",
            "ENTT_API_IMPORT"
		}
        runtime "Debug"
        symbols "on"


    filter "configurations:Release"
        defines "_JADE_RELEASE"
        runtime "Release"
        optimize "on"


    filter "configurations:Dist"
        defines "_JADE_DIST"
        runtime "Release"
        optimize "on"



project "JadeScriptRuntime"
    location "JadeScriptRuntime"
    kind "SharedLib"
    language "C#"

    editorOutputDir = "bin/" .. outputdir .. "/JadeEditor"
    fullOutputDir = "bin/" .. outputdir .. "/%{prj.name}"
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    nuget {
        "Microsoft.CodeAnalysis.CSharp.Workspaces:3.7.0",
        "Microsoft.CodeAnalysis.CSharp:3.7.0",
        "Microsoft.CodeAnalysis:3.7.0"
	}

    files {
        "%{prj.name}/src/**.cs"
    }

    filter "system:windows"
        postbuildcommands {
            "copy /y \"$(SolutionDir)%{fullOutputDir}\\JadeScriptRuntime.dll\" \"$(SolutionDir)%{editorOutputDir}\\JadeScriptRuntime.dll\"",
             "xcopy /s /e /q /y /i \"$(SolutionDir)%{fullOutputDir}\\*.dll\" \"$(SolutionDir)%{editorOutputDir}\\\" > nul",
        }

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"


    filter "configurations:Release"
        runtime "Release"
        optimize "on"


    filter "configurations:Dist"
        runtime "Release"
        optimize "on"


project "JadeScriptCompiler"
    location "JadeScriptCompiler"
    kind "SharedLib"
    language "C#"

    editorOutputDir = "bin/" .. outputdir .. "/JadeEditor"
    fullOutputDir = "bin/" .. outputdir .. "/%{prj.name}"
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    nuget {
        "Microsoft.CodeAnalysis.CSharp.Workspaces:3.7.0",
        "Microsoft.CodeAnalysis.CSharp:3.7.0",
        "Microsoft.CodeAnalysis:3.7.0"
	}

    links {
        "JadeScriptRuntime"
	}

    files {
        "%{prj.name}/src/**.cs"
    }

    filter "system:windows"
        postbuildcommands {
            "copy /y \"$(SolutionDir)%{fullOutputDir}\\JadeScriptCompiler.exe\" \"$(SolutionDir)%{editorOutputDir}\\JadeScriptCompiler.exe\"",
            "xcopy /s /e /q /y /i \"$(SolutionDir)%{fullOutputDir}\\*.dll\" \"$(SolutionDir)%{editorOutputDir}\\\" > nul",
        }

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"


    filter "configurations:Release"
        runtime "Release"
        optimize "on"


    filter "configurations:Dist"
        runtime "Release"
        optimize "on"