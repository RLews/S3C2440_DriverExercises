@echo off
REM Setup for compiling with Watcom C/C++ 10.6 in 32 bit mode

SET LIB=%WC10_PATH%\LIB386;%WC10_PATH%\LIB386\NT;.
SET EDPATH=%WC10_PATH%\EDDAT
SET INCLUDE=%WC10_PATH%\H;%WC10_PATH%\H\NT;
SET WATCOM=%WC10_PATH%
SET MAKESTARTUP=%SCITECH%\MAKEDEFS\WC32.MK
SET USE_TNT=
SET USE_X32=
SET USE_X32VM=
SET USE_WIN16=
SET USE_WIN32=1
SET USE_WIN386=
SET WIN32_GUI=1
SET USE_OS216=
SET USE_OS232=
SET USE_OS2GUI=
SET USE_SNAP=
SET USE_QNX4=
SET WC_LIBBASE=WC10
PATH %SCITECH_BIN%;%WC10_PATH%\BINNT;%WC10_PATH%\BINW;%DEFPATH%%WC_CD_PATH%

echo Watcom C/C++ 10.6 X11 compilation environment set up
