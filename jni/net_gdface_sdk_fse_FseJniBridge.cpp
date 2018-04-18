/*
 * net_gdface_facedbsdk_local_CodeCacheManager.cpp
 *
 *  Created on: 2015年10月2日
 *      Author: guyadong
 */
#include "net_gdface_sdk_fse_FseJniBridge.h"
#include "jni_utilits.h"
#include "BeanUtilits.h"
#include "feature_se.h"
#include "JNIContext.h"
using namespace gdface;
using namespace std;
// JNI 上下文对象全局指针
JNIContext* context;
void throwJNIException(const char* msg) {
	jni_utilits::throwByName("net/gdface/sdk/fse/JNIException", msg);
}
JNIEXPORT void JNICALL Java_net_gdface_sdk_fse_FseJniBridge_init
(JNIEnv *env, jclass,jint mode, jint initCapacity, jfloat loadFactor, jint overBlockCapacity){
	// 初始化表对象
	auto result = fse_init(initCapacity, loadFactor, overBlockCapacity);
	if (result >= 0) {
		context = new JNIContext(BeanUtilits::JavaClassMirrorForCodeBean());
	}
	else {
		throwJNIException(fse_error_msg());
	}

}

JNIEXPORT void JNICALL Java_net_gdface_sdk_fse_FseJniBridge_release
  (JNIEnv *, jclass){
	std::cout << "feature_se jni release..." << std::endl;
	delete context;
	auto result = fse_release();
	if (result < 0) {
		throwJNIException(fse_error_msg());
	}
}

JNIEXPORT jobjectArray JNICALL Java_net_gdface_sdk_fse_FseJniBridge_searchCode
  (JNIEnv *env, jclass, jbyteArray jcode, jdouble threshold, jint rows, jobjectArray imgMD5Set, jboolean full){
	try{
		face_code code;
		if(!BeanUtilits::jbytearraytoface_code(jcode,code)){
			throw invalid_argument("fail to jbytearraytoface_code");
		}
		auto v=BeanUtilits::jmd5settoMD5Vector(imgMD5Set);
		vector<code_bean> result(rows);
		auto size=fse_searchCode(std::addressof(code),threshold,rows,result.data(),v->data(), v->size());
		if (size >= 0) {
			return BeanUtilits::code_bean_ptr_tojobjectArray(result.data(), (jsize)size, context->m_code_bean_mirror, full).norelease().get();
		}
		throwJNIException(fse_error_msg());
	}catch(invalid_argument &e){
		if (!env->ExceptionOccurred()) {
			jni_utilits::throwIllegalArgumentException(e.what());
		}
	}catch(exception &e){
		if (!env->ExceptionOccurred()) {
			throwJNIException(e.what());
		}			
	}
	return nullptr;
}

JNIEXPORT jobject JNICALL Java_net_gdface_sdk_fse_FseJniBridge_getFeature
  (JNIEnv *env, jclass, jbyteArray jmd5, jboolean full){
	try{
		MD5 md5;
		if(BeanUtilits::jbytearraytoMD5(jmd5,md5)){
			code_bean bean;
			auto result=fse_getFeature(std::addressof(md5),std::addressof(bean));
			switch (result) {
			case 1: {
				return BeanUtilits::toJCodeBean(bean, context->m_code_bean_mirror, full).norelease().get();
			}
			case 0: {
				return nullptr;
			}
			default:
				throwJNIException(fse_error_msg());
				break;
			}
		}else if(!env->ExceptionOccurred()){
			jni_utilits::throwIllegalArgumentException("fail to jbytearraytoMD5");
		}
	}catch(exception &e){
		if (!env->ExceptionOccurred()) {
			throwJNIException(e.what());
		}
	}
	return nullptr;
}

JNIEXPORT jboolean JNICALL Java_net_gdface_sdk_fse_FseJniBridge_addFeature
  (JNIEnv *env, jclass, jbyteArray id, jbyteArray code, jstring imgMD5){
	try{
		code_bean bean;
		if(BeanUtilits::tocodeBean(bean,id,code,imgMD5)){
			//BeanUtilits::output_bean(bean,"jni add ");
			auto result = fse_addFeature(std::addressof(bean));
			if(result >= 0){
				return JNI_TRUE;
			}
			throwJNIException(fse_error_msg());
		}else if(!env->ExceptionOccurred()){
			jni_utilits::throwIllegalArgumentException("fail to tocodeBean");
		}
	}catch(exception &e){
		if (!env->ExceptionOccurred()) {
			throwJNIException(e.what());
		}			
	}
	return JNI_FALSE;
}

JNIEXPORT jboolean JNICALL Java_net_gdface_sdk_fse_FseJniBridge_removeFeature
  (JNIEnv *env, jclass, jbyteArray codeMD5){
	try {
		MD5 md5;
		if (BeanUtilits::jbytearraytoMD5(codeMD5, md5)) {
			auto result = fse_removeFeature(std::addressof(md5));
			switch (result) {
			case 0: {
				return JNI_FALSE;
			}
			case 1: {
				return JNI_TRUE;
			}
			default:
				throwJNIException(fse_error_msg());
				break;
			}
		}			
		else if (!env->ExceptionOccurred()) {
			jni_utilits::throwIllegalArgumentException("fail to jbytearraytoMD5");
		}
	} catch (exception &e) {
		if (!env->ExceptionOccurred()){
			throwJNIException(e.what());
		}
	}
	return JNI_FALSE;
}

JNIEXPORT jint JNICALL Java_net_gdface_sdk_fse_FseJniBridge_size
  (JNIEnv *, jclass){
	auto result = fse_size();
	if (result < 0) {
		throwJNIException(fse_error_msg());
	}
	return (jint)result;
}
JNIEXPORT jstring JNICALL Java_net_gdface_sdk_fse_FseJniBridge_statInfo
  (JNIEnv *, jclass){
	auto info=fse_statInfo();
	if (nullptr == info) {
		throwJNIException(fse_error_msg());
		return nullptr;
	}
	return jni_utilits::raii_NewStringUTF(info).norelease().get();
}
