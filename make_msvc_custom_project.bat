echo off 
echo make feature_se VS2015 project
if not defined VS140COMNTOOLS (
	echo vs2015 NOT FOUND.
	exit /B -1
)
echo vs2015 found.
where cmake
if errorlevel 1 (
	echo cmake NOT FOUND.
	exit /B -1
)
echo cmake found
set sh_folder=%~dp0
pushd %sh_folder%

rem 需要先编译feature_se
if exist project.custom.vs2015 rmdir project.custom.vs2015 /s/q
mkdir project.custom.vs2015
pushd project.custom.vs2015
if not defined VisualStudioVersion (
	echo make MSVC environment ...
	call "%VS140COMNTOOLS%..\..\vc/vcvarsall" x86_amd64
)
echo creating x86_64 Project for Visual Studio 2015 ...
cmake -G "Visual Studio 14 2015 Win64" -DCMAKE_INSTALL_PREFIX=..\release\fse_cas_windows_x86_64 .. ^
	-DEXT_SDK_TYPE=CUSTOM ^
	-DCUSTOM_FEACOMP_INCLUDE=J:\workspace.neon\cassdk54\FSFaceSDK\FSFaceSDK-windows-x86_64\include ^
	-DCUSTOM_FEACOMP_LIBRARY=J:\workspace.neon\cassdk54\FSFaceSDK\FSFaceSDK-windows-x86_64\lib\FSFaceSDK.lib ^
	-DCUSTOM_FEACOMP_HEADERS=FiStdDefEx.h,FSFaceSDK.h
	-DCUSTOM_FEACOMP_FUNNAME=EF_Compare

popd
popd