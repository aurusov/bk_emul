# Microsoft Developer Studio Project File - Name="bk_emul" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=bk_emul - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "bk_emul.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "bk_emul.mak" CFG="bk_emul - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "bk_emul - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "bk_emul - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "bk_emul - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x419 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 winmm.lib dxguid.lib dxerr8.lib ddraw.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib uuid.lib Version.lib /nologo /subsystem:windows /machine:I386
# SUBTRACT LINK32 /profile

!ELSEIF  "$(CFG)" == "bk_emul - Win32 Debug"

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
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x419 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 winmm.lib dxguid.lib dxerr8.lib ddraw.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib uuid.lib Version.lib /nologo /subsystem:windows /debug /machine:I386
# SUBTRACT LINK32 /profile

!ENDIF 

# Begin Target

# Name "bk_emul - Win32 Release"
# Name "bk_emul - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\bkemul.cpp
# End Source File
# Begin Source File

SOURCE=.\bkemulabout.cpp
# End Source File
# Begin Source File

SOURCE=.\bkemulapp.cpp
# End Source File
# Begin Source File

SOURCE=.\bkemulcpu.cpp
# End Source File
# Begin Source File

SOURCE=.\bkemulkeyboard.cpp
# End Source File
# Begin Source File

SOURCE=.\bkemulmainfrm.cpp
# End Source File
# Begin Source File

SOURCE=.\bkemulmemory.cpp
# End Source File
# Begin Source File

SOURCE=.\bkemulspeaker.cpp
# End Source File
# Begin Source File

SOURCE=.\bkemultaperecorder.cpp
# End Source File
# Begin Source File

SOURCE=.\bkemultimer.cpp
# End Source File
# Begin Source File

SOURCE=.\bkemulvideo.cpp
# End Source File
# Begin Source File

SOURCE=.\ddutil.cpp

!IF  "$(CFG)" == "bk_emul - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "bk_emul - Win32 Debug"

# ADD CPP /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\dxutil.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\rdolink.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\bkemul.h
# End Source File
# Begin Source File

SOURCE=.\bkemulabout.h
# End Source File
# Begin Source File

SOURCE=.\bkemulapp.h
# End Source File
# Begin Source File

SOURCE=.\bkemulcpu.h
# End Source File
# Begin Source File

SOURCE=.\bkemulkeyboard.h
# End Source File
# Begin Source File

SOURCE=.\bkemulmainfrm.h
# End Source File
# Begin Source File

SOURCE=.\bkemulmemory.h
# End Source File
# Begin Source File

SOURCE=.\bkemulspeaker.h
# End Source File
# Begin Source File

SOURCE=.\bkemultaperecorder.h
# End Source File
# Begin Source File

SOURCE=.\bkemultimer.h
# End Source File
# Begin Source File

SOURCE=.\bkemulvideo.h
# End Source File
# Begin Source File

SOURCE=.\ddutil.h
# End Source File
# Begin Source File

SOURCE=.\dxutil.h
# End Source File
# Begin Source File

SOURCE=.\rdolink.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\bk_emul.ico
# End Source File
# Begin Source File

SOURCE=.\bk_emul.rc
# End Source File
# Begin Source File

SOURCE=.\res\bk_emul.rc2
# End Source File
# Begin Source File

SOURCE=.\res\font_atlantic.bin
# End Source File
# Begin Source File

SOURCE=.\res\font_big.bin
# End Source File
# Begin Source File

SOURCE=.\res\font_bk11.bin
# End Source File
# Begin Source File

SOURCE=.\res\font_clock.bin
# End Source File
# Begin Source File

SOURCE=.\res\font_color1.bin
# End Source File
# Begin Source File

SOURCE=.\res\font_color2.bin
# End Source File
# Begin Source File

SOURCE=.\res\font_comp.bin
# End Source File
# Begin Source File

SOURCE=.\res\font_cursiv.bin
# End Source File
# Begin Source File

SOURCE=.\res\font_default.bin
# End Source File
# Begin Source File

SOURCE=.\res\font_future.bin
# End Source File
# Begin Source File

SOURCE=.\res\font_gothic.bin
# End Source File
# Begin Source File

SOURCE=.\res\font_ibm.bin
# End Source File
# Begin Source File

SOURCE=.\res\font_modern.bin
# End Source File
# Begin Source File

SOURCE=.\res\font_pcfont.bin
# End Source File
# Begin Source File

SOURCE=.\res\font_prs.bin
# End Source File
# Begin Source File

SOURCE=.\res\font_prs2.bin
# End Source File
# Begin Source File

SOURCE=.\res\font_prs3.bin
# End Source File
# Begin Source File

SOURCE=.\res\font_reader.bin
# End Source File
# Begin Source File

SOURCE=.\res\font_rtv21.bin
# End Source File
# Begin Source File

SOURCE=.\res\font_script.bin
# End Source File
# Begin Source File

SOURCE=.\res\font_shrift1.bin
# End Source File
# Begin Source File

SOURCE=.\res\font_shrift2.bin
# End Source File
# Begin Source File

SOURCE=.\res\font_super.bin
# End Source File
# Begin Source File

SOURCE=.\res\font_thick.bin
# End Source File
# Begin Source File

SOURCE=.\res\font_works.bin
# End Source File
# Begin Source File

SOURCE=.\res\rom_basic_10_120.bin
# End Source File
# Begin Source File

SOURCE=.\res\rom_basic_10_140.bin
# End Source File
# Begin Source File

SOURCE=.\res\rom_basic_10_160.bin
# End Source File
# Begin Source File

SOURCE=.\res\rom_mirage120.bin
# End Source File
# Begin Source File

SOURCE=.\res\rom_monitor_10.bin
# End Source File
# End Group
# End Target
# End Project
