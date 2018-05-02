# feature_se(feature search engine)

(人脸)特征内存搜索引擎(feature search engine)，提供高速的人脸特征相似度比对搜索/排序,支持多线程并行搜索，适用于百万级以上人脸库的快速搜索。(C++11实现)

## clone代码

	git clone --recursive https://gitee.com/l0km/feature_se.git
	# 因为项目中有submodule，克隆代码时必须加上 --recursive 参数

## feature_se

实现在内存中进行大规模特征比对的人脸搜索功能。

人脸搜索引擎的核心是一个允许并发读写的自定义哈希表(参见 [hashtable_abstract.h](feature_se/hashtable_abstract.h),[ICodeManagerCPU.cpp](feature_se/ICodeManagerCPU.cpp)),特征搜索过程是就是对哈希表中的(人脸)特征多线程分段比对排序/合并结果(类似map/reduce)的过程。

大规模人脸搜索的速度最终取决于人脸识别算法提供的比对函数的计算速度和服务器内存读写速度。本库提供的搜索算法将所有的特征加载到内存，完全基于内存比对，最大限度减少了比对函数之外的运行时间开销，基于CASSDK人脸识别算法在500万条记录的环境下，(24核处理器)搜索速度在0.1~0.2秒。

当前项目支持CASSDK人脸识别算法，但是搜索算法本身已经高度抽象，独立于人脸识别算法，如果需要支持其他人脸识别算法，只需要替换对应的特征比对函数即可,参见:

[feature_se/feature_compare_cas.h](feature_se/feature_compare_cas.h)

[feature_se/HashMapCl.cpp](feature_se/HashMapCl.cpp)


cmake生成的工程文件可以同时生成动态库(fse_cas)，静态库(fse_cas_static)。windows下还提供不依赖msvcrt.dll的MT静态库(fse_cas_static_mt)。

参见cmake脚本：[feature_se/CMakeLists.txt](feature_se/CMakeLists.txt)

**接口描述**

标准C接口，参见 [feature_se/feature_se.h](feature_se/feature_se.h)

## jni

feature_se的jni接口动态库实现，提供java访问能力。

jni接口定义参见[jni/net_gdface_sdk_fse_FseJniBridge.h](jni/net_gdface_sdk_fse_FseJniBridge.h)

对应的java接口定义参见[java.net.gdface.sdk.fse.FseJniBridge.java](https://gitee.com/l0km/faceapi/blob/master/faceapi-base/src/main/java/net/gdface/sdk/fse/FseJniBridge.java)

## 编译

要求cmake 3.0以上版本

要求第三方库:CASSDK 8.0.0 人脸识别算法库（商业产品，需要另外购买）

### windows

window下编译要求Visual Studio 2015

Windows下命令行编译过程:

	rem 清除 build 文件夹
	if exist build rmdir build /s/q
	mkdir build
	cd build
	rem 创建VS2015编译环境,只需要执行一次
	call "%VS140COMNTOOLS%..\..\vc/vcvarsall" x86_amd64
	rem 生成Makefile
	cmake -G "NMake Makefiles" ^ 
		-DCMAKE_BUILD_TYPE=RELEASE ^
		-DCASSDK_ROOT_DIR=%CASSDK算法SDK安装位置% ^
		-DCMAKE_INSTALL_PREFIX=..\release\fse_windows_x86_64 ..
	rem 编译并安装到CMAKE_INSTALL_PREFIX指定的位置
	nmake install 

参见编译脚本：[build.bat](build.bat)

执行[all_build.bat](all_build.bat)同时编译debug,release版

### vs2015工程编译

可以用 [make_msvc_project.bat](make_msvc_project.bat) 创建Visuao Studio 2015工程

### Linux

linux下编译要求支持c++11的gcc 5.2.0

linux下命令行编译过程:

	# 清除 build.gcc 文件夹
	[ -d build.gcc ] && rm -fr build.gcc
	mkdir build.gcc
	pushd build.gcc
	# 生成release 版 Makefile
	cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=RELEASE \
		-DCASSDK_ROOT_DIR=${CASSDK算法SDK安装位置} \
		-DCMAKE_INSTALL_PREFIX=../release/fse_linux_x86_64 ..
	# 编译并安装到CMAKE_INSTALL_PREFIX指定的位置
	make install
	popd

参见编译脚本 [build.sh](build.sh)

## 调用示例

cmake查找feature_se库的示例：

	# 调用cmake/Modules/FindFse.cmake查找fse依赖库
	# 需要在 CMAKE_MODULE_PATH 指定 FindFse.cmake的位置
	find_package(Fse REQUIRED)

cmake脚本中引用feature_se库的示例：

	# 引用fse_cas动态库
	target_link_libraries(test_fse gdface::fse_cas)
	# 引用fse_cas静态库
	target_link_libraries(test_fse_static gdface::fse_cas_static)
	# 引用fse_cas静态库/MT
	target_link_libraries(test_fse_static_mt gdface::fse_cas_static_mt)

cmake脚本中引用feature_se库的完整示例参见 [test/CMakeLists.txt](test/CMakeLists.txt)

创建调用示例的VS2015工程:

	mkdir build
	cd build
	call "%VS140COMNTOOLS%..\..\vc/vcvarsall" x86_amd64
	rem 生成64位工程
	cmake -G "Visual Studio 14 2015 Win64" ^
		-DCASSDK_ROOT_DIR=%CASSDK算法SDK安装位置% ^
		-DFSE_ROOT_DIR=..\..\release\fse_windows_x86_64 ..
	rem 需要先编译feature_se
	rem FSE_ROOT_DIR用于指定 feature_se 安装位置

完整脚本参见 [test/make_msvc_project.bat](test/make_msvc_project.bat)
 
生成unix Makefile过程参见：
[test/make_unix_makefile.sh](test/make_unix_makefile.sh)