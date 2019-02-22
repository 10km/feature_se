#!/bin/bash
# Android NDK 交叉编译脚本,创建Makefile
# author guyadong

# 检测是否安装NDK,没有安装NDK则报错退出
[ -z "$ANDROID_NDK" ] && echo "ERROR:environment variable ANDROID_NDK not define" && exit -1

sh_folder=$(dirname $(readlink -f $0))
folder_name=$(basename $sh_folder) 
# 定义编译的版本类型(DEBUG|RELEASE)
build_type=RELEASE
typeset -u arg1=$1
[ "$arg1" = "DEBUG" ] && build_type=$arg1
echo build_type=$build_type

pushd $sh_folder/..

[ -d $folder_name.ndk.prj ] && rm -fr $folder_name.ndk.prj
mkdir $folder_name.ndk.prj

pushd $folder_name.ndk.prj
cmake "$sh_folder" -G "Eclipse CDT4 - Unix Makefiles" -DCMAKE_BUILD_TYPE=$build_type \
	-DEXT_SDK_TYPE=EUCLIDEAN \
	-DDEFAULT_ELEM_TYPE=double \
	-DDEFAULT_ELEM_LEN=512 \
	-DDEFAULT_CODE_END_WITH_SUM=OFF \
	-DCMAKE_INSTALL_PREFIX=$sh_folder/release/fse_android_armeabi-v7a \
	-DCMAKE_SYSTEM_NAME=Android \
	-DCMAKE_SYSTEM_VERSION=17 \
	-DANDROID_ARM_NEON=ON \
	-DCMAKE_TOOLCHAIN_FILE=$ANDROID_NDK/build/cmake/android.toolchain.cmake

popd
popd
