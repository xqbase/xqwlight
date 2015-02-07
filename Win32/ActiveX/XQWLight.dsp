# Microsoft Developer Studio Project File - Name="XQWLight" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=XQWLight - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "XQWLight.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "XQWLight.mak" CFG="XQWLight - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "XQWLight - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "XQWLight - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "XQWLight - Win32 Release"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Ext "ocx"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Ext "ocx"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 WINMM.LIB /nologo /subsystem:windows /dll /machine:I386
# Begin Custom Build - Registering ActiveX Control...
OutDir=.\Release
TargetPath=.\Release\XQWLight.ocx
InputPath=.\Release\XQWLight.ocx
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "XQWLight - Win32 Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Ext "ocx"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Ext "ocx"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 WINMM.LIB /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# Begin Custom Build - Registering ActiveX Control...
OutDir=.\Debug
TargetPath=.\Debug\XQWLight.ocx
InputPath=.\Debug\XQWLight.ocx
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ENDIF 

# Begin Target

# Name "XQWLight - Win32 Release"
# Name "XQWLight - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\XQWLight.cpp
# End Source File
# Begin Source File

SOURCE=.\XQWLight.def
# End Source File
# Begin Source File

SOURCE=.\XQWLight.odl
# End Source File
# Begin Source File

SOURCE=.\XQWLight.rc
# End Source File
# Begin Source File

SOURCE=.\XQWLightCtl.cpp
# End Source File
# Begin Source File

SOURCE=.\XQWLightPpg.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\XQWLight.h
# End Source File
# Begin Source File

SOURCE=.\XQWLightCtl.h
# End Source File
# Begin Source File

SOURCE=.\XQWLightPpg.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=..\RES\BA.BMP
# End Source File
# Begin Source File

SOURCE=..\RES\BB.BMP
# End Source File
# Begin Source File

SOURCE=..\RES\BC.BMP
# End Source File
# Begin Source File

SOURCE=..\RES\BK.BMP
# End Source File
# Begin Source File

SOURCE=..\RES\BN.BMP
# End Source File
# Begin Source File

SOURCE=..\RES\BOARD.BMP
# End Source File
# Begin Source File

SOURCE=..\BOOK.DAT
# End Source File
# Begin Source File

SOURCE=..\RES\BP.BMP
# End Source File
# Begin Source File

SOURCE=..\RES\BR.BMP
# End Source File
# Begin Source File

SOURCE=..\RES\CAPTURE.WAV
# End Source File
# Begin Source File

SOURCE=..\RES\CAPTURE2.WAV
# End Source File
# Begin Source File

SOURCE=..\RES\CHECK.WAV
# End Source File
# Begin Source File

SOURCE=..\RES\CHECK2.WAV
# End Source File
# Begin Source File

SOURCE=..\RES\CLICK.WAV
# End Source File
# Begin Source File

SOURCE=..\RES\DRAW.WAV
# End Source File
# Begin Source File

SOURCE=..\RES\ILLEGAL.WAV
# End Source File
# Begin Source File

SOURCE=..\RES\LOSS.WAV
# End Source File
# Begin Source File

SOURCE=..\RES\MOVE.WAV
# End Source File
# Begin Source File

SOURCE=..\RES\MOVE2.WAV
# End Source File
# Begin Source File

SOURCE=..\RES\RA.BMP
# End Source File
# Begin Source File

SOURCE=..\RES\RB.BMP
# End Source File
# Begin Source File

SOURCE=..\RES\RC.BMP
# End Source File
# Begin Source File

SOURCE=..\RES\RK.BMP
# End Source File
# Begin Source File

SOURCE=..\RES\RN.BMP
# End Source File
# Begin Source File

SOURCE=..\RES\RP.BMP
# End Source File
# Begin Source File

SOURCE=..\RES\RR.BMP
# End Source File
# Begin Source File

SOURCE=..\RES\SELECTED.BMP
# End Source File
# Begin Source File

SOURCE=..\RES\WIN.WAV
# End Source File
# Begin Source File

SOURCE=..\RES\XQWIZARD.ICO
# End Source File
# Begin Source File

SOURCE=.\XQWLightCtl.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
