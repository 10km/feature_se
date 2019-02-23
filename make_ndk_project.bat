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

@rem EXT_SDK_TYPE 指定算法类型可选值： 
@rem                    CASSDK(默认值) 
@rem                    EUCLIDEAN  默认使用欧氏距离计算相似度 
@rem                    CUSTOM   使用自定义算法提的供相似度比较函数，此方式暂时未支持 
@rem 如果EXT_SDK_TYPE指定为EUCLIDEAN,下列参数需要设置:
@rem EUCLIDEAN_ELEM_TYPE 定义特征值数组类型(double/float)，如果不指定，默认值为double 
@rem EUCLIDEAN_ELEM_LEN  定义特征值数组长度 
@rem EUCLIDEAN_CODE_END_WITH_SUM  定义特征值数组最后是否有一个double保存特征值数组的点积和，默认为OFF 
@rem ============================下列为通用参数与EXT_SDK_TYPE无关
@rem FSE_LIBNAME        指定生成jni动态名,不指定则使用默认值 
@rem JNI_FSE_LIBNAME    指定生成jni动态名,不指定则使用默认值 

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


popd
popd
