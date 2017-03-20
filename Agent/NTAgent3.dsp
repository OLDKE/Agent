# Microsoft Developer Studio Project File - Name="NTAgent3" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=NTAgent3 - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "NTAgent3.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "NTAgent3.mak" CFG="NTAgent3 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "NTAgent3 - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "NTAgent3 - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "NTAgent3 - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fr /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 PSAPI.LIB XFS/msxfs.lib XFS/xfs_conf.lib XFS/xfs_supp.lib Shlwapi.lib imagehlp.lib /nologo /subsystem:windows /machine:I386 /out:"C:\EbringAgent\Agent.EXE"

!ELSEIF  "$(CFG)" == "NTAgent3 - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fr /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 PSAPI.LIB XFS/msxfs.lib XFS/xfs_conf.lib XFS/xfs_supp.lib Shlwapi.lib imagehlp.lib /nologo /subsystem:windows /debug /machine:I386 /out:"C:\EbringAgent\AgentD.exe"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "NTAgent3 - Win32 Release"
# Name "NTAgent3 - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\BrowseDir.cpp
# End Source File
# Begin Source File

SOURCE=.\GetPCResours.cpp
# End Source File
# Begin Source File

SOURCE=.\LogicCommon.cpp
# End Source File
# Begin Source File

SOURCE=.\LogicRecvCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\LogicRecvDisp.cpp
# End Source File
# Begin Source File

SOURCE=.\LogicRecvFuns.cpp
# End Source File
# Begin Source File

SOURCE=.\MyDes.cpp
# End Source File
# Begin Source File

SOURCE=.\MyFTP.cpp
# End Source File
# Begin Source File

SOURCE=.\NTAgent3.cpp
# End Source File
# Begin Source File

SOURCE=.\NTAgent3.rc
# End Source File
# Begin Source File

SOURCE=.\NTAgent3Dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\NTLogic.cpp
# End Source File
# Begin Source File

SOURCE=.\NTTRACE.cpp
# End Source File
# Begin Source File

SOURCE=.\Pack.cpp
# End Source File
# Begin Source File

SOURCE=.\PackXML.cpp
# End Source File
# Begin Source File

SOURCE=.\Prop0.cpp
# End Source File
# Begin Source File

SOURCE=.\Prop1.cpp
# End Source File
# Begin Source File

SOURCE=.\Prop2.cpp
# End Source File
# Begin Source File

SOURCE=.\Prop3.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\Subclass.cpp
# End Source File
# Begin Source File

SOURCE=.\SysStatus.cpp
# End Source File
# Begin Source File

SOURCE=.\TabSheet.cpp
# End Source File
# Begin Source File

SOURCE=.\TCPServer.cpp
# End Source File
# Begin Source File

SOURCE=.\TestDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\TrayIcon.cpp
# End Source File
# Begin Source File

SOURCE=.\UDPServer.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\BrowseDir.h
# End Source File
# Begin Source File

SOURCE=.\general.h
# End Source File
# Begin Source File

SOURCE=.\GetPCResours.h
# End Source File
# Begin Source File

SOURCE=.\LogicCommon.h
# End Source File
# Begin Source File

SOURCE=.\LogicRecvCtrl.h
# End Source File
# Begin Source File

SOURCE=.\LogicRecvDisp.h
# End Source File
# Begin Source File

SOURCE=.\LogicRecvFuns.h
# End Source File
# Begin Source File

SOURCE=.\MyDes.h
# End Source File
# Begin Source File

SOURCE=.\MyFTP.h
# End Source File
# Begin Source File

SOURCE=.\NTAgent3.h
# End Source File
# Begin Source File

SOURCE=.\NTAgent3Dlg.h
# End Source File
# Begin Source File

SOURCE=.\NTLogic.h
# End Source File
# Begin Source File

SOURCE=.\NTTRACE.h
# End Source File
# Begin Source File

SOURCE=.\NTXFS_ALL.H
# End Source File
# Begin Source File

SOURCE=.\Pack.h
# End Source File
# Begin Source File

SOURCE=.\PackXML.h
# End Source File
# Begin Source File

SOURCE=.\Prop0.h
# End Source File
# Begin Source File

SOURCE=.\Prop1.h
# End Source File
# Begin Source File

SOURCE=.\Prop2.h
# End Source File
# Begin Source File

SOURCE=.\Prop3.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\Subclass.h
# End Source File
# Begin Source File

SOURCE=.\SysStatus.h
# End Source File
# Begin Source File

SOURCE=.\TabSheet.h
# End Source File
# Begin Source File

SOURCE=.\TCPServer.h
# End Source File
# Begin Source File

SOURCE=.\TestDlg.h
# End Source File
# Begin Source File

SOURCE=.\TrayIcon.h
# End Source File
# Begin Source File

SOURCE=.\UDPServer.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\agent.ico
# End Source File
# Begin Source File

SOURCE=.\res\ebringlogo.bmp
# End Source File
# Begin Source File

SOURCE=.\res\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\res\NTAgent3.ico
# End Source File
# Begin Source File

SOURCE=.\res\NTAgent3.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
