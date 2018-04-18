/*
 * OverTableManager1.h
 *
 *  Created on: 2015年10月12日
 *      Author: guyadong
 */

#ifndef FEATURE_SE_OVERTABLEMANAGER_H_
#define FEATURE_SE_OVERTABLEMANAGER_H_
#include <list>
#include <cstdlib>
#include <cstring>
#include <memory>
#include <type_traits>
#include <iostream>
#include <vector>
#include <bitset>
#include "gf_utility.h"
#include "hashtable_core.h"
#include "intrinsic_wrapper.h"
using namespace std;
namespace gdface {

#define MIN_BLOCK_BIT 6
#define MIN_BLOCK_SIZE (1U<<MIN_BLOCK_BIT)
#define DEFAULT_TABLE_CAPACITY MIN_BLOCK_SIZE
/* 溢出表管理器 */
class OverTableManager {
private:
	/* 溢出表对象 */
	class OverTableInfo {
		using OM_ELEMENT =uint64_t;
		vector<HashNode> m_overTable;
		size_t m_free; // 空闲块数目
		vector<OM_ELEMENT> m_occupyMap; // overTable的空闲/占用状态表,每一位代表一个HashNode,1代表空闲，0,代表占用
		// 调整capacity尺寸为MIN_BLOCK_SIZE的整数倍
		size_t FORMAT_CAPACITY(size_t capacity)const noexcept{
			return capacity > DEFAULT_TABLE_CAPACITY ?
					(capacity + MIN_BLOCK_SIZE - 1)
							& (~(MIN_BLOCK_SIZE - 1)) :
					DEFAULT_TABLE_CAPACITY;
		}
public:
		explicit OverTableInfo(size_t capacity) :
				m_overTable(FORMAT_CAPACITY(capacity))// 分配的内存块初始化为0
				,m_free(m_overTable.size())
				,m_occupyMap(m_overTable.size() >> MIN_BLOCK_BIT){
			// map内存块所有位初始化为1
			memset(m_occupyMap.data(), -1, m_occupyMap.size() * sizeof(OM_ELEMENT));
		}
		/* 允许移动构造 */
		OverTableInfo(OverTableInfo&& rv)=default;
		/*
		 * 返回第一个找到的空闲内存块,并将该内存块置为占用
		 * 找不到返回nullptr
		 */
		HASH_NODE_PTR newNode()noexcept {
			if(!full()){
				auto offset = -1;
				for (size_t seg=0,size=m_occupyMap.size(); seg < size; ++seg) {
					auto &om=m_occupyMap[seg];
					if ((offset = intrinsics::_bsr(om)) >= 0) {
						assert(offset<(sizeof(OM_ELEMENT)<<3));
						om &=  ~(UINT64_C(1) << offset); //将该位置0(占用)
						//om=bitset<64>(om).set(offset,false).to_ullong();
						assert(((seg << 6) | offset) < m_overTable.size());
						assert(m_free>0);
						--m_free;
						return m_overTable.data()+((seg << 6) | offset);
					}
				}
			}
			return nullptr;
		}
		bool deleteNode(HashNode& node)noexcept {
			assert(nullptr!=std::addressof(node));
			if(!empty()){
				// 判断指针是否在本块内存
				auto dist=std::addressof(node) - m_overTable.data();
				if (dist>=0 &&(size_t)dist <m_overTable.size()) {
					auto seg = dist >> MIN_BLOCK_BIT;
					auto offset = dist & (MIN_BLOCK_SIZE - 1);
					assert(offset<64&&offset>=0);
					auto &om=m_occupyMap[seg];
					om |= om | (UINT64_C(1) << offset); // 将该位置1(空闲)
					node.next=nullptr,node.obj=nullptr;					
					++m_free;
					assert(m_free<=m_overTable.size());
					return true;
				}
			}
			return false;
		}
		bool full()const noexcept {return 0==m_free;}
		bool empty()const noexcept{return m_overTable.size()==m_free;}
		size_t free()const noexcept {return m_free;}
	};
	const size_t TABLECAPACITY;
	list<OverTableInfo>  m_overList; // 公共溢出区
public:
	HASH_NODE_PTR newNode();
	bool deleteNode(HashNode& node)noexcept;
	OverTableManager(size_t capacity);
	void print_info()const{
		int count=0;
		cout<<"om:["<<this<<"]";
		for(auto &node: m_overList){
			cout<<count++<<":"<<node.free()<<"/"<< TABLECAPACITY<<"," ;
		}
		cout<<endl;
	}
};
} /* namespace gdface */

#endif /* FEATURE_SE_OVERTABLEMANAGER_H_ */
