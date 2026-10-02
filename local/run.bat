@echo off
setlocal

if "%_project_name_%"=="" set "_project_name_=main"
if "%_build_%"=="" set "_build_=%~dp0..\build"

pushd "%_build_%"

echo.
echo Running: %_project_name_%.exe
echo Working directory: %CD%
echo.

"%_project_name_%.exe"

popd

endlocal