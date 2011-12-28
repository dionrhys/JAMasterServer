solution "JAMasterServer"
  configurations { "Debug", "Release" }
  location "build"

project "JAMasterServer"
  kind "ConsoleApp"
  language "C++"
  location "build"
  
  files {
    -- Libraries
    "cJSON.h", "cJSON.c",
    
    -- Classes
    "Command.h", "Command.cpp",
    "NetAdr.h", "NetAdr.cpp",
    "Q3OobMsg.h", "Q3OobMsg.cpp",
    
    -- Static files
    "net.h", "net.cpp",
    "oob_cmds.h", "oob_cmds.cpp",
    
    -- Jumbled mess
    "Common.cpp", "jams.cpp", "jams_local.h", "main.cpp",
    "rcon.h", "rcon.cpp", "Server.h", "Server.cpp",
    
    -- Platform-specific headers
    "platform.h", "platform_win32.cpp", "platform_posix.cpp"
  }