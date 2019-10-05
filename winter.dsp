# Microsoft Developer Studio Project File - Name="winter" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=winter - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "winter.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "winter.mak" CFG="winter - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "winter - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "winter - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "winter - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "winter - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "winter___Win32_Debug"
# PROP BASE Intermediate_Dir "winter___Win32_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "winter___Win32_Debug"
# PROP Intermediate_Dir "winter___Win32_Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "winter - Win32 Release"
# Name "winter - Win32 Debug"
# Begin Group "Core"

# PROP Default_Filter ""
# Begin Group "Box"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Box.cpp
# End Source File
# Begin Source File

SOURCE=.\Box.h
# End Source File
# End Group
# Begin Group "Card"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Card.cpp
# End Source File
# Begin Source File

SOURCE=.\Card.h
# End Source File
# End Group
# Begin Group "Play"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\play.cpp
# End Source File
# Begin Source File

SOURCE=.\play.h
# End Source File
# End Group
# Begin Group "Player"

# PROP Default_Filter ""
# Begin Group "AsdxPlayer"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\asdxplayer.cpp
# End Source File
# Begin Source File

SOURCE=.\asdxPlayer.h
# End Source File
# End Group
# Begin Group "ComputerPlayer"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ComputerPlayer.cpp
# End Source File
# Begin Source File

SOURCE=.\ComputerPlayer.h
# End Source File
# End Group
# Begin Group "LocalPlayer"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\LocalPlayer.cpp
# End Source File
# Begin Source File

SOURCE=.\LocalPlayer.h
# End Source File
# End Group
# Begin Group "NetworkPlayer"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\networkplayer.cpp
# End Source File
# Begin Source File

SOURCE=.\NetworkPlayer.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\Player.cpp
# End Source File
# Begin Source File

SOURCE=.\Player.h
# End Source File
# End Group
# Begin Group "Checker"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Checker.cpp
# End Source File
# Begin Source File

SOURCE=.\Checker.h
# End Source File
# End Group
# End Group
# Begin Group "Library"

# PROP Default_Filter ""
# Begin Group "DirectX"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\DirectDraw.cpp
# End Source File
# Begin Source File

SOURCE=.\DirectDraw.h
# End Source File
# Begin Source File

SOURCE=.\Sound.cpp
# End Source File
# Begin Source File

SOURCE=.\Sound.h
# End Source File
# Begin Source File

SOURCE=.\Sprite.cpp
# End Source File
# Begin Source File

SOURCE=.\Sprite.h
# End Source File
# End Group
# Begin Group "IP"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\IP.cpp
# End Source File
# Begin Source File

SOURCE=.\IP.h
# End Source File
# End Group
# Begin Group "MsgQueue"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\MsgQueue.cpp
# End Source File
# Begin Source File

SOURCE=.\MsgQueue.h
# End Source File
# End Group
# Begin Group "Random"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\random.cpp
# End Source File
# Begin Source File

SOURCE=.\Random.h
# End Source File
# End Group
# Begin Group "Util"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\util.cpp
# End Source File
# Begin Source File

SOURCE=.\util.h
# End Source File
# End Group
# Begin Group "Tree"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\SimpleTree.cpp
# End Source File
# Begin Source File

SOURCE=.\SimpleTree.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\ISO.h
# End Source File
# End Group
# Begin Group "External Library"

# PROP Default_Filter "*.lib"
# Begin Source File

SOURCE="..\..\Program Files\Microsoft Visual Studio\VC98\Lib\DXGUID.LIB"
# End Source File
# Begin Source File

SOURCE="..\..\Program Files\Microsoft Visual Studio\VC98\Lib\DSOUND.LIB"
# End Source File
# Begin Source File

SOURCE="..\..\Program Files\Microsoft Visual Studio\VC98\Lib\DDRAW.LIB"
# End Source File
# Begin Source File

SOURCE="..\..\Program Files\Microsoft Visual Studio\VC98\Lib\WINMM.LIB"
# End Source File
# Begin Source File

SOURCE="..\..\Program Files\Microsoft Visual Studio\VC98\Lib\WS2_32.LIB"
# End Source File
# End Group
# Begin Group "System"

# PROP Default_Filter ""
# Begin Group "Client"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Client.cpp
# End Source File
# Begin Source File

SOURCE=.\Client.h
# End Source File
# End Group
# Begin Group "Server"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Server.cpp
# End Source File
# Begin Source File

SOURCE=.\Server.h
# End Source File
# End Group
# Begin Group "Menu"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Menu.cpp
# End Source File
# Begin Source File

SOURCE=.\Menu.h
# End Source File
# End Group
# Begin Group "Game"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\game.cpp
# End Source File
# Begin Source File

SOURCE=.\game.h
# End Source File
# End Group
# Begin Group "Terminal"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Terminal.cpp
# End Source File
# Begin Source File

SOURCE=.\Terminal.h
# End Source File
# End Group
# Begin Group "Unit"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\unit.cpp
# End Source File
# Begin Source File

SOURCE=.\Unit.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\Main.cpp
# End Source File
# Begin Source File

SOURCE=.\protocol.h
# End Source File
# End Group
# Begin Group "Resource"

# PROP Default_Filter ""
# Begin Group "Menu Resource"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Resource\menu.BMP
# End Source File
# End Group
# Begin Group "Server_Client Resource"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Resource\back.bmp
# End Source File
# Begin Source File

SOURCE=.\Resource\o.bmp
# End Source File
# Begin Source File

SOURCE=.\Resource\x.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\resource.rc
# End Source File
# End Group
# Begin Group "Document"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\programming_style.txt
# End Source File
# Begin Source File

SOURCE=.\windows_socket_programming.txt
# End Source File
# Begin Source File

SOURCE=.\winter.txt
# End Source File
# End Group
# End Target
# End Project
