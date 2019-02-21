echo off
rem Android NDK 交叉编译脚本,创建Makefile
rem author guyadong

rem 检测是否安装NDK,没有安装NDK则报错退出
if not DEFINED ANDROID_NDK (
	echo "ERROR:environment variable ANDROID_NDK not define" && exit /B -1
	)
where cmake
if errorlevel 1 (
	echo cmake NOT FOUND.
	exit /B -1
)

set sh_folder=%~dp0
rem 定义编译的版本类型(DEBUG|RELEASE)
set build_type=RELEASE
if "%1" == "DEBUG" set build_type=%1
echo build_type=%build_type%

pushd %sh_folder%..
if exist feature_se.ndk.prj rmdir  feature_se.ndk.prj /s/q
mkdir feature_se.ndk.prj

pushd feature_se.ndk.prj

cmake %sh_folder% -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=%build_type% ^
	-DEXT_SDK_TYPE=DEFAULT ^
	-DCMAKE_SYSTEM_VERSION=17 ^
	-DANDROID_ARM_NEON=ON ^
	-DCMAKE_INSTALL_PREFIX=%sh_folder%release\fse_android_armeabi-v7a ^
  -DCMAKE_TOOLCHAIN_FILE=%ANDROID_NDK%\build\cmake\android.toolchain.cmake 

popd
popd
