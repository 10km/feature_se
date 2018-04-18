/*
 * OverTableManager1.cpp
 *
 *  Created on: 2015年10月12日
 *      Author: guyadong
 */
#include <cassert>
#include "OverTableManager.h"

namespace gdface {

OverTableManager::OverTableManager(size_t tableCapacity):TABLECAPACITY(tableCapacity) {
}
/*
 * 分配一个新的节点空间
 */
HASH_NODE_PTR OverTableManager::newNode() {
	HASH_NODE_PTR node=nullptr;
	// 遍历内存表，寻找空闲的内存块
	for(auto count=m_overList.size();count>0;--count){
		auto &front=m_overList.front();
		node=front.newNode();
		if(front.full()){
			//将全满的对象放到链表最后,减少下次查找次数
			m_overList.push_back(std::move(front));
			m_overList.pop_front();
		}
		if(nullptr!=node)
			return node;
	}

	//cout << "new block:"<<m_overList.size() << endl;
	// 找不到空闲的内存时，就新分配一块内存加到链表头部
	m_overList.emplace_front(TABLECAPACITY);
	node=m_overList.front().newNode();
	assert(nullptr!=node);
	return node;
}

/* 删除一个节点,将之指向的内存置为空闲状态,并清理表中多余的空块 */
bool OverTableManager::deleteNode(HashNode& node) noexcept {
	auto hasEmpty=false;
	auto sucess = false;
	for(auto itor=m_overList.begin(),end=m_overList.end();itor!=end;){
		sucess=itor->deleteNode(node);
		if(itor->empty()){
			if(hasEmpty){
				// 存在超过一个空块，就删除当前块，始终保持只有一个空块
				itor=m_overList.erase(itor);
				//cout << "empty block "<<"deleted,left"<<m_overList.size()<<endl;
				// itor已经指向下一个元素，所以不能再执行++操作，直接进行下一次循环
				if (sucess)break;
				continue;
			}else{
				hasEmpty=true;
				//cout << "first empty block" << endl;
			}
		}
		if(sucess)break;
		++itor;
	}
	//this->print_info();
	return sucess;
}

} /* namespace gdface */
