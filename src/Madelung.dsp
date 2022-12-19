# Microsoft Developer Studio Project File - Name="Madelung" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Madelung - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Madelung.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Madelung.mak" CFG="Madelung - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Madelung - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Madelung - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Madelung - Win32 Release"

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
# ADD CPP /nologo /MD /W3 /GX /O2 /I "D:\Projects\Madelung\Spg_dll" /I "D:\Projects\Madelung\UnitCelldll" /I "D:\Projects\Madelung\ewald" /I "D:\Projects\Madelung" /I "D:\Projects\Win32MTProgramming\Mcl4Mfc\INCLUDE" /I "D:\Projects\Win32MTProgramming\Mcl\Include" /I "D:\Projects\Madelung\Symmetry" /I "C:\Program Files\vtk31\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 vtkdll.lib Spg_dll.lib UnitCelldll.lib mcl.lib mcl4mfc.lib SolvOpt.lib Symmetry.lib /nologo /subsystem:windows /machine:I386 /nodefaultlib:"library" /libpath:"C:\Program Files\vtk31\lib" /libpath:"D:\Projects\Madelung\Spg_dll\Release" /libpath:"D:\Projects\Madelung\UnitCelldll\Release" /libpath:"D:\Projects\SolvOpt\Release" /libpath:"D:\Projects\Madelung\Symmetry\Release" /libpath:"D:\Projects\Win32MTProgramming\Mcl\Lib" /libpath:"D:\Projects\Win32MTProgramming\Mcl4Mfc\LIB"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "Madelung - Win32 Debug"

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
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "D:\Projects\Madelung\Spg_dll" /I "D:\Projects\Madelung\UnitCelldll" /I "D:\Projects\Madelung\ewald" /I "D:\Projects\Madelung" /I "D:\Projects\Win32MTProgramming\Mcl4Mfc\INCLUDE" /I "D:\Projects\Win32MTProgramming\Mcl\Include" /I "D:\Projects\Madelung\Symmetry" /I "C:\Program Files\vtk31\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 vtkdll.lib Spg_dll.lib UnitCelldll.lib mcld.lib mcl4mfcd.lib SolvOpt.lib Symmetry.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept /libpath:"C:\Program Files\vtk31\lib" /libpath:"D:\Projects\Madelung\Spg_dll\Debug" /libpath:"D:\Projects\Madelung\UnitCelldll\Debug" /libpath:"D:\Projects\SolvOpt\Debug" /libpath:"D:\Projects\Madelung\Symmetry\Debug" /libpath:"D:\Projects\Win32MTProgramming\Mcl\Lib" /libpath:"D:\Projects\Win32MTProgramming\Mcl4Mfc\LIB"

!ENDIF 

# Begin Target

# Name "Madelung - Win32 Release"
# Name "Madelung - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AtomProperties.cpp
# End Source File
# Begin Source File

SOURCE=.\AtomView.cpp
# End Source File
# Begin Source File

SOURCE=.\AtomWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\DPQ1.cpp
# End Source File
# Begin Source File

SOURCE=.\EditAtom.cpp
# End Source File
# Begin Source File

SOURCE=.\EditAtomsInMolecule.cpp
# End Source File
# Begin Source File

SOURCE=.\EditMolecule.cpp
# End Source File
# Begin Source File

SOURCE=.\Ewald\Ewald.cpp
# End Source File
# Begin Source File

SOURCE=.\ExtendLattice.cpp
# End Source File
# Begin Source File

SOURCE=.\ExtendLatticeSphere.cpp
# End Source File
# Begin Source File

SOURCE=.\InteractorStyleNorm.cpp
# End Source File
# Begin Source File

SOURCE=.\InteractorStyleSelect.cpp
# End Source File
# Begin Source File

SOURCE=.\Lattice.cpp
# End Source File
# Begin Source File

SOURCE=.\Madelung.cpp
# End Source File
# Begin Source File

SOURCE=.\hlp\Madelung.hpj

!IF  "$(CFG)" == "Madelung - Win32 Release"

# PROP Ignore_Default_Tool 1
USERDEP__MADEL="hlp\AfxCore.rtf"	"hlp\AfxPrint.rtf"	"hlp\$(TargetName).hm"	
# Begin Custom Build - Making help file...
OutDir=.\Release
TargetName=Madelung
InputPath=.\hlp\Madelung.hpj
InputName=Madelung

"$(OutDir)\$(InputName).hlp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	start /wait hcw /C /E /M "hlp\$(InputName).hpj" 
	if errorlevel 1 goto :Error 
	if not exist "hlp\$(InputName).hlp" goto :Error 
	copy "hlp\$(InputName).hlp" $(OutDir) 
	goto :done 
	:Error 
	echo hlp\$(InputName).hpj(1) : error: 
	type "hlp\$(InputName).log" 
	:done 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Madelung - Win32 Debug"

# PROP Ignore_Default_Tool 1
USERDEP__MADEL="hlp\AfxCore.rtf"	"hlp\AfxPrint.rtf"	"hlp\$(TargetName).hm"	
# Begin Custom Build - Making help file...
OutDir=.\Debug
TargetName=Madelung
InputPath=.\hlp\Madelung.hpj
InputName=Madelung

"$(OutDir)\$(InputName).hlp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	start /wait hcw /C /E /M "hlp\$(InputName).hpj" 
	if errorlevel 1 goto :Error 
	if not exist "hlp\$(InputName).hlp" goto :Error 
	copy "hlp\$(InputName).hlp" $(OutDir) 
	goto :done 
	:Error 
	echo hlp\$(InputName).hpj(1) : error: 
	type "hlp\$(InputName).log" 
	:done 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Madelung.rc
# End Source File
# Begin Source File

SOURCE=.\MadelungDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\MadelungView.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\Molecule.cpp
# End Source File
# Begin Source File

SOURCE=.\NumberOfUnitCellsToPlot.cpp
# End Source File
# Begin Source File

SOURCE=.\PlotAnAtom.cpp
# End Source File
# Begin Source File

SOURCE=.\PointCharges.cpp
# End Source File
# Begin Source File

SOURCE=.\PointGroupIrreps.cpp
# End Source File
# Begin Source File

SOURCE=.\PropertyPageAtomsInUnitCell.cpp
# End Source File
# Begin Source File

SOURCE=.\PropertyPageMoleculesInUnitCell.cpp
# End Source File
# Begin Source File

SOURCE=.\PropertyPageSpaceGroup.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\StringArray.cpp
# End Source File
# Begin Source File

SOURCE=.\SymmetryDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\TextWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\TextWindowThread.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AtomProperties.h
# End Source File
# Begin Source File

SOURCE=.\AtomView.h
# End Source File
# Begin Source File

SOURCE=.\AtomWindow.h
# End Source File
# Begin Source File

SOURCE=.\DPQ1.H
# End Source File
# Begin Source File

SOURCE=.\EditAtom.h
# End Source File
# Begin Source File

SOURCE=.\EditAtomsInMolecule.h
# End Source File
# Begin Source File

SOURCE=.\EditMolecule.h
# End Source File
# Begin Source File

SOURCE=.\EnumTypes.h
# End Source File
# Begin Source File

SOURCE=.\ExtendLattice.h
# End Source File
# Begin Source File

SOURCE=.\ExtendLatticeSphere.h
# End Source File
# Begin Source File

SOURCE=.\InteractorStyleNorm.h
# End Source File
# Begin Source File

SOURCE=.\InteractorStyleSelect.h
# End Source File
# Begin Source File

SOURCE=.\Lattice.h
# End Source File
# Begin Source File

SOURCE=.\Madelung.h
# End Source File
# Begin Source File

SOURCE=.\MadelungDoc.h
# End Source File
# Begin Source File

SOURCE=.\MadelungView.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\Molecule.h
# End Source File
# Begin Source File

SOURCE=.\NumberOfUnitCellsToPlot.h
# End Source File
# Begin Source File

SOURCE=.\PlotAnAtom.h
# End Source File
# Begin Source File

SOURCE=.\PointGroupIrreps.h
# End Source File
# Begin Source File

SOURCE=.\PropertyPageAtomsInUnitCell.h
# End Source File
# Begin Source File

SOURCE=.\PropertyPageMoleculesInUnitCell.h
# End Source File
# Begin Source File

SOURCE=.\PropertyPageSpaceGroup.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h

!IF  "$(CFG)" == "Madelung - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Making help include file...
TargetName=Madelung
InputPath=.\Resource.h

"hlp\$(TargetName).hm" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	echo. >"hlp\$(TargetName).hm" 
	echo // Commands (ID_* and IDM_*) >>"hlp\$(TargetName).hm" 
	makehm ID_,HID_,0x10000 IDM_,HIDM_,0x10000 resource.h >>"hlp\$(TargetName).hm" 
	echo. >>"hlp\$(TargetName).hm" 
	echo // Prompts (IDP_*) >>"hlp\$(TargetName).hm" 
	makehm IDP_,HIDP_,0x30000 resource.h >>"hlp\$(TargetName).hm" 
	echo. >>"hlp\$(TargetName).hm" 
	echo // Resources (IDR_*) >>"hlp\$(TargetName).hm" 
	makehm IDR_,HIDR_,0x20000 resource.h >>"hlp\$(TargetName).hm" 
	echo. >>"hlp\$(TargetName).hm" 
	echo // Dialogs (IDD_*) >>"hlp\$(TargetName).hm" 
	makehm IDD_,HIDD_,0x20000 resource.h >>"hlp\$(TargetName).hm" 
	echo. >>"hlp\$(TargetName).hm" 
	echo // Frame Controls (IDW_*) >>"hlp\$(TargetName).hm" 
	makehm IDW_,HIDW_,0x50000 resource.h >>"hlp\$(TargetName).hm" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Madelung - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Making help include file...
TargetName=Madelung
InputPath=.\Resource.h

"hlp\$(TargetName).hm" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	echo. >"hlp\$(TargetName).hm" 
	echo // Commands (ID_* and IDM_*) >>"hlp\$(TargetName).hm" 
	makehm ID_,HID_,0x10000 IDM_,HIDM_,0x10000 resource.h >>"hlp\$(TargetName).hm" 
	echo. >>"hlp\$(TargetName).hm" 
	echo // Prompts (IDP_*) >>"hlp\$(TargetName).hm" 
	makehm IDP_,HIDP_,0x30000 resource.h >>"hlp\$(TargetName).hm" 
	echo. >>"hlp\$(TargetName).hm" 
	echo // Resources (IDR_*) >>"hlp\$(TargetName).hm" 
	makehm IDR_,HIDR_,0x20000 resource.h >>"hlp\$(TargetName).hm" 
	echo. >>"hlp\$(TargetName).hm" 
	echo // Dialogs (IDD_*) >>"hlp\$(TargetName).hm" 
	makehm IDD_,HIDD_,0x20000 resource.h >>"hlp\$(TargetName).hm" 
	echo. >>"hlp\$(TargetName).hm" 
	echo // Frame Controls (IDW_*) >>"hlp\$(TargetName).hm" 
	makehm IDW_,HIDW_,0x50000 resource.h >>"hlp\$(TargetName).hm" 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\StringArray.h
# End Source File
# Begin Source File

SOURCE=.\SymmetryDialog.h
# End Source File
# Begin Source File

SOURCE=.\TextWindow.h
# End Source File
# Begin Source File

SOURCE=.\TextWindowThread.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\ico00001.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00002.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00003.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00004.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00005.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00006.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00007.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00008.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00009.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00010.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00011.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00012.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00013.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00014.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00015.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00016.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00017.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00018.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00019.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00020.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00021.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00022.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00023.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00024.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00025.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00026.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00027.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00028.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00029.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00030.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00031.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00032.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00033.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00034.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00035.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00036.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00037.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00038.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00039.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00040.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00041.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00042.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00043.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00044.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00045.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00046.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00047.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00048.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00049.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00050.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00051.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00052.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00053.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00054.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00055.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00056.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00057.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00058.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00059.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00060.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00061.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00062.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00063.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00064.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00065.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00066.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00067.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00068.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00069.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00070.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00071.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00072.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00073.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00074.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00075.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon2.ico
# End Source File
# Begin Source File

SOURCE=.\res\Madelung.ico
# End Source File
# Begin Source File

SOURCE=.\res\Madelung.rc2
# End Source File
# Begin Source File

SOURCE=.\res\MadelungDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# End Group
# Begin Group "Help Files"

# PROP Default_Filter "cnt;rtf"
# Begin Source File

SOURCE=.\hlp\AfxCore.rtf
# End Source File
# Begin Source File

SOURCE=.\hlp\AfxPrint.rtf
# End Source File
# Begin Source File

SOURCE=.\hlp\AppExit.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\Bullet.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\CurArw2.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\CurArw4.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\CurHelp.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\EditCopy.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\EditCut.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\EditPast.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\EditUndo.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\FileNew.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\FileOpen.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\FilePrnt.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\FileSave.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\HlpSBar.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\HlpTBar.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\Madelung.cnt

!IF  "$(CFG)" == "Madelung - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Copying contents file...
OutDir=.\Release
InputPath=.\hlp\Madelung.cnt
InputName=Madelung

"$(OutDir)\$(InputName).cnt" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy "hlp\$(InputName).cnt" $(OutDir)

# End Custom Build

!ELSEIF  "$(CFG)" == "Madelung - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Copying contents file...
OutDir=.\Debug
InputPath=.\hlp\Madelung.cnt
InputName=Madelung

"$(OutDir)\$(InputName).cnt" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy "hlp\$(InputName).cnt" $(OutDir)

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\hlp\RecFirst.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\RecLast.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\RecNext.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\RecPrev.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\Scmax.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\ScMenu.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\Scmin.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\backup.txt
# End Source File
# Begin Source File

SOURCE=.\HowToAddSpaceGroup.txt
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
