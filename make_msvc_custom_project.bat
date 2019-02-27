echo off 
echo make feature_se VS2015 project
if not defined VS140COMNTOOLS (
	echo vs2015 NOT FOUND.
	exit /B -1
)
echo vs2015 found.
where cmake
if errorlevel 1 (
	echo cmake NOT FOUND.
	exit /B -1
)
echo cmake found
where java
if errorlevel 1 (
	echo java NOT FOUND.
	exit /B -1
)
echo java found
set sh_folder=%~dp0
pushd %sh_folder%

rem 需要先编译feature_se
if exist project.custom.vs2015 rmdir project.custom.vs2015 /s/q
mkdir project.custom.vs2015
pushd project.custom.vs2015
if not defined VisualStudioVersion (
	echo make MSVC environment ...
	call "%VS140COMNTOOLS%..\..\vc/vcvarsall" x86_amd64
)

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
@rem CUSTOM_CODE_BYTE_LEN 特征数据长度(byte)
@rem CUSTOM_FEACOMP_INIT 比对函数初始化代码，可不指定

echo creating x86_64 Project for Visual Studio 2015 ...
cmake -G "Visual Studio 14 2015 Win64" -DCMAKE_INSTALL_PREFIX=..\release\fse_custom_windows_x86_64 .. ^
	-DEXT_SDK_TYPE=CUSTOM ^
	-DCUSTOM_FEACOMP_INCLUDE=J:\workspace.neon\cassdk54\FSFaceSDK\FSFaceSDK-windows-x86_64\include ^
	-DCUSTOM_FEACOMP_LIBRARY=J:\workspace.neon\cassdk54\FSFaceSDK\FSFaceSDK-windows-x86_64\lib\FSFaceSDK.lib ^
	-DCUSTOM_FEACOMP_HEADERS=FSFaceSDK.h ^
	-DCUSTOM_SYS_HEADERS=windows.h ^
	-DCUSTOM_FEACOMP_FUNNAME=FSCompare ^
	-DCUSTOM_FEACOMP_FUNTYPE="double(unsigned char*, unsigned char*)" ^
	-DCUSTOM_CODE_BYTE_LEN=2560 

popd
popd