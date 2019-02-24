# feature_se(feature search engine)

(人脸)特征内存搜索引擎(feature search engine)，提供高速的人脸特征相似度比对搜索/排序,支持多线程并行搜索，适用于百万级以上人脸库的快速搜索。(C++11实现)

## clone代码

	git clone --recursive https://gitee.com/l0km/feature_se.git
	# 因为项目中有submodule，克隆代码时必须加上 --recursive 参数

## feature_se

实现在内存中进行大规模特征比对的人脸搜索功能。

人脸搜索引擎的核心是一个允许并发读写的自定义哈希表(参见 [hashtable_abstract.h](feature_se/hashtable_abstract.h),[ICodeManagerCPU.cpp](feature_se/ICodeManagerCPU.cpp)),特征搜索过程是就是对哈希表中的(人脸)特征多线程分段比对排序/合并结果(类似map/reduce)的过程。

大规模人脸搜索的速度最终取决于人脸识别算法提供的比对函数的计算速度和服务器内存读写速度。本库提供的搜索算法将所有的特征加载到内存，完全基于内存比对，最大限度减少了比对函数之外的运行时间开销，基于CASSDK人脸识别算法在500万条记录的环境下，(24核处理器)搜索速度在0.1~0.2秒。

当前项目支持CASSDK人脸识别算法，也可以支持其他第三方算法，搜索算法本身已经高度抽象，独立于人脸识别算法，如果需要支持其他人脸识别算法，只需要替换对应的特征比对函数即可,参见:

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

编译要求


- cmake 3.0以上版本
- JDK 1.7


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

可以用 [make_msvc_cassdk_project.bat](make_msvc_cassdk_project.bat) 创建Visuao Studio 2015工程

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

### Android NDK

编译要求

- Android NDK 版本不限，但Clang要求支持C++11编译，15c编译通过 

Android NDK交叉编译方法如下(参见 [make_ndk_project.bat](make_ndk_project.bat)).



	@rem EXT_SDK_TYPE 指定算法类型可选值： 
	@rem                    CASSDK(默认值) 
	@rem                    EUCLIDEAN  默认使用欧氏距离计算相似度 
	@rem                    CUSTOM   使用自定义算法提的供相似度比较函数，此方式暂时未支持 
	@rem 如果EXT_SDK_TYPE指定为EUCLIDEAN,下列参数需要设置:
	@rem EUCLIDEAN_ELEM_TYPE 定义特征值数组类型(double/float)，如果不指定，默认值为double 
	@rem EUCLIDEAN_ELEM_LEN  定义特征值数组长度 
	@rem EUCLIDEAN_CODE_END_WITH_SUM  定义特征值数组最后是否有一个double保存特征值数组的点积和，默认为OFF 
	@rem ============================下列为通用参数与EXT_SDK_TYPE无关
	@rem FSE_LIBNAME        指定生成动态库名,不指定则使用默认值 
	@rem JNI_FSE_LIBNAME    指定生成jni动态库名,不指定则使用默认值 
	
	cmake %sh_folder% -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=%build_type% ^
		-DJNI_FSE_LIBNAME=FS_FaceFeatureCompare ^
		-DEXT_SDK_TYPE=EUCLIDEAN ^
		-DEUCLIDEAN_ELEM_TYPE=double ^
		-DEUCLIDEAN_ELEM_LEN=512 ^
		-DEUCLIDEAN_CODE_END_WITH_SUM=OFF ^
		-DCMAKE_SYSTEM_VERSION=17 ^
		-DANDROID_ARM_NEON=ON ^
		-DCMAKE_INSTALL_PREFIX=%sh_folder%release\fse_android_armeabi-v7a ^
	 	-DCMAKE_TOOLCHAIN_FILE=%ANDROID_NDK%\build\cmake\android.toolchain.cmake 

### 指定第三方库的特征比对函数

本项目独立于具体人脸识别算法，所以在实际应用中需要指定人脸特征比对函数，可以如下示例使用第三方库提供的比对函数，无需修改本项目代码,完整脚本参见  [make_msvc_custom_project.bat](make_msvc_custom_project.bat)
	
	@rem 使用第三方识别库提供的特征比对函数 
	@rem EXT_SDK_TYPE 识别函数类型 CUSTOM 使用第三方库提供的特征比对函数  
	@rem 如果EXT_SDK_TYPE指定为CUSTOM,下列参数需要设置:
	@rem CUSTOM_FEACOMP_INCLUDE 指定比对函数所在头文件的位置(文件夹全路径) 
	@rem CUSTOM_FEACOMP_LIBRARY 指定比对函数所在库文件(全路径) 
	@rem CUSTOM_FEACOMP_HEADERS 指定引用比对函数所需要的头文件名列表,';'分隔,按顺序引用 
	@rem CUSTOM_FEACOMP_FUNNAME 指定比对函数名,
	@rem CUSTOM_FEACOMP_FUNTYPE 指定比对函数类型定义,
	@rem        格式:return_type(intput_type0,intput_type1),如果不指定则默认为double(unsigned char*,unsigned char*)
	@rem CUSTOM_SYS_HEADERS 指定需要引用的系统头文件名,如windows.h,可不设置  
	
	echo creating x86_64 Project for Visual Studio 2015 ...
	cmake -G "Visual Studio 14 2015 Win64" -DCMAKE_INSTALL_PREFIX=..\release\fse_custom_windows_x86_64 .. ^
		-DEXT_SDK_TYPE=CUSTOM ^
		-DCUSTOM_FEACOMP_INCLUDE=J:\workspace.neon\cassdk54\FSFaceSDK\FSFaceSDK-windows-x86_64\include ^
		-DCUSTOM_FEACOMP_LIBRARY=J:\workspace.neon\cassdk54\FSFaceSDK\FSFaceSDK-windows-x86_64\lib\FSFaceSDK.lib ^
		-DCUSTOM_FEACOMP_HEADERS=FSFaceSDK.h ^
		-DCUSTOM_SYS_HEADERS=windows.h ^
		-DCUSTOM_FEACOMP_FUNNAME=FSCompare ^
		-DCUSTOM_FEACOMP_FUNTYPE="double(unsigned char*, unsigned char*)"

根据以上提供的信息,cmake脚本会自动生成如下的临时代码文件`custom_feature_compare.h`,并加入项目中，在实现内存特征搜索时会调用代码中的`compare`函数从而实现调用第三方比对函数完成特征比对：

	/*
	 * custom_feature_compare.h
	 *  Description: 由custom_feature_compare.h.in模板自动生成,
	 *               用于将第三方特征比对函数封装为统一的接口供搜索比对调用
	 *  Created on: 2019年2月22日
	 *      Author: guyadong
	 */
	
	#ifndef CUSTOM_FEACOMP_H_
	#define CUSTOM_FEACOMP_H_
	#include <functional>
	#include <tuple>
	
	#include <windows.h>
	#include "FSFaceSDK.h"
	
	#include "codemgr.h"
	namespace gdface{
	namespace feature{
	template<typename T> 
	struct function_traits;  
	
	template<typename R, typename ...Args> 
	struct function_traits<std::function<R(Args...)>>
	{
	    static const size_t nargs = sizeof...(Args);
	
	    typedef R result_type;
	
	    template <size_t i>
	    struct arg
	    {
	        typedef typename std::tuple_element<i, std::tuple<Args...>>::type type;
	    };
	};
	inline double compare(const face_code &f1,const face_code&f2){
	
		typedef std::function<double(unsigned char*, unsigned char*)> feacomp_fun;
		return (double)FSCompare((function_traits<feacomp_fun>::arg<0>::type)f1.element,(function_traits<feacomp_fun>::arg<1>::type)f2.element);
	}
	} /* namespace feature*/
	} /* namespace gdface*/
	
	#endif /* CUSTOM_FEACOMP_H_ */


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

完整脚本参见 [test/make_msvc_cassdk_project.bat](test/make_msvc_cassdk_project.bat)
 
生成unix Makefile过程参见：
[test/make_gcc_project.sh](test/make_gcc_project.sh)