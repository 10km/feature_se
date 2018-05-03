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
static thread_local std::string error_msg;
static string INVALID_ARGUMENT = "Invalid argument:";
static string NULL_POINTER = "null pointer:";

static void fill_error_msg(const std::string& msg) {
	error_msg = msg;
}

static void fill_error_msg(const char* msg) {
	if(nullptr != msg){
		error_msg = msg;
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
	return error_msg.c_str();
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
		fill_error_msg(NULL_POINTER + "code");
		return -1;
	}
	if (nullptr == out) {
		fill_error_msg(NULL_POINTER + "out");
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

int fse_getFeature(const MD5 *md5, code_bean* out){
	if (is_null_MD5(md5)) {
		return 0;
	}
	if (nullptr == out) {
		fill_error_msg(NULL_POINTER + "out");
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
int fse_addFeatures(const code_bean *beans, size_t beanCount) {
	if (nullptr == beans) {
		return 0;
	}
	if (!valid_instance()) {
		return -1;
	}
	int count = 0;
	try {
		for (decltype(beanCount) i = 0; i < beanCount; ++i) {
			if (!beans[i].id.is_null()) {
				instance->addBean(beans[i]);
				++count;
			}
		}
	}
	catch (exception &e) {
		fill_error_msg(e.what());
		return -1;
	}
	return count;
}

int fse_removeFeatures(const MD5 *md5s, size_t md5Count) {
	if (nullptr == md5s) {
		return 0;
	}
	if (!valid_instance()) {
		return -1;
	}
	int count = 0;
	bool result;
	try {
		for (decltype(md5Count) i = 0; i < md5Count; ++i) {
			if (!md5s[i].is_null()) {
				result = instance->removeBean(md5s[i]);
				count += result ? 1 : 0;
			}
		}
	}
	catch (exception &e) {
		fill_error_msg(e.what());
		return -1;
	}
	return count;
}

int fse_removeFeaturesByImgMD5(const MD5 *imgMD5s, size_t md5Count) {
	if (nullptr == imgMD5s) {
		return 0;
	}
	if (!valid_instance()) {
		return -1;
	}
	try {
		int count = 0;
		for (decltype(md5Count) i = 0; i < md5Count; ++i) {
			if(!imgMD5s[i].is_null()){
				count += instance->removeBeansByImgMD5(imgMD5s[i]);
			}
		}
		return count;
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

static thread_local std::string tls_str_statinfo;
const char* fse_statInfo() {
	if (!valid_instance()) {
		return nullptr;
	}
	try {
		tls_str_statinfo = instance->statInfo();
		return tls_str_statinfo.c_str();
	}
	catch (exception &e) {
		fill_error_msg(e.what());
		return nullptr;
	}
}
