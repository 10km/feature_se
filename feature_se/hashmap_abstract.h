/*
 * hashmap_abstract.h
 *
 *  Created on: 2015年10月13日
 *      Author: guyadong
 */

#ifndef FEATURE_SE_HASHMAP_ABSTRACT_H_
#define FEATURE_SE_HASHMAP_ABSTRACT_H_
#include "hashtable_abstract.h"
namespace gdface {
/*
 * 基于 HashTableAbstract实现hashmap基本功能的抽象类
 * 要求子类提供从V中可以获取K的虚函数 keyFrom
 */
template<typename K, typename V,bool CONCURRENCY=false>
class HashMap_Abstract: public HashTableAbstract<K, V,CONCURRENCY> {
protected:
	virtual const K& keyFrom(const V& value)const noexcept=0;
public:
	HashMap_Abstract(HASH_TABLE_SIZE_TYPE initCapacity=0, bool isCopy=false, float loadFactor=0, size_t overBlockCapacity=0) :
			HashTableAbstract<K, V,CONCURRENCY>::HashTableAbstract(initCapacity, isCopy, loadFactor, overBlockCapacity) {
	}
	HashMap_Abstract(HashMap_Abstract&&)=default;
	virtual ~HashMap_Abstract()=default;
	/*
	 * 根据key向表中插入一个value，返回为插入的对象指针(非复制模式下[isCopy==false],等于value),
	 */
	const V & put(const K &key, const V& value) {
		return *(V*) HashTableAbstract<K, V,CONCURRENCY>::insert(key, value).obj;
	}
	/*
	 * 向表中插入一个value，返回为插入的对象指针(非复制模式下[isCopy==false],等于value),
	 * value为nullptr则返回nullptr
	 */
	const V & put(V& value) {
		return put(keyFrom(value), value);
	}
	bool remove(K &key) {
		return HashTableAbstract<K, V,CONCURRENCY>::removeKey(key);
	}
	bool remove(V &value) {
		return HashTableAbstract<K, V,CONCURRENCY>::removeValue(value);
	}
	bool hasValue(V &value)const noexcept {
		return HashTableAbstract<K, V,CONCURRENCY>::has(keyFrom(value));
	}
	const V* get(K& key)const noexcept {
		auto node = HashTableAbstract<K, V, CONCURRENCY>::findNode(key);
		return nullptr == node ? nullptr : (V*) node->obj;
	}
};

}

#endif /* FEATURE_SE_HASHMAP_ABSTRACT_H_ */
