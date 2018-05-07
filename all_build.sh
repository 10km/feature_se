#!/bin/bash
sh_folder=$(dirname $(readlink -f $0))
pushd $sh_folder
[ -d release ] && rm -fr release
./build.sh debug
./build.sh release
popd
