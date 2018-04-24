echo off 
echo make test VS2015 project
if not defined VS140COMNTOOLS (
	echo vs2015 NOT FOUND.
	exit -1
)
echo vs2015 found.
where cmake
if errorlevel 1 (
	echo cmake NOT FOUND.
	exit -1
)
echo cmake found
rem 需要先编译feature_se
if exist project.vs2015 rmdir project.vs2015 /s/q
mkdir project.vs2015
cd project.vs2015
if not defined VisualStudioVersion (
	echo make MSVC environment ...
	call "%VS140COMNTOOLS%..\..\vc/vcvarsall" x86_amd64
)
echo creating x86_64 Project for Visual Studio 2015 ...
cmake -G "Visual Studio 14 2015 Win64" -DFSE_ROOT_DIR=..\..\release\fse_windows_x86_64 .. 