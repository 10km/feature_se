/*
 * beanutilits.cpp
 *
 *  Created on: 2015年11月28日
 *      Author: guyadong
 */
#include <cstring>
#include "BeanUtilits.h"
#include "dot_product.h"


namespace gdface {
bool BeanUtilits::tocodeBean(code_bean& bean, jobject obj, jni_utilits::JavaClassMirror& mirror) {
	if (nullptr == obj) {
		return false;
	}
	auto success = tocodeBean(bean,
			mirror.GetField<jbyteArray>(obj, CODEBEAN_ID).get(),
			mirror.GetField<jbyteArray>(obj, CODEBEAN_CODE).get(),
			mirror.GetField<jstring>(obj, CODEBEAN_IMGMD5).get());
	static_assert(std::is_same<decltype(code_bean::similarity),jdouble>::value,"different type with code_bean::similarity and jdouble");
	bean.similarity = mirror.GetField<jdouble>(obj, CODEBEAN_SIMILARITY);
	return success;
}
bool BeanUtilits::jstringToMD5(jstring jstr, MD5& md5) {
	if (nullptr == jstr){
		// 为null时填0
		md5.l[0] = 0,md5.l[1] = 0;
		return false;
	}
	auto env = jni_utilits::getJNIEnv();
	auto len = env->GetStringUTFLength(jstr);
	if (len == (sizeof(MD5) << 1)) {
		auto bytes = jni_utilits::raii_GetStringUTFChars(jstr);		
		PSTRtoMD5(bytes.get(), md5);
		return true;
	}
	return false;
}

bool BeanUtilits::jbytearraytoMD5(jbyteArray bytes, MD5& md5) {
	if (nullptr == bytes) {
		return false;
	}
	auto env = jni_utilits::getJNIEnv();
	if (env->GetArrayLength(bytes) == sizeof(MD5)) {
		auto byte_ptr = jni_utilits::raii_GetByteArrayElements(bytes);
		md5 = *((MD5*) ((byte_ptr.get())));
		return true;
	}
	return false;
}

bool BeanUtilits::jbytearraytoface_code(jbyteArray bytes, face_code& code) {
	if (nullptr == bytes) {
		return false;
	}
	auto env = jni_utilits::getJNIEnv();
	if (env->GetArrayLength(bytes) == sizeof(face_code)) {
		auto byte_ptr = jni_utilits::raii_GetByteArrayElements(bytes);
		code = *((face_code*)((byte_ptr.get())));
		return true;
	}
#if EUCLIDEAN_FEACOMP && ! CODE_END_WITH_SUM
	else if (env->GetArrayLength(bytes) == sizeof(code.element)) {		
		auto byte_ptr = jni_utilits::raii_GetByteArrayElements(bytes);
		// 复制所有特征数据到element
		std::memcpy(code.element, byte_ptr.get(), sizeof(code.element));
		// 计算特征值数组的点积和保存在sum
		code.sum = dot_product<CODE_FLOAT_NUM>(code.element, code.element);
		return true;
	}
#endif
	return false;
}

raii_var<jobject> BeanUtilits::toJCodeBean(const code_bean& bean, jni_utilits::JavaClassMirror& mirror,jboolean full) {
	auto var = jni_utilits::raii_NewObject(mirror.javaclass.get(), mirror.constructor);
	auto obj = *var;
	if (nullptr != obj) {
		mirror.SetField(obj, CODEBEAN_ID, MD5tojbyteArray(bean.id).get());
		if (full) {
			mirror.SetField(obj, CODEBEAN_CODE, face_codetojbyteArray(FACE_CODE_CONVERT(bean.code)).get());
		}
		// 如果imgMD5不为全0则转为java string,否则为null
		if ( ! bean.imgMD5.is_null() ) {
			mirror.SetField(obj, CODEBEAN_IMGMD5, MD5toJString(bean.imgMD5).get());
		}
		mirror.SetField(obj, CODEBEAN_SIMILARITY, (jdouble) (bean.similarity));
	}
	return var;
}
bool BeanUtilits::tocodeBean(code_bean& bean, jbyteArray id, jbyteArray code, jstring imgMD5) {
	auto result = jbytearraytoface_code(code, bean.code);
	// 允许imgMD5为null
	if (result) {
		if (nullptr == id) {
			// id 为null抛出异常
			jni_utilits::throwIllegalArgumentException("feature id must not be null");
			return false;
			/*auto md5util = std::make_shared<md5::MD5>();
			md5util->digestMemory(std::addressof(bean.code), sizeof(bean.code));
			bean.id = *(MD5*)md5util->digestRaw;*/
		}
		else {
			result &= jbytearraytoMD5(id, bean.id);
		}
		result &= jstringToMD5(imgMD5, bean.imgMD5) || nullptr == imgMD5;
	}
	return result;
	
}
std::shared_ptr<vector<MD5> >  BeanUtilits::jmd5settoMD5Vector(jobjectArray jset) {
	decltype(jni_utilits::getJNIEnv()->GetArrayLength(jset)) len =
			nullptr != jset ? jni_utilits::getJNIEnv()->GetArrayLength(jset) : 0;
	auto v = std::make_shared<vector<MD5> >(len);
	for (auto i = len; i > 0; --i) {
		auto jstr = jni_utilits::raii_GetObjectArrayElement<jstring>(jset, i - 1);
		if (!BeanUtilits::jstringToMD5(*jstr, (*v)[i])) {
			throw invalid_argument("invalid jstring argument,cant convert to MD5");
			break;
		}
	}
	return std::move(v);
}
raii_var<jobjectArray> BeanUtilits::code_bean_ptr_tojobjectArray(const code_bean* src, jsize size, jni_utilits::JavaClassMirror& mirror,
		jboolean full) {
	auto env = jni_utilits::getJNIEnv();
	auto var=jni_utilits::raii_NewObjectArray(size, *mirror.javaclass, nullptr);
	auto joarray = *var;
	if (nullptr != joarray) {
		for (auto i = size; i > 0; --i) {
			auto obj = toJCodeBean(src[i - 1], mirror, full);
			env->SetObjectArrayElement(joarray, i - 1, obj.get());
		}
	}
	return var;
}
} /* namespace gdface */
