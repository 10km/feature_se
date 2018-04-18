/*
 * JNIContext.h
 *
 *  Created on: 2015年11月26日
 *      Author: guyadong
 */

#ifndef JNI_JNICONTEXT_H_
#define JNI_JNICONTEXT_H_
#include "raii.h"
#include "jni_utilits.h"
namespace gdface {

class JNIContext {
public:
	jni_utilits::JavaClassMirror m_code_bean_mirror;
	JNIContext(jni_utilits::JavaClassMirror&& jcm) :
			m_code_bean_mirror(std::move(jcm)) {
	}
	~JNIContext()=default;
};

} /* namespace gdface */

#endif /* JNI_JNICONTEXT_H_ */
