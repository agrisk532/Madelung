@echo off
REM -- First make map file from Microsoft Visual C++ generated resource.h
echo // MAKEHELP.BAT generated Help Map file.  Used by MADELUNG.HPJ. >"hlp\Madelung.hm"
echo. >>"hlp\Madelung.hm"
echo // Commands (ID_* and IDM_*) >>"hlp\Madelung.hm"
makehm ID_,HID_,0x10000 IDM_,HIDM_,0x10000 resource.h >>"hlp\Madelung.hm"
echo. >>"hlp\Madelung.hm"
echo // Prompts (IDP_*) >>"hlp\Madelung.hm"
makehm IDP_,HIDP_,0x30000 resource.h >>"hlp\Madelung.hm"
echo. >>"hlp\Madelung.hm"
echo // Resources (IDR_*) >>"hlp\Madelung.hm"
makehm IDR_,HIDR_,0x20000 resource.h >>"hlp\Madelung.hm"
echo. >>"hlp\Madelung.hm"
echo // Dialogs (IDD_*) >>"hlp\Madelung.hm"
makehm IDD_,HIDD_,0x20000 resource.h >>"hlp\Madelung.hm"
echo. >>"hlp\Madelung.hm"
echo // Frame Controls (IDW_*) >>"hlp\Madelung.hm"
makehm IDW_,HIDW_,0x50000 resource.h >>"hlp\Madelung.hm"
REM -- Make help for Project MADELUNG


echo Building Win32 Help files
start /wait hcw /C /E /M "hlp\Madelung.hpj"
if errorlevel 1 goto :Error
if not exist "hlp\Madelung.hlp" goto :Error
if not exist "hlp\Madelung.cnt" goto :Error
echo.
if exist Debug\nul copy "hlp\Madelung.hlp" Debug
if exist Debug\nul copy "hlp\Madelung.cnt" Debug
if exist Release\nul copy "hlp\Madelung.hlp" Release
if exist Release\nul copy "hlp\Madelung.cnt" Release
echo.
goto :done

:Error
echo hlp\Madelung.hpj(1) : error: Problem encountered creating help file

:done
echo.
