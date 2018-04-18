/*
 * jni_mgr.cpp
 *
 *  Created on: 2015年11月26日
 *      Author: guyadong
 */
#include <iostream>
#include "jni_mgr.h"
#include "jni_utilits.h"
using namespace gdface;
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved) {
	jni_utilits::setJVM(vm);
	std::cout << "feature_se jni loaded" << std::endl;
	return JNI_VERSION_1_6;
}

JNIEXPORT void JNICALL JNI_OnUnload(JavaVM* vm, void* reserved){
	std::cout << "feature_se jni unloaded" << std::endl;
}


