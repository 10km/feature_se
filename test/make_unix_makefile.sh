#!/bin/bash
GXX_PATH=
if [ `/usr/bin/g++ -dumpversion` != "5.2.0" ]
then
	if [ `/usr/local/bin/g++ -dumpversion` = "5.2.0" ]
	then
		GXX_PATH="-DCMAKE_CXX_COMPILER:FILEPATH=/usr/local/bin/g++ -DCMAKE_C_COMPILER:FILEPATH=/usr/local/bin/gcc -DCMAKE_BUILD_TYPE:STRING=RELEASE"
	else
		echo "g++ compiler required version 5.2.0"
		exit -1
	fi
fi

sh_folder=$(dirname $(readlink -f $0))
cd $sh_folder

if [ -d build.gcc ]
then 
	rm -fr build.gcc/*
else 
	mkdir build.gcc
fi

cd build.gcc

cmake $GXX_PATH -G "Unix Makefiles" \
	-DCMAKE_MODULE_PATH=$sh_folder/../cmake/Modules \
	-DFSE_ROOT_DIR=../../release/fse_linux_x86_64 ..

