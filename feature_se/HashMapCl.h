/*
 * HashMapCl.h
 *
 *  Created on: 2015年10月13日
 *      Author: guyadong
 */

#ifndef FEATURE_SE_HASHMAPCL_H_
#define FEATURE_SE_HASHMAPCL_H_
#include <atomic>
#include <vector>
#include "hashmap_abstract.h"
#include "hashset_cl.h"
#include "TopKCodeBean.h"
namespace gdface {
class HashMapCl: public HashMap_Abstract<MD5,code_bean,true> {
protected:
	virtual inline const MD5& keyFrom(const code_bean& value)const noexcept{
		return value.id;
	}
	/* 计算MD5哈希值 */
	virtual inline HASH_TABLE_SIZE_TYPE hashcode(MD5 *obj) const noexcept{
		auto hashcode =obj->l[0]+obj->l[1];
		return (hashcode>>32)^(hashcode&0xffffffff);
	}
public:
	using MD5Set=HashSetCl<MD5,false>;	
	HashMapCl(HASH_TABLE_SIZE_TYPE initCapacity);
	HashMapCl(HashMapCl&&)=default;
	HashMapCl(HASH_TABLE_SIZE_TYPE initCapacity=0, bool isCopy=true, float loadFactor=0, size_t overBlockCapacity=0);
	virtual ~HashMapCl()=default;
	TopKCodeBean searchCode(const face_code &code, double threshold, size_t rows, const MD5Set *md5set=nullptr) const;
	auto searchCode(atomic_uint & countDown,const face_code &code, double threshold, size_t rows, const MD5Set *md5set=nullptr)
		->decltype(searchCode(code,threshold,rows,md5set)) const{
		raii guard([&countDown]{--countDown;});
		return searchCode(code,threshold,rows,md5set);
	}
	HASH_TABLE_SIZE_TYPE removeBeansByImgMD5(const MD5 &imgMD5);
	std::vector<code_bean> getBeansByImgMD5(const MD5 &imgMD5)const;
};

} /* namespace gdface */

#endif /* FEATURE_SE_HASHMAPCL_H_ */
