workspace "Win32+OpenGL With ImGui Docking"
    architecture "x64"
    configurations { "Debug", "Release" }

    links { "opengl32.lib" }


project "Application"
    location "Application"
    kind "ConsoleApp"
    language "C++"
    
    targetdir "build/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

    files { 
        "./%{prj.name}/**.h",
        "./%{prj.name}/**.hpp",
        "./%{prj.name}/**.c",
        "./%{prj.name}/**.cpp",

        -- ImGUI CPP files
        "./thirdparty/imgui/*.cpp",
        "./thirdparty/imgui/backends/imgui_impl_win32.cpp",
        "./thirdparty/imgui/backends/imgui_impl_opengl3.cpp"
    }

    includedirs {       
        -- Thirdparty
        "./%{prj.name}/",
        "./thirdparty/",
        "./thirdparty/imgui/",
    }

-- Windows system

    filter "system:windows"
        system "windows"
        cppdialect "C++17"
        systemversion "latest"    

-- Linux system
    filter "system:linux"
        system "linux"
        cppdialect "gnu++17"
        links { "glfw", "GL", "m", "X11", "pthread", "Xi", "Xrandr", "dl" }

-- Build configurations
    filter "configurations:Debug"
        defines "DEBUG"
        symbols "On"

    filter "configurations:Release"
        defines "NDEBUG"
        optimize "On"