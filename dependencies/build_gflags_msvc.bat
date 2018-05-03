@rem 根据%%1参数 编译DEBUG|RELEASE版本,默认RELEASE
@echo off 
echo build gflags by VS2015
if not defined VS140COMNTOOLS (
	echo vs2015 NOT FOUND.
	exit /B -1
)
echo vs2015 found.
where cmake
if errorlevel 1 (
	echo cmake NOT FOUND.
	echo download from http://cmake.org/ ,extract to disk 
	echo add installation path to environment variable PATH
	exit /B -1
)
echo cmake found.
set sh_folder=%~dp0
rem 定义编译的版本类型(DEBUG|RELEASE)
set build_type=RELEASE
rem 如果输入参数1为"DEBUG"(不区分大小写)则编译DEBUG版本
if /I "%1" == "DEBUG" ( set build_type=DEBUG) 
echo build_type=%build_type%

set source_folder=gflags-2.2.1
if not exist %source_folder% (
	echo not found source folder: %source_folder%,please unzip %source_folder%.zip in current folder
	exit /B -1
	)
pushd %sh_folder%\%source_folder%

rem 默认只编译64位版本
rem call :msvc_x86
call :msvc_x86_64
popd
goto :eof

:msvc_x86
echo build x86 
echo cd %cd%
if exist build_msvc rmdir build_msvc /s/q
mkdir build_msvc
cd build_msvc
echo make MSVC x86 environment ...

call "%VS140COMNTOOLS%..\..\vc/vcvarsall" x86

cmake -G "NMake Makefiles" ^
	-DCMAKE_BUILD_TYPE=%build_type% ^
	-DCMAKE_DEBUG_POSTFIX=_d ^
  -DBUILD_SHARED_LIBS=OFF ^
  -DBUILD_STATIC_LIBS=ON ^
  -DBUILD_gflags_LIB=ON ^
	-DCMAKE_INSTALL_PREFIX=%sh_folder%/gflags-windows-vc-x86-mt ^
	-DWITH_STATIC_CRT=ON ^
	-DWITH_DEBUG_Z7=ON ^
	-DCMAKE_USER_MAKE_RULES_OVERRIDE=%sh_folder%\..\cmake\compiler_flag_overrides.cmake ^
	..
nmake install
del * /s/q
cmake -G "NMake Makefiles" ^
	-DCMAKE_BUILD_TYPE=%build_type% ^
	-DCMAKE_DEBUG_POSTFIX=_d ^
  -DBUILD_SHARED_LIBS=OFF ^
  -DBUILD_STATIC_LIBS=ON ^
  -DBUILD_gflags_LIB=ON ^
	-DCMAKE_INSTALL_PREFIX=%sh_folder%/gflags-windows-vc-x86 ^
	-DWITH_DEBUG_Z7=ON ^
	-DCMAKE_USER_MAKE_RULES_OVERRIDE=%sh_folder%\..\cmake\compiler_flag_overrides.cmake ^
	..
nmake clean install
cd ..
rmdir build_msvc/s/q
goto :eof

:msvc_x86_64
echo build x86_64 
if exist build_msvc rmdir build_msvc /s/q
mkdir build_msvc
cd build_msvc
echo make MSVC amd64 environment ...
call "%VS140COMNTOOLS%..\..\vc/vcvarsall" x86_amd64

cmake -G "NMake Makefiles" ^
	-DCMAKE_BUILD_TYPE=%build_type% ^
	-DCMAKE_DEBUG_POSTFIX=_d ^
  -DBUILD_SHARED_LIBS=OFF ^
  -DBUILD_STATIC_LIBS=ON ^
  -DBUILD_gflags_LIB=ON ^
	-DCMAKE_INSTALL_PREFIX=%sh_folder%/gflags-windows-vc-x86_64-mt ^
	-DWITH_STATIC_CRT=ON ^
	-DWITH_DEBUG_Z7=ON ^
	-DCMAKE_USER_MAKE_RULES_OVERRIDE=%sh_folder%\..\cmake\compiler_flag_overrides.cmake ^
	..
nmake install
del * /s/q
cmake -G "NMake Makefiles" ^
	-DCMAKE_BUILD_TYPE=%build_type% ^
	-DCMAKE_DEBUG_POSTFIX=_d ^
  -DBUILD_SHARED_LIBS=OFF ^
  -DBUILD_STATIC_LIBS=ON ^
  -DBUILD_gflags_LIB=ON ^
	-DCMAKE_INSTALL_PREFIX=%sh_folder%/gflags-windows-vc-x86_64 ^
	-DWITH_DEBUG_Z7=ON ^
	-DCMAKE_USER_MAKE_RULES_OVERRIDE=%sh_folder%\..\cmake\compiler_flag_overrides.cmake ^
	..
nmake clean install
cd ..

rmdir build_msvc/s/q
goto :eof
