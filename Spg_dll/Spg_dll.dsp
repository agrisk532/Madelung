# Microsoft Developer Studio Project File - Name="Spg_dll" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=Spg_dll - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Spg_dll.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Spg_dll.mak" CFG="Spg_dll - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Spg_dll - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Spg_dll - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Spg_dll - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "SPG_DLL_EXPORTS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "D:\Projects\Madelung\UnitCelldll" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "SPG_DLL_EXPORTS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib unitcelldll.lib /nologo /dll /machine:I386 /libpath:"D:\Projects\Madelung\UnitCelldll\Release"

!ELSEIF  "$(CFG)" == "Spg_dll - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "SPG_DLL_EXPORTS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "D:\Projects\Madelung\UnitCelldll" /I "D:\Projects\Madelung" /I "D:\Projects\Win32MTProgramming\Mcl4Mfc\INCLUDE" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "SPG_DLL_EXPORTS" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib unitcelldll.lib /nologo /dll /debug /machine:I386 /pdbtype:sept /libpath:"D:\Projects\Madelung\UnitCelldll\Debug"

!ENDIF 

# Begin Target

# Name "Spg_dll - Win32 Release"
# Name "Spg_dll - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\A112.CPP
# End Source File
# Begin Source File

SOURCE=.\A121.CPP
# End Source File
# Begin Source File

SOURCE=.\B112.CPP
# End Source File
# Begin Source File

SOURCE=.\C121.CPP
# End Source File
# Begin Source File

SOURCE=.\C2dm2dc2s1da.cpp
# End Source File
# Begin Source File

SOURCE=.\C2dm2dc2s1dm.cpp
# End Source File
# Begin Source File

SOURCE=.\EqPositions.cpp
# End Source File
# Begin Source File

SOURCE=.\I112.CPP
# End Source File
# Begin Source File

SOURCE=.\I121.CPP
# End Source File
# Begin Source File

SOURCE=.\I2dm2dm2dm.cpp
# End Source File
# Begin Source File

SOURCE=.\P1.cpp
# End Source File
# Begin Source File

SOURCE=.\P112.CPP
# End Source File
# Begin Source File

SOURCE=.\P112dm.cpp
# End Source File
# Begin Source File

SOURCE=.\P112s1.cpp
# End Source File
# Begin Source File

SOURCE=.\P11m.cpp
# End Source File
# Begin Source File

SOURCE=.\P121.CPP
# End Source File
# Begin Source File

SOURCE=.\P12dm1.cpp
# End Source File
# Begin Source File

SOURCE=.\P12s11.cpp
# End Source File
# Begin Source File

SOURCE=.\P1m1.cpp
# End Source File
# Begin Source File

SOURCE=.\P23.CPP
# End Source File
# Begin Source File

SOURCE=.\P2dm2dm2dm.cpp
# End Source File
# Begin Source File

SOURCE=.\P_1.cpp
# End Source File
# Begin Source File

SOURCE=.\Spg_dll.cpp
# End Source File
# Begin Source File

SOURCE=.\spg_names.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\A112.H
# End Source File
# Begin Source File

SOURCE=.\A121.H
# End Source File
# Begin Source File

SOURCE=.\AllSGHeaders.h
# End Source File
# Begin Source File

SOURCE=.\B112.H
# End Source File
# Begin Source File

SOURCE=.\C121.H
# End Source File
# Begin Source File

SOURCE=.\C2dm2dc2s1da.h
# End Source File
# Begin Source File

SOURCE=.\C2dm2dc2s1dm.h
# End Source File
# Begin Source File

SOURCE=.\EqPositions.h
# End Source File
# Begin Source File

SOURCE=.\I112.H
# End Source File
# Begin Source File

SOURCE=.\I121.H
# End Source File
# Begin Source File

SOURCE=.\I2dm2dm2dm.h
# End Source File
# Begin Source File

SOURCE=.\P1.h
# End Source File
# Begin Source File

SOURCE=.\P112.H
# End Source File
# Begin Source File

SOURCE=.\P112dm.h
# End Source File
# Begin Source File

SOURCE=.\P112s1.h
# End Source File
# Begin Source File

SOURCE=.\P11m.h
# End Source File
# Begin Source File

SOURCE=.\P121.H
# End Source File
# Begin Source File

SOURCE=.\P12dm1.h
# End Source File
# Begin Source File

SOURCE=.\P12s11.h
# End Source File
# Begin Source File

SOURCE=.\P1m1.h
# End Source File
# Begin Source File

SOURCE=.\P23.H
# End Source File
# Begin Source File

SOURCE=.\P2dm2dm2dm.h
# End Source File
# Begin Source File

SOURCE=.\P_1.h
# End Source File
# Begin Source File

SOURCE=.\Spg_dll.h
# End Source File
# Begin Source File

SOURCE=.\spg_names.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\win32HeaderSG.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
