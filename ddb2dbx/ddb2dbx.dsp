# Microsoft Developer Studio Project File - Name="ddb2dbx" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=ddb2dbx - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ddb2dbx.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ddb2dbx.mak" CFG="ddb2dbx - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ddb2dbx - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "ddb2dbx - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ddb2dbx - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W4 /GX /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "_AFXDLL" /FR /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 /nologo /subsystem:console /map /debug /machine:I386

!ELSEIF  "$(CFG)" == "ddb2dbx - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W4 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "ddb2dbx - Win32 Release"
# Name "ddb2dbx - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\Shared\db_dbx.cpp
# End Source File
# Begin Source File

SOURCE=.\ddb2dbx.cpp
# End Source File
# Begin Source File

SOURCE=..\Shared\file.cpp
# End Source File
# Begin Source File

SOURCE=..\shared\file_find.cpp
# End Source File
# Begin Source File

SOURCE=..\Shared\file_walker.cpp
# End Source File
# Begin Source File

SOURCE=..\Shared\hash_murmur3.cpp
# End Source File
# Begin Source File

SOURCE=..\Shared\mmf.cpp
# End Source File
# Begin Source File

SOURCE=..\shared\prng_int.cpp
# End Source File
# Begin Source File

SOURCE=..\Shared\search_bmh.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=..\Shared\text.cpp
# End Source File
# Begin Source File

SOURCE=..\Shared\timestamp.cpp
# End Source File
# Begin Source File

SOURCE=..\Shared\vector.cpp
# End Source File
# Begin Source File

SOURCE=..\Shared\vector_sorted.cpp
# End Source File
# Begin Source File

SOURCE=..\Shared\xlat.cpp
# End Source File
# Begin Source File

SOURCE=..\Shared\xlat_tables.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\Shared\db_dbx.h
# End Source File
# Begin Source File

SOURCE=..\Shared\file.h
# End Source File
# Begin Source File

SOURCE=..\shared\file_find.h
# End Source File
# Begin Source File

SOURCE=..\Shared\file_walker.h
# End Source File
# Begin Source File

SOURCE=..\Shared\hash_murmur3.h
# End Source File
# Begin Source File

SOURCE=..\Shared\mmf.h
# End Source File
# Begin Source File

SOURCE=..\shared\prng_int.h
# End Source File
# Begin Source File

SOURCE=..\Shared\search_bmh.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=..\Shared\text.h
# End Source File
# Begin Source File

SOURCE=..\Shared\timestamp.h
# End Source File
# Begin Source File

SOURCE=..\Shared\vector.h
# End Source File
# Begin Source File

SOURCE=..\Shared\vector_sorted.h
# End Source File
# Begin Source File

SOURCE=..\Shared\xlat.h
# End Source File
# Begin Source File

SOURCE=..\Shared\xlat_tables.h
# End Source File
# End Group
# End Target
# End Project
