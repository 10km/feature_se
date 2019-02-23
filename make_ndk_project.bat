echo off
rem Android NDK �������ű�,����Makefile
rem author guyadong

rem ����Ƿ�װNDK,û�а�װNDK�򱨴��˳�
if not DEFINED ANDROID_NDK (
	echo "ERROR:environment variable ANDROID_NDK not define" && exit /B -1
	)
where cmake
if errorlevel 1 (
	echo cmake NOT FOUND.
	exit /B -1
)

set sh_folder=%~dp0
rem �������İ汾����(DEBUG|RELEASE)
set build_type=RELEASE
if "%1" == "DEBUG" set build_type=%1
echo build_type=%build_type%

pushd %sh_folder%..
if exist feature_se.ndk.prj rmdir  feature_se.ndk.prj /s/q
mkdir feature_se.ndk.prj

pushd feature_se.ndk.prj

@rem EXT_SDK_TYPE ָ���㷨���Ϳ�ѡֵ�� 
@rem                    CASSDK(Ĭ��ֵ) 
@rem                    EUCLIDEAN  Ĭ��ʹ��ŷ�Ͼ���������ƶ� 
@rem                    CUSTOM   ʹ���Զ����㷨��Ĺ����ƶȱȽϺ������˷�ʽ��ʱδ֧�� 
@rem ���EXT_SDK_TYPEָ��ΪEUCLIDEAN,���в�����Ҫ����:
@rem EUCLIDEAN_ELEM_TYPE ��������ֵ��������(double/float)�������ָ����Ĭ��ֵΪdouble 
@rem EUCLIDEAN_ELEM_LEN  ��������ֵ���鳤�� 
@rem EUCLIDEAN_CODE_END_WITH_SUM  ��������ֵ��������Ƿ���һ��double��������ֵ����ĵ���ͣ�Ĭ��ΪOFF 
@rem ============================����Ϊͨ�ò�����EXT_SDK_TYPE�޹�
@rem FSE_LIBNAME        ָ������jni��̬��,��ָ����ʹ��Ĭ��ֵ 
@rem JNI_FSE_LIBNAME    ָ������jni��̬��,��ָ����ʹ��Ĭ��ֵ 

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
