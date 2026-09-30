@echo off
setlocal enabledelayedexpansion


cd /D %~dp0

for %%a in (%*) do set "%%a=1"
if not "%release%"=="1" set debug=1
if "%debug%"=="1"   set release=0 && echo [debug mode]
if "%release%"=="1" set debug=0 && echo [release mode]

set _build_=build
set _home_=..
set third_party=%_home_%\code\third_party

set cl_warnings=/W4 /wd4456 /wd4389 /wd4702 /wd4100 /wd4152 /wd4244 /wd4005 /wd4457 /wd4245 /wd4701 /wd4471 /wd4505


set cl_libs=/I%_home_%\code\ /I%third_party% /I%third_party%\googol_tech /I%third_party%\raylib /I%third_party%\raylib\external\glfw\include

set cl_common=/nologo /MT /EHsc /FC /Z7 /Oi /std:c++20 /Zc:strictStrings- /D_CRT_SECURE_NO_WARNINGS /DPLATFORM_DESKTOP %cl_warnings% %cl_libs%

if "%debug%"=="1" (
    set compile=call cl /Od /Ob1 /DBUILD_DEBUG=1 %cl_common%
) else (
    set compile=call cl /O2 /DBUILD_DEBUG=0 %cl_common%
)

set compile_link=/link /IGNORE:4099 /SUBSYSTEM:console /ENTRY:mainCRTStartup /NODEFAULTLIB:MSVCRT /NODEFAULTLIB:LIBCMTD /NODEFAULTLIB:libucrt.lib /DEFAULTLIB:ucrt.lib /incremental:no %third_party%\googol_tech\gts.lib %third_party%\googol_tech\ExtMdl.lib opengl32.lib gdi32.lib shell32.lib winmm.lib dwmapi.lib user32.lib advapi32.lib ole32.lib oleaut32.lib

::%third_party%\glfw-3.4\src\Release\glfw3.lib

if not exist "%_build_%" mkdir "%_build_%"

set raylib_src=%third_party%\raylib\rcore.c %third_party%\raylib\rshapes.c %third_party%\raylib\rtext.c %third_party%\raylib\rtextures.c %third_party%\raylib\rmodels.c %third_party%\raylib\raudio.c %third_party%\raylib\rglfw.c

pushd %_build_%
del *.pdb >nul 2>&1
%compile% %_home_%\code\samples\main.cpp %raylib_src% %compile_link% /out:main.exe || (popd && exit /b 1)

if not exist "gts.dll" copy /y "%third_party%\googol_tech\*.dll" . >nul
if not exist "GTS800.cfg" copy /y "%third_party%\googol_tech\GTS800.cfg" . >nul
popd
