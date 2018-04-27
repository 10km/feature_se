echo off 
echo build feature_se by VS2015
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

set sh_folder=%~dp0
rem 定义编译的版本类型(DEBUG|RELEASE)
set build_type=RELEASE
rem 如果输入参数1为"DEBUG"(不区分大小写)则编译DEBUG版本
if /I "%1" == "DEBUG" ( set build_type=DEBUG) 
echo build_type=%build_type%

pushd %sh_folder%

:msvc_x86_64
echo build x86_64 
if exist build_msvc rmdir build_msvc /s/q
mkdir build_msvc
pushd build_msvc
if not defined VisualStudioVersion (
	echo make MSVC environment ...
	call "%VS140COMNTOOLS%..\..\vc/vcvarsall" x86_amd64
)
echo creating x86_64 Project for Visual Studio 2015 ...
cmake -G "NMake Makefiles" -DCMAKE_BUILD_TYPE=%build_type% ^
	-DCMAKE_INSTALL_PREFIX=%sh_folder%release\fse_windows_x86_64 ..
nmake install
rem cmake -G "Visual Studio 14 2015 Win64" -DCMAKE_INSTALL_PREFIX=%sh_folder%release\fse_windows_x86_64 .. 
rem devenv feature_se.sln /Build "%build_type%|X64" /Project INSTALL
popd

rmdir build_msvc/s/q

popd