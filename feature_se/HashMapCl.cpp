/*
 * HashMapCl.cpp
 *
 *  Created on: 2015年10月13日
 *      Author: guyadong
 */
#include <cassert>
#include <stdexcept>
#include "HashMapCl.h"
#ifdef CASSDK
#include "feature_compare_cas.h"
#elif defined(CUSTOM_FEACOMP)
#include "custom_feature_compare.h"
#else
#include "feature_compare.h"
#endif
namespace gdface {

HashMapCl::HashMapCl(HASH_TABLE_SIZE_TYPE initCapacity, bool isCopy, float loadFactor, size_t overBlockCapacity)
	:HashMap_Abstract<MD5,code_bean,true>(initCapacity, isCopy, loadFactor, overBlockCapacity){
}
HashMapCl::HashMapCl(HASH_TABLE_SIZE_TYPE initCapacity)
	:HashMapCl(initCapacity, true, 0, (size_t)(initCapacity*0.1f)){
}
/* 通过特征码比对查找表中与code相似度大于阀值threshold(>0&&<=1.0)的前rows个code_bean,
 * imgMD5Filter不为nullptr时,只对code_bean.imgMD5包含在imgMD5Filter集合的对象进行特征值比对,否则全表查找
 * 返回的对象中包含搜索结果
 */
TopKCodeBean HashMapCl::searchCode(const face_code &code, double threshold, size_t rows, const MD5Set *imgMD5Filter)const{
	if(!(rows>0&&threshold>0.0&&threshold<=1.0))
		throw invalid_argument("argument not match with rows>0&&threshold>0.0&&threshold<=1.0");
	TopKCodeBean top(rows, threshold);
	if(nullptr==imgMD5Filter||imgMD5Filter->empty()){
		for_each_break_if([&](const code_bean &node)->bool{
			top.insert(node, feature::compare(node.code, code));
			return false;
		});
	}else{
		for_each_break_if([&](const code_bean &node)->bool{
			if(imgMD5Filter->has(node.id))
				top.insert(node, feature::compare(node.code, code));
			return false;
		});
	}
	return top;
}
HASH_TABLE_SIZE_TYPE HashMapCl::removeBeansByImgMD5(const MD5 &imgMD5){
	return erase_if([&imgMD5](code_bean& bean) {return imgMD5==bean.imgMD5; });
}
std::vector<code_bean> HashMapCl::getBeansByImgMD5(const MD5 &imgMD5)const{
	vector<code_bean> v;
	for_each_break_if([&](const code_bean&bean){
		if(imgMD5==bean.imgMD5)
			v.push_back(bean);
		return false;
	});
	return v;
}
} /* namespace gdface */
