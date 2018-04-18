echo off 
echo build test by VS2015
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
if exist build rmdir project.vs2015 /s/q
mkdir project.vs2015
cd project.vs2015
if not defined VisualStudioVersion (
	call "%VS140COMNTOOLS%..\..\vc/vcvarsall" x86_amd64
)
cmake -G "Visual Studio 14 2015 Win64" -DCMAKE_INSTALL_PREFIX=..\release\fse_windows_x86_64 .. 