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
:msvc_x86_64
echo build x86_64 
if not "%1" == "" (set install_folder="%~f1") else (set install_folder="%cd%\release\fse_windows_x86_64") 
echo install_folder=%install_folder%
if exist build_msvc rmdir build_msvc /s/q
mkdir build_msvc
cd build_msvc
if not defined VisualStudioVersion (
	call "%VS140COMNTOOLS%..\..\vc/vcvarsall" x86_amd64
)
cmake -G "NMake Makefiles" -DCMAKE_BUILD_TYPE=RELEASE -DCMAKE_INSTALL_PREFIX=%install_folder% ..
nmake install
cd ..

rmdir build_msvc/s/q