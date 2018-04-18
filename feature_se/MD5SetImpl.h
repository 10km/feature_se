/*
 * MD5SetImpl1.h
 *
 *  Created on: 2015年12月7日
 *      Author: guyadong
 */

#ifndef FEATURE_SE_MD5SETIMPL_H_
#define FEATURE_SE_MD5SETIMPL_H_
#include "hashset_cl.h"
#include "codemgr.h"
namespace gdface {

class MD5SetImpl:public HashSetCl<MD5,false>{
public:
	MD5SetImpl(uint32_t initCapacity=0, bool isCopy=false, float loadFactor=0.0, size_t overBlockCapacity=0):HashSetCl<MD5,false>(initCapacity, isCopy, loadFactor, overBlockCapacity){}
	MD5SetImpl(MD5SetImpl&&)=default;
	virtual ~MD5SetImpl()=default;
};

} /* namespace gdface */

#endif /* FEATURE_SE_MD5SETIMPL_H_ */
