/*
 * feature_se.cpp
 *
 *  Created on: 2018年4月14日
 *      Author: 10km
 */
#include <iostream>
#include "feature_se.h"
#include "CodeManagerFactory.h"
using namespace gdface;
// 搜索引擎对象，需要调用初始化函数初始化才有效
static ICodeManager* instance = nullptr;
// 当前线程的错误消息
static thread_local char error_msg[256];
static string INVALID_ARGUMENT = "Invalid argument:";
static string NULL_POINTER = "null pointer:";

static void fill_error_msg(const char* msg) {
	if(nullptr != msg){
		strncpy(error_msg, msg, std::min(sizeof(error_msg), strlen(msg)));
		// 未尾截断
		error_msg[std::min(strlen(msg) + 1,sizeof(error_msg) - 1)] = 0;
	}
}
// 将MD5数组转为MD5Set对象
static MD5Set toMD5Set(const MD5 *md5Arrays, size_t md5Count) {
	MD5Set md5_set;
	if(nullptr != md5Arrays){
		for (auto i = md5Count; i > 0; --i) {
			md5_set.insert(md5Arrays[i]);
		}
	}
	return std::move(md5_set);
}
// 检查 instance是否初始化
static bool valid_instance() {
	if (nullptr != instance) {
		return true;
	}
	else {
		fill_error_msg("uninitialized instance,must call init(...) firstly to initialize feature search engine");
		return false;
	}
}
const char* fse_error_msg() {
	return error_msg;
}

int fse_init(uint32_t initCapacity, float loadFactor, size_t overBlockCapacity){
	// 初始化表对象
	try {
		if(nullptr == instance){
			cout << "feature_se jni initialized:" << endl;
			cout << "initCapacity:" << initCapacity << ";loadFactor:" << loadFactor << ";overBlockCapacity:" << overBlockCapacity << endl;
			instance = CodeManagerFactory::getICodeManagerCPU(initCapacity, true, loadFactor, overBlockCapacity);
		}
		return 0;
	}
	catch (exception &e) {
		fill_error_msg(e.what());
		return -1;
	}
}
int fse_release() {
	if (!valid_instance()) {
		return -1;
	}
	std::cout << "ICodeManager instance release" << std::endl;
	try {
		instance->release();
		instance = nullptr;
		return 0;
	}
	catch (exception &e) {
		fill_error_msg(e.what());
		return -1;
	}
}

int fse_searchCode(const face_code *code, double threshold, size_t rows, code_bean*out, const MD5 *md5Arrays,size_t md5Count) {
	if (nullptr == code) {
		fill_error_msg((NULL_POINTER + "code").c_str());
		return -1;
	}
	if (nullptr == out) {
		fill_error_msg((NULL_POINTER + "out").c_str());
		return -1;
	}
	if (!valid_instance()) {
		return -1;
	}
	try {
		MD5Set md5_set = toMD5Set(md5Arrays, md5Count);
		return (int)instance->searchCode(*code, threshold, rows, out, std::addressof(md5_set));
	}
	catch (exception &e) {
		fill_error_msg(e.what());
		return -1;
	}
}

int fse_getFeature(MD5 *md5, code_bean* out){
	if (nullptr == md5) {
		fill_error_msg((NULL_POINTER + "md5").c_str());
		return -1;
	}
	if (nullptr == out) {
		fill_error_msg((NULL_POINTER + "out").c_str());
		return -1;
	}
	if (!valid_instance()) {
		return -1;
	}
	try{
		auto result = instance->getBean(*md5);
		if (nullptr != result) {
			*out = *result;
			return 1;
		}
		return 0;
	}
	catch (exception &e) {
		fill_error_msg(e.what());
		return -1;
	}
}
int fse_addFeature(code_bean* bean){
	if (nullptr == bean) {
		fill_error_msg((NULL_POINTER + "bean").c_str());
		return -1;
	}
	if (!valid_instance()) {
		return -1;
	}
	try{
		instance->addBean(*bean);
		return 0;
	}	
	catch (exception &e) {
		fill_error_msg(e.what());
		return -1;
	}
}
int fse_addFeatures(code_bean beans[], size_t beanCount) {
	if (nullptr == beans) {
		fill_error_msg((NULL_POINTER + "beans").c_str());
		return -1;
	}
	if (!valid_instance()) {
		return -1;
	}
	int addedCount = 0;
	for (auto i = beanCount -1; i >= 0; --i, ++addedCount) {
		if (fse_addFeature(std::addressof(beans[i])) < 0) {
			return -1;
		}
	}
	return addedCount;
}
int fse_removeFeature(MD5 *md5) {
	if (nullptr == md5) {
		fill_error_msg((NULL_POINTER + "md5").c_str());
		return -1;
	}
	if (!valid_instance()) {
		return -1;
	}
	try {
		auto result = instance->removeBean(*md5);
		return result ? 1 : 0;
	}
	catch (exception &e) {
		fill_error_msg(e.what());
		return -1;
	}
}
int fse_removeFeatures(MD5 md5s[], size_t md5Count) {
	if (nullptr == md5s) {
		fill_error_msg((NULL_POINTER + "md5s").c_str());
		return -1;
	}
	if (!valid_instance()) {
		return -1;
	}
	int count = 0,result;
	for (auto i = md5Count - 1; i >= 0; --i, count += result) {
		result = fse_removeFeature(std::addressof(md5s[i]));
		if (result < 0) {
			return -1;
		}
	}
	return count;
}
int fse_removeFeaturesByImgMD5(MD5 *imgMD5) {
	if (nullptr == imgMD5) {
		fill_error_msg((NULL_POINTER + "imgMD5").c_str());
		return -1;
	}
	if (!valid_instance()) {
		return -1;
	}
	try {
		return instance->removeBeansByImgMD5(*imgMD5);
	}
	catch (exception &e) {
		fill_error_msg(e.what());
		return -1;
	}
}
int fse_size() {
	if (!valid_instance()) {
		return -1;
	}
	return instance->size();
}
const char* fse_statInfo() {
	if (!valid_instance()) {
		return nullptr;
	}
	try {
		return instance->statInfo().c_str();
	}
	catch (exception &e) {
		fill_error_msg(e.what());
		return nullptr;
	}
}
