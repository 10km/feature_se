#!/bin/bash
GXX_PATH=
if [ `/usr/bin/g++ -dumpversion` != "5.2.0" ]
then
	if [ `/usr/local/bin/g++ -dumpversion` = "5.2.0" ]
	then
		GXX_PATH="-DCMAKE_CXX_COMPILER:FILEPATH=/usr/local/bin/g++ -DCMAKE_C_COMPILER:FILEPATH=/usr/local/bin/gcc"
	else
		echo "g++ compiler required version 5.2.0"
		exit -1
	fi
fi

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
cmake "$sh_folder" $GXX_PATH -G "Eclipse CDT4 - Unix Makefiles" -DCMAKE_BUILD_TYPE=$build_type 
popd

popd
