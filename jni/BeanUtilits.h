/*
 * beanutilits.h
 *
 *  Created on: 2015年11月28日
 *      Author: guyadong
 */

#ifndef JNI_BEANUTILITS_H_
#define JNI_BEANUTILITS_H_
#include <iostream>
#include <jni.h>
#include <stdexcept>
#include <vector>
#include "jni_utilits.h"
#include "codemgr.h"
namespace gdface {
class BeanUtilits {
public:
#define JAVA_STRING					"Ljava/lang/String;"
#define CODEBEAN 						"net/gdface/sdk/fse/CodeBean"
#define CODEBEAN_ID 				"id"
#define CODEBEAN_CODE 			"code"
#define CODEBEAN_IMGMD5 		"imgMD5"
#define CODEBEAN_SIMILARITY	"similarity"
	static raii_var<jobject> toJCodeBean(const code_bean& bean, jni_utilits::JavaClassMirror& mirror,jboolean full=true);
	// 将id,code,imgMD5填充到bean对应的字段
	static bool tocodeBean(code_bean& bean, jbyteArray id, jbyteArray code, jstring imgMD5);

	static bool tocodeBean(code_bean& bean,jobject obj,  jni_utilits::JavaClassMirror& mirror);
	static bool jstringToMD5(jstring jstr, MD5& md5);
	static bool jbytearraytoMD5(jbyteArray bytes, MD5& md5);
	static raii_var<jstring> MD5toJString(const MD5&md5) {
		return jni_utilits::raii_NewStringUTF(MD5toMD5_STR(md5).data());
	}
	static raii_var<jbyteArray>MD5tojbyteArray(const MD5&md5){
		return jni_utilits::tojbytearray((jbyte*) (&md5), (jsize)sizeof(MD5));
	}
	static raii_var<jbyteArray>face_codetojbyteArray(const face_code &code){
#if defined(CUSTOM_FEACOMP) || defined(CASSDK)
		return jni_utilits::tojbytearray((jbyte*)(&code), (jsize)sizeof(face_code));
#elif CODE_END_WITH_SUM
		return jni_utilits::tojbytearray((jbyte*)(&code), (jsize)sizeof(face_code));
#else
		// 特征码中不包含点积和时只将element数组转为java array
		return jni_utilits::tojbytearray((jbyte*)(&code.element), (jsize)sizeof(code.element));
#endif
	}
	static bool jbytearraytoface_code(jbyteArray bytes, face_code& code);

	/* 生成 code_bean镜像描述 */
	static jni_utilits::JavaClassMirror JavaClassMirrorForCodeBean(){
		return jni_utilits::JavaClassMirror(
				CODEBEAN,
			{ "<init>", "()V" },
			{	{CODEBEAN_ID, "[B"},
				{CODEBEAN_CODE, "[B"},
				{CODEBEAN_IMGMD5, JAVA_STRING},
				{CODEBEAN_SIMILARITY, "D"}
			});
	}

	static std::shared_ptr<vector<MD5> > jmd5settoMD5Vector(jobjectArray jset);

	static void output_md5(const MD5& md5, const char* prefix = "") {
		cout << prefix << " [" << MD5toMD5_STR(md5) << "]" << endl;
	}
	static void output_bean(const code_bean& bean, const char* prefix = "") {
		cout << prefix << " id[" << MD5toMD5_STR(bean.id) << "]img[" << MD5toMD5_STR(bean.imgMD5).data() << "]" << endl;
	}
	static raii_var<jobjectArray> code_bean_ptr_tojobjectArray(const code_bean* src, jsize size, jni_utilits::JavaClassMirror& mirror, jboolean full);
	/*static jobject toJCodeBean(JNIEnv* env, const code_bean& bean) {
		auto code_bean_class =jni_utilits::raii_FindClass_LocalRef("Lnet/gdface/facedbsdk/local/CodeCacheManager$CodeBean;");
		auto constructor = env->GetMethodID(code_bean_class.get(), "<init>", "()V");
		auto obj = env->NewObject(code_bean_class.get(), constructor);
		auto field_id = env->GetFieldID(code_bean_class.get(), "id", "[B");
		env->SetObjectField(obj, field_id, jni_utilits::tojbytearray((jbyte*) (&bean.id), (jsize) sizeof(bean.id)).get());
		auto field_code = env->GetFieldID(code_bean_class.get(), "code", "[B");
		env->SetObjectField(obj, field_code,
		jni_utilits::tojbytearray((jbyte*) &(FACE_CODE_CONVERT(bean.code)), (jsize) sizeof(bean.code)).get());
		auto field_imgMD5 = env->GetFieldID(code_bean_class.get(), "imgMD5", "Ljava/lang/String;");
		env->SetObjectField(obj, field_imgMD5, MD5toJString(bean.imgMD5).get());
		auto field_similarity = env->GetFieldID(code_bean_class.get(), "similarity", "D");
		env->SetFloatField(code_bean_class.get(), field_similarity, (jfloat) (((bean.similarity))));
		return obj;
	}*/	
};


} /* namespace gdface */

#endif /* JNI_BEANUTILITS_H_ */
