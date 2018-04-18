/*
 * hashtable_core.h
 *
 *  Created on: 2015年10月6日
 *      Author: guyadong
 */
#ifdef __cplusplus
extern "C" {
#endif
#ifndef FEATURE_SE_HASHTABLE_CORE_H_
#define FEATURE_SE_HASHTABLE_CORE_H_
#include <stdlib.h>
#include <stdint.h>
#define DEFAULT_HASHTABLE_INITCAPACITY 16
#define MAX_HASHTABLE_CAPACITY (1UL<<27)
#define DEFAULT_LOADFACTOR 0.75f
/* 哈希表节点数据结构 */
typedef struct _HashNode {
	void * obj;
	struct _HashNode *next;
	_HashNode(void *obj,struct _HashNode *next):obj(obj),next(next){}
	_HashNode():_HashNode(nullptr,nullptr){}
	_HashNode(const _HashNode&)=default;
	_HashNode(_HashNode&&)=default;
	_HashNode& operator=(const _HashNode&)=default;
	_HashNode& operator=(_HashNode&&)=default;

} HashNode;
typedef HashNode* HASH_NODE_PTR;
/* 哈希表核心数据结构 */
typedef struct _HashtableCore {
	uint32_t size; // hash 表中的元素个数
	uint32_t capacity; // hash 表容量(数组大小)
	uint32_t obj_size; //hash_node 中 obj大小
	HASH_NODE_PTR nodes; // hash表(动态数组)
} HashtableCore;
uint32_t default_hashcode(const void *obj, size_t size);
int default_equals(const void *obj1, const void *obj2, size_t size);
#endif /* FEATURE_SE_HASHTABLE_CORE_H_ */

#ifdef __cplusplus
}
#endif
