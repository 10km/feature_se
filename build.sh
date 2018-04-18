#!/bin/sh
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
[ -n "$1" ] && INSTALL_FOLDER="$1"
[ -z "$1" ] && INSTALL_FOLDER=$(dirname $(readlink -f $0))/release/fse_linux_x86_64
# 默认INSTALL_FOLDER为源码文件夹下
echo INSTALL_FOLDER:$INSTALL_FOLDER
if [ -d build.gcc ]
then 
	rm -fr build.gcc/*
else 
	mkdir build.gcc
fi
pushd build.gcc
cmake "`dirs +1`" $GXX_PATH -G "Unix Makefiles" -DCMAKE_INSTALL_PREFIX=$INSTALL_FOLDER
make install
popd
rm -fr build.gcc