#!/bin/bash

sh_folder=$(dirname $(readlink -f $0))
folder_name=$(basename $sh_folder) 
# 定义编译的版本类型(DEBUG|RELEASE)
build_type=DEBUG
typeset -u arg1=$1
[ "$arg1" = "DEBUG" ] && build_type=$arg1
echo build_type=$build_type

pushd $sh_folder/..

[ -d $folder_name.prj ] && rm -fr $folder_name.prj
mkdir $folder_name.prj

pushd $folder_name.prj
cmake "$sh_folder" -G "Eclipse CDT4 - Unix Makefiles" -DCMAKE_BUILD_TYPE=$build_type \
	-DCMAKE_INSTALL_PREFIX=$sh_folder/release/fse_linux_x86_64
popd

popd
