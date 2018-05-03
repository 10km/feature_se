@rem 根据%%1参数 编译DEBUG|RELEASE版本,默认RELEASE
@echo off 
echo build glog by MinGW
where gcc
if errorlevel 1 (
	echo MinGW/gcc NOT FOUND.
	exit /B -1
)
echo MinGW/gcc found.
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

set source_folder=glog-0.3.5
if not exist %source_folder% (
	echo not found source folder: %source_folder%,please unzip %source_folder%.zip in current folder
	exit /B -1
	)
pushd %sh_folder%\%source_folder%
rem 判断是否能编译64位程序
gcc --version |findstr "sjlj seh"
if errorlevel 1 (
	echo unsupported x86_64 build
	)else call:gcc_x86_64
	
rem 判断是否能编译32位程序
gcc --version |findstr "sjlj dwarf"
if errorlevel 1 (
	echo unsupported x86 build	
	)else call:gcc_x86

popd
goto :eof
:gcc_x86
echo build x86 use MinGW 
if exist build_gcc_x86 rmdir build_gcc_x86 /s/q
mkdir build_gcc_x86
pushd build_gcc_x86
rem gcc SJLJ or DWARF distribution required

cmake -G "MinGW Makefiles" ^
	-DCMAKE_BUILD_TYPE=%build_type% ^
	-DCMAKE_DEBUG_POSTFIX=_d ^
	-DCMAKE_C_FLAGS=-m32 ^
  -DBUILD_SHARED_LIBS=OFF ^
  -DCMAKE_PREFIX_PATH=%sh_folder%/gflags-windows-gcc-x86 ^
	-DCMAKE_INSTALL_PREFIX=%sh_folder%/glog-windows-gcc-x86 ^
	..
make install -j8
popd
rmdir build_gcc_x86 /s/q
goto:eof

:gcc_x86_64
echo build x86_64 use MinGW 
if exist build_gcc_x86_64 rmdir build_gcc_x86_64 /s/q
mkdir build_gcc_x86_64
pushd build_gcc_x86_64
rem gcc SJLJ or SEH distribution required

cmake -G "MinGW Makefiles" ^
	-DCMAKE_BUILD_TYPE=%build_type% ^
	-DCMAKE_DEBUG_POSTFIX=_d ^
	-DCMAKE_C_FLAGS=-m64 ^
  -DBUILD_SHARED_LIBS=OFF ^
  -DCMAKE_PREFIX_PATH=%sh_folder%/gflags-windows-gcc-x86_64 ^
	-DCMAKE_INSTALL_PREFIX=%sh_folder%/glog-windows-gcc-x86_64 ^
	..
make install -j8
popd
rmdir build_gcc_x86_64 /s/q
goto:eof
