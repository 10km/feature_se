/*
 * jni_mgr.h
 *
 *  Created on: 2015年11月26日
 *      Author: guyadong
 */

#ifndef JNI_JNI_MGR_H_
#define JNI_JNI_MGR_H_
#include <jni.h>
#ifdef __cplusplus
extern "C" {
#endif
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved);
JNIEXPORT void JNICALL JNI_OnUnload(JavaVM* vm, void* reserved);

#ifdef __cplusplus
}
#endif
#endif /* JNI_JNI_MGR_H_ */
