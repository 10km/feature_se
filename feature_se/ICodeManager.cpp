/*
 * ICodeManager.cpp
 *
 *  Created on: 2015年12月7日
 *      Author: guyadong
 */
#include "ICodeManager.h"
#include "MD5SetImpl.h"
#include "gf_utility.h"
namespace gdface {
MD5Set::MD5Set(uint32_t initCapacity, bool isCopy, float loadFactor, size_t overBlockCapacity)
	:m_set(new MD5SetImpl(initCapacity,isCopy,loadFactor,overBlockCapacity)){
}
MD5Set::~MD5Set(){
	delete m_set;
}
MD5Set::MD5Set(MD5Set&&rv):m_set(rv.m_set){
	modify_const(rv.m_set,(decltype(rv.m_set))nullptr);
}
bool MD5Set::has(const MD5&key){
	return m_set->has(key);
}
const MD5& MD5Set::insert(const MD5&key){
	return m_set->insert(key);
}
} /* namespace gdface */

