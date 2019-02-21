#!/bin/bash

[ -z "$ANDROID_NDK" ] && echo "environment variable ANDROID_NDK not define" && exit -1

sh_folder=$(dirname $(readlink -f $0))
folder_name=$(basename $sh_folder) 
# 定义编译的版本类型(DEBUG|RELEASE)
build_type=DEBUG
typeset -u arg1=$1
[ "$arg1" = "DEBUG" ] && build_type=$arg1
echo build_type=$build_type

pushd $sh_folder/..

[ -d $folder_name.ndk.prj ] && rm -fr $folder_name.ndk.prj
mkdir $folder_name.ndk.prj

pushd $folder_name.ndk.prj
cmake "$sh_folder" -G "Eclipse CDT4 - Unix Makefiles" -DCMAKE_BUILD_TYPE=$build_type \
	-DEXT_SDK_TYPE=DEFAULT \
	-DCMAKE_INSTALL_PREFIX=$sh_folder/release/fse_android_armeabi-v7a \
	-DCMAKE_SYSTEM_NAME=Android \
	-DCMAKE_SYSTEM_VERSION=17 \
	-DANDROID_ARM_NEON=ON \
	-DANDROID_TOOLCHAIN_NAME=arm-linux-androideabi-4.9 \
	-DCMAKE_TOOLCHAIN_FILE=$ANDROID_NDK/build/cmake/android.toolchain.cmake

# -DCMAKE_ANDROID_STANDALONE_TOOLCHAIN=$ANDROID_NDK
#	-DCMAKE_ANDROID_NDK=$ANDROID_NDK 

popd
popd
