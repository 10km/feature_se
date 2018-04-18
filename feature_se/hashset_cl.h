/*
 * hashsetm_cl.h
 *
 *  Created on: 2015年10月12日
 *      Author: guyadong
 */

#ifndef FEATURE_SE_HASHSET_CL_H_
#define FEATURE_SE_HASHSET_CL_H_
#include "hashtable_abstract.h"
namespace gdface {
/* hashset实现模板*/
template<typename K,bool CONCURRENCY=false>
class HashSetCl: public HashTableAbstract<K, K,CONCURRENCY> {
public:
	HashSetCl(HASH_TABLE_SIZE_TYPE initCapacity=0, bool isCopy=false, float loadFactor=0.0, size_t overBlockCapacity=0) :
		HashTableAbstract<K, K,CONCURRENCY>::HashTableAbstract(initCapacity, isCopy, loadFactor, overBlockCapacity) {
	}
	HashSetCl(HASH_TABLE_SIZE_TYPE initCapacity, bool isCopy) :
		HashSetCl(initCapacity, isCopy, 0.0, 0) {

	}
	HashSetCl(HashSetCl&&rv)=default;
	virtual ~HashSetCl()=default;
	/*
	 * 向表中插入一个key
	 */
	const K& insert(const K &key){
		return *(K*)HashTableAbstract<K,K,CONCURRENCY>::insert(key,key).obj;
	}
	bool remove(const K &key) {
		return HashTableAbstract<K,K,CONCURRENCY>::removeKey(key);
	}
protected:
	virtual const K& keyFrom(const K& value)const noexcept{
		return value;
	}
};

}

#endif /* FEATURE_SE_HASHSET_CL_H_ */
