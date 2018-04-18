/*
 * hashtablebase.h
 *
 *  Created on: 2015年10月12日
 *      Author: guyadong
 */

#ifndef FEATURE_SE_HASHTABLE_ABSTRACT_H_
#define FEATURE_SE_HASHTABLE_ABSTRACT_H_
#include <iostream>
#include <sstream>
#include <cassert>
#include <string>
#include <sstream>
#include <cmath>
#include <type_traits>
#include <stdexcept>
#include <iterator>
#include <memory>
#include <bitset>
#include "gf_utility.h"
#include "hash_fun.h"
#include "hashtable_core.h"
#include "intrinsic_wrapper.h"
#include "OverTableManager.h"
#include "RWLock.h"
namespace gdface {
/*  与HashtableCore 中所有整数类型变量的类型一致 */
using HASH_TABLE_SIZE_TYPE=decltype(HashtableCore::obj_size);

/* 用于hashtable 的标准forward迭代器
 * position　first为table.capacity,second为nullptr时代表表结尾
 *  */
template<typename V>
struct _HashTable_iterator:public std::iterator<std::forward_iterator_tag,V>{
	using	_Self			=_HashTable_iterator<V>;
	using	_Node		=_HashNode;
	using	reference	=V&;
	using 	pointer		=V*;
	using   position			=std::pair<HASH_TABLE_SIZE_TYPE,HASH_NODE_PTR>;
	const HashtableCore *m_table=nullptr;
	/* 当前节点 */
	position m_cur;
	/* 当前节点的前一个节点 */
	position m_prev;
	explicit
	_HashTable_iterator(const HashtableCore &table,position pos={0,nullptr}):
		m_table(&table),
		m_cur((pos.first>table.capacity)? position{table.capacity, nullptr}:pos){
		if(nullptr==m_cur.second){
			// 定位到第一个元素
			next();
		}
		m_prev=m_cur;
	}
	_HashTable_iterator():m_table(),m_cur(),m_prev(){}
	_HashTable_iterator(const _Self &)=default;
	_Self& operator=(const _Self &)=default;
	/*
	 * ++操作符,如果m_cur已经指向表结尾,则直接返回不修改对象数据
	 * */
	_Self& operator++()noexcept{
		assert(nullptr!=m_cur.second&&nullptr!=m_table);
		// 如果已经到表尾部则跳过
		if(m_cur.first<m_table->capacity||nullptr!=m_cur.second){
			m_prev=m_cur;
			m_cur.second=m_cur.second->next;
			if(nullptr==m_cur.second){
				++m_cur.first;
				next();
			}
		}
		return *this;
	}
	_Self operator++(int)noexcept{
		_Self tmp=*this;
		this->operator++();
		return tmp;
	}

	reference operator*() const noexcept { return *static_cast<V*>(m_cur.second->obj); }
	pointer operator->() const noexcept { return static_cast<V*>(m_cur.second->obj); }
	bool operator==(const _Self& v) const noexcept { assert(m_table == v.m_table); return m_cur == v.m_cur; }
	bool operator!=(const _Self& v) const noexcept { assert(m_table == v.m_table); return m_cur != v.m_cur; }
private:
	/*
	 * 跳到下一个有效的根节点
	 * 如果后续没有元素，则m_cur.second==nullptr,m_cur.first==m_table.capacity
	 * */
	inline void next() noexcept{
		// 定位到第一个元素,如果表为空或m_cur.first>=capacity，则m_cur.second为nullptr
		for(;m_cur.first<m_table->capacity&&nullptr==(m_table->nodes+m_cur.first)->obj;++m_cur.first);
		if(m_cur.first<m_table->capacity)m_cur.second=m_table->nodes+m_cur.first;
	}
};
/* 用于hashtable 的标准forward迭代器(const) */
template<typename V>
struct _HashTable_const_iterator:public _HashTable_iterator<V>{
	using   _Base 		=_HashTable_iterator<V>;
	using	_Self		=_HashTable_const_iterator<V>;
	using	reference	=const V&;
	using 	pointer		=const V*;
	using  _Base::_Base; // 继承基类构造函数
	_HashTable_const_iterator(const _Base &base)noexcept:_Base(base){}
	_Self& operator=(const _Self &)=default;
	_Self& operator++()noexcept{
		return static_cast<_Self&>(_Base::operator ++());
	}
	_Self operator++(int)noexcept {
		_Self tmp = *this;
		this->operator++();
		return tmp;
	}

	reference operator*() const noexcept
	{ return _Base::operator *();}
	pointer operator->() const noexcept
	{ return _Base::operator ->();}

};

/* 实现哈希表的抽象类,
 * 要求子类提供从V中可以获取K的虚函数 keyFrom
 * CONCURRENCY为true 支持并行
 * CONCURRENCY为true时begin(),end()为private
 * CONCURRENCY为false时begin(),end()为public
 */
template<typename K, typename V,bool CONCURRENCY=false>
class HashTableAbstract {
	using	value_type=V;
	using	iterator=_HashTable_iterator<value_type>;
	using	const_iterator=_HashTable_const_iterator<value_type>;
	using   position=typename iterator::position;
private:
	const bool m_isCopy; // 加入元素时是否将元素复制到新分配内存中
	const float m_loadFactor; // 装载因子
	HASH_TABLE_SIZE_TYPE m_threshold; //resize阀值,当tabe.size大于此阀值时重建哈希表
	HashtableCore m_table;
	OverTableManager m_overTableManager;
	unique_ptr<HashNode[]> mu_nodes;// 指向表的内存指针,析构时自动释放内存
	using lock_type = typename std::conditional<CONCURRENCY,RWLock,bool>::type;
	mutable lock_type m_lock{true};// 并发模式(CONCURRENCY=true)时的读写锁
	/* 并发模式(CONCURRENCY=true)时返回raii对象,编译根据CONCURRENCY决定 */
	template<bool C=CONCURRENCY>
	typename std::enable_if<C,raii>::type read_guard()const noexcept{
		return m_lock.read_guard();
	}
	template<bool C=CONCURRENCY>
	typename std::enable_if<!C,lock_type>::type read_guard()const noexcept{
		return m_lock;
	}
	template<bool C=CONCURRENCY>
	typename std::enable_if<C,raii>::type write_guard()const noexcept{
		return m_lock.write_guard();
	}
	template<bool C=CONCURRENCY>
	typename std::enable_if<!C,lock_type>::type write_guard()const noexcept{
		return m_lock;
	}

	/*
	 * 从hashcode指定偏移位置的链表节点查找指定的key
	 * lastNode不为nullptr时,lastNode返回找到的节点上一个节点
	 * 返回key所在的节点,找不到key则返回nullptr
	 */
	HASH_NODE_PTR findInBucket(const K &key, HASH_TABLE_SIZE_TYPE bucket, HASH_NODE_PTR *lastNode=nullptr)const noexcept {
		assert(bucket < m_table.capacity&&nullptr != m_table.nodes[bucket].obj);
		HASH_NODE_PTR pNode=std::addressof(m_table.nodes[bucket]),last = pNode;
		for (; nullptr != pNode && !equals(keyFrom(*static_cast<V*>(pNode->obj)), key); last = pNode, pNode = pNode->next);
		if (nullptr != lastNode)
			*lastNode = last;
		return pNode;
	}

	/* 计算obj的hashcode,并根据hashcode计算偏移量 */
	inline auto bucketIndex(const K &key)const noexcept->decltype( m_table.capacity) {
		static hash_fun<K> hash_impl;// hash函数实现
		return hash_impl.fun(key) % m_table.capacity;
	}

	/* 表长度加1	*/
	inline auto incTableSize() noexcept->decltype(m_table.size){
		return ++m_table.size;
	}
	/* 表长度减1	*/
	inline auto decTableSize()noexcept->decltype(m_table.size) {
		return --m_table.size;
	}

	/*
	 * 将initCapacity向上格式化为2的N次幂整数,
	 * 最大不超过MAX_HASHTABLE_CAPACITY
	 * 最小DEFAULT_HASHTABLE_INITCAPACITY
	 */
	static auto formatInitCapacity(HASH_TABLE_SIZE_TYPE initCapacity)noexcept->decltype(initCapacity) {
		if(initCapacity <= DEFAULT_HASHTABLE_INITCAPACITY)
			return DEFAULT_HASHTABLE_INITCAPACITY;
		else{
			initCapacity= bitset<sizeof(initCapacity) * 8>(initCapacity).count() > 1 
				?1U << intrinsics::_bsr(initCapacity << 1) : initCapacity;
			// initCapacity为0时是左移溢出了
			return 0==initCapacity||initCapacity>MAX_HASHTABLE_CAPACITY?MAX_HASHTABLE_CAPACITY:initCapacity;
		}
	}
	/* 初始化表信息，根据initCapacity分配表空间
	 * initCapacity为0或<DEFAULT_HASHTABLE_INITCAPACITY时，使用默认的容量DEFAULT_HASHTABLE_INITCAPACITY
	 * */
	static void initTable(HashtableCore & table, HASH_TABLE_SIZE_TYPE initCapacity,decltype(mu_nodes) &uptr_nodes) {
		table.capacity = formatInitCapacity(initCapacity);
		table.obj_size = sizeof(K);
		table.size = 0;
		using array_type=typename decltype(mu_nodes)::element_type[];
		if(nullptr==uptr_nodes.get())
			uptr_nodes=make_unique<array_type>(table.capacity);
		table.nodes = uptr_nodes.get();
	}
	/*
	 * 右值引用版本
	 * 向表中插入一个节点pNewNode(不能为nullptr)，返回插入后的新节点指针,
	 * isOverNode为true时pNewNode是溢出表中的节点
	 */
	HashNode& _insertNode(HashNode&& newNode, bool isOverNode)noexcept {
		assert(nullptr!=newNode.obj);
		auto key=keyFrom(*static_cast<V*>(newNode.obj));
		auto bkt=bucketIndex(key);
		auto pNode = m_table.nodes+bkt;
		if (nullptr == pNode->obj) {
			// hashcode不冲突，直接加到根节点
			// 添加第一个节点
			pNode->obj = newNode.obj;
			pNode->next = nullptr;
			// 如果是溢出表节点则从溢出表中删除该节点
			if (isOverNode) {
				auto b=m_overTableManager.deleteNode(newNode);
				assert(b);
			}
			incTableSize();
		} else {
			//cout<<"conflict"<<endl;
			// hashcode冲突,则查找链表
			auto found = findInBucket(key, bkt);
			if (nullptr == found) {
				// 没有找到key,则新建一个节点
				if (isOverNode) {
					// 如果是溢出表节点则直接使用该节点
					found = &newNode;
				} else {
					// 如果是根节点则在溢出表中申请一个新节点空间
					found = m_overTableManager.newNode();
					found->obj = newNode.obj;
				}
				// 将新节点插入到根节点之后
				found->next = pNode->next;
				pNode->next = found;
				incTableSize();
			}
			pNode = found;
		}
		assert(nullptr != pNode->obj);
		return *pNode;
	}
	/*	根据newCapacity重建哈希表 */
	void resize(HASH_TABLE_SIZE_TYPE newCapacity) {
		HashtableCore newTable;
		decltype(mu_nodes) new_nodes;
		newCapacity=formatInitCapacity(newCapacity);
		if (newCapacity == m_table.capacity)
			return;
		//cout << "resize to:" << newCapacity << endl;
		try {
			initTable(newTable, newCapacity,new_nodes);
		} catch (const bad_alloc&e) {
			// 内存分配失败则返回
			cerr << e.what() << endl;
			return;
		}
		// 将newTable数据更新到对象,原table数据交换到oldTable;
		auto oldTable = this->m_table;
		this->m_table = newTable;
		this->m_threshold = (HASH_TABLE_SIZE_TYPE)(this->m_loadFactor * this->m_table.capacity); // 更新阀值
		decltype(oldTable.nodes) pBucketNode;
		for (decltype(oldTable.capacity) bucket = 0; bucket <oldTable.capacity; ++bucket) {
			pBucketNode = oldTable.nodes + bucket;
			if (nullptr != pBucketNode->obj) {
				_insertNode(std::move(*pBucketNode), false);
				for (auto pNode = pBucketNode->next; nullptr != pNode&&nullptr != pNode->obj;) {
					auto next = pNode->next; // 临时保存next，因为在_insertNode中可能会修改pNode的next值
					_insertNode(std::move(*pNode), true);
					pNode = next;
				}
			}
		}
		assert(oldTable.size==m_table.size);
		mu_nodes=std::move(new_nodes);//内存指针指向新分配的内存并删除之前的内存
	}

	/* (无锁)查找key所在的节点位置，找不到则返回nullptr  */
	position inline _find(const K & key)const noexcept{
		auto bkt=bucketIndex(key);
		return {bkt,nullptr == m_table.nodes[bkt].obj ? nullptr : findInBucket(key, bkt)};
	}
	/* (无锁)根据key向表中插入一个value，返回插入后所在节点指针 */
	auto inline _insert(const K &key, const V &value)noexcept->decltype(_insertNode(declval<HashNode>(),true)) {
		// 表大小超过重建阀值时则将表容量增加一倍重建哈希表
		if (m_table.size > m_threshold) {
			resize(m_table.capacity << 1);
		}
		//匿名对象使用右值引用版本的insertNode
		return _insertNode(HashNode{ m_isCopy?new V(value):std::addressof(const_cast<V&>(value)), nullptr }, false);
	}
	/* 删除bucket下的erased指定的节点 */
	void _remove(HASH_TABLE_SIZE_TYPE bucket,HashNode& erased)noexcept {
		// 函数结束时释放内存
		raii var([=] {
			if (m_isCopy) {
				assert(nullptr != erased.obj);
				delete (static_cast<V*>(erased.obj));
			}
		});
		HashNode&bucketNode=m_table.nodes[bucket];
		if (std::addressof(erased) == std::addressof(bucketNode)) {
			if (nullptr != bucketNode.next) {
				// 超过一个节点 将第二个节点数据复制到根节点，然后删除第二个节点
				auto next = bucketNode.next;
				bucketNode = *bucketNode.next;
				auto b = m_overTableManager.deleteNode(*next);
				assert(b);
				assert(nullptr == bucketNode.next||nullptr != bucketNode.next->obj);
			} else {
				// 只有一个节点
				bucketNode.obj = nullptr;
			}

		} else {
			HASH_NODE_PTR last;
			auto f = findInBucket(keyFrom(*static_cast<V*>(erased.obj)), bucket, &last);// 获取上一节点
			assert(nullptr != f&&nullptr != last);
			// 将上一节点与下一节点相连,然后删除当前节点
			last->next = erased.next;
			auto b=m_overTableManager.deleteNode(erased);
			assert(b);
			assert(nullptr == last->next || nullptr != last->next->obj);
		}
		
		decTableSize();
	}

	/* (无锁)删除key指定的节点,如果key不存在则返回false */
	bool _removeKey(const K &key) {
		auto pos=_find(key);
		if(nullptr!=pos.second){
			_remove(pos.first,*pos.second);
			// 表大小小于重建阀值1/4时则将表容量减小一倍重建哈希表
			if (m_table.size <(m_threshold>>2)) {
				resize(m_table.capacity >> 1);
			}
			return true;
		}
		return false;
	}
	int _count_bucket(size_t bucket)const noexcept{
		assert(bucket<m_table.capacity);
		int c=0;
		for(auto p=m_table.nodes+bucket;nullptr!=p&&nullptr!=p->obj;p=p->next,++c);
		return c;
	}
	/* 生成哈希表的统计信息,如果内存分配失败则返回错误信息 */
	string _stat_info()const {
		stringstream out;
		size_t b_empty=0,b_occupy=0,b_conflict=0;
		size_t c_sum=0;
		vector<int>v_stat;
		try{
			v_stat=vector<int>(m_table.capacity);
		}catch(const bad_alloc&e){
			// 内存分配失败则返回
			cerr << e.what() << endl;
			return string("can't generate status information,caused by ")+string(e.what());
		}
		for (auto i = m_table.capacity; i >0; --i) {
			auto c=_count_bucket(i-1);
			v_stat[i-1]=c;
			switch(c){
			case 0:
				++b_empty;
				break;
			case 1:
				++b_occupy;
				break;
			default:
				++b_conflict;
				c_sum+=c;
			}
		}
		double c_average= (0== b_conflict)? 0.0 : (double)c_sum/b_conflict;
		double c_sum_square=0;
		for (auto i = m_table.capacity; i >0; --i) {
			auto c=v_stat[i-1];
			if(c>1)
				c_sum_square+=(c-c_average)*(c-c_average);
		}
		auto c_std_dev= (0 == b_conflict) ? 0.0 : sqrt(c_sum_square/b_conflict);
		out<<"hashtable:"<<this<<""<<endl;
		out<<"{"<<endl;
		out<<"\tcapacity:"<<m_table.capacity<<",size:"<<m_table.size<<",\tratio:"<<(double)m_table.size/m_table.capacity*100<<"%"<<endl;
		out<<"\tbucket:empty   ="<<b_empty   <<",\tratio:"<<(double)b_empty/m_table.capacity*100<<"%"<<endl;
		out<<"\tbucket:occupy  ="<<b_occupy  <<",\tratio:"<<(double)b_occupy/m_table.capacity*100<<"%"<<endl;
		out<<"\tbucket:conflict="<<b_conflict<<",\tratio:"<<(double)b_conflict/m_table.capacity*100<<"%"<<endl;
		out<<"\tconflict sum   ="<<c_sum     <<",\tratio:"<<(double)c_sum/m_table.size*100<<"%"<<endl;
		out<<"\tconflict avg   ="<<c_average <<",\tconflict std_dev="<<c_std_dev<<endl;
		out<<"}"<<endl;
		return out.str();
	}
protected:
	/*
	 * 查找key所在的节点位置，找不到则返回nullptr
	 */
	decltype(m_table.nodes) findNode(const K & key) const noexcept {
		auto guard = read_guard<CONCURRENCY>();
		return _find(key).second;
	}
	/*
	 * 根据key向表中插入一个value，返回插入后所在节点指针,
	 */
	HashNode&  insert(const K &key, const V &value)noexcept{
		auto guard = write_guard<CONCURRENCY>();
		return _insert(key,value);
	}
	HashTableAbstract(HASH_TABLE_SIZE_TYPE initCapacity=0, bool isCopy=true, float loadFactor=0, size_t overBlockCapacity=0) :
			m_isCopy(isCopy),
			m_loadFactor(loadFactor > 0 && loadFactor < 1 ? loadFactor : DEFAULT_LOADFACTOR),
			m_overTableManager(overBlockCapacity){
		initTable(m_table, initCapacity,mu_nodes);
		m_threshold = (HASH_TABLE_SIZE_TYPE)(this->m_loadFactor * m_table.capacity);
	}

	/* 判断k1,k2是否相等 */
	/*bool equals(const K &k1, const K &k2)const {
		return 0 == default_equals(&k1, &k2, sizeof(K));
	}*/
	/* 判断k1,k2是否相等
	 * 如果K支持==操作符则使用==比较版本，否则使用default_equals函数进行二进制比较
	 */
	template<typename _K=K>
	typename std::enable_if<!has_equal_operator<_K>::value,bool>::type equals(const _K &k1, const _K &k2)const {
		return 0 == memcmp(&k1, &k2, sizeof(_K));
	}
	template<typename _K=K>
	typename std::enable_if<has_equal_operator<_K>::value,bool>::type equals(const _K &k1, const _K &k2)const {
		return k1==k2;
	}

	/* 删除value指定的节点,如果value不存在则返回false */
	bool removeValue(const V &value) {
		return removeKey(keyFrom(value));
	}

	/* 删除key指定的节点,如果key不存在则返回false */
	bool removeKey(const K &key) {
		auto guard=write_guard<CONCURRENCY>();
		return _removeKey(key);
	}
	/* 从V中返回K,纯虚拟函数,子类必须实现 */
	virtual const K& keyFrom(const V& value)const noexcept=0;

private:
	template<bool C = CONCURRENCY>
	typename ::enable_if<C, iterator>::type  begin () noexcept
	{ return iterator(this->m_table); }
	template<bool C = CONCURRENCY>
	typename ::enable_if<C, iterator>::type  end() noexcept
	{ return iterator(this->m_table,position(this->m_table.capacity,nullptr)); }
	template<bool C = CONCURRENCY>
	typename ::enable_if<C, const_iterator>::type  begin ()const noexcept
	{ return const_iterator(this->m_table); }
	template<bool C = CONCURRENCY>
	typename ::enable_if<C, const_iterator>::type  end()const noexcept
	{ return const_iterator(this->m_table,position(this->m_table.capacity,nullptr)); }

public:
	template<bool C = CONCURRENCY>
	typename ::enable_if<!C, iterator>::type begin () noexcept
	{ return iterator(this->m_table); }
	template<bool C = CONCURRENCY>
	typename ::enable_if<!C, iterator>::type end() noexcept
	{ return iterator(this->m_table,position(this->m_table.capacity,nullptr)); }
	template<bool C = CONCURRENCY>
	typename ::enable_if<!C, const_iterator>::type begin ()const noexcept
	{ return const_iterator(this->m_table); }
	template<bool C = CONCURRENCY>
	typename ::enable_if<!C, const_iterator>::type end()const noexcept
	{ return const_iterator(this->m_table,position(this->m_table.capacity,nullptr)); }

public:
	bool has(const K & key)const noexcept{
		return nullptr != findNode(key);
	}
	/* 返回表中元素个数 */
	decltype(m_table.size) size()const noexcept{
		return m_table.size;
	}
	bool empty()const noexcept{
		return 0==size();
	}
	decltype(m_table.capacity) capacity()const noexcept{
		return m_table.capacity;
	}
	/* for_each 函数类型 */
	using for_each_fun=std::function<bool(V&)noexcept>;
	using for_each_fun_const=std::function<bool(const V&)noexcept>;
	/* 遍历所有节点执行指定的函数fun,当fun返回true时,中止遍历 */
	inline void for_each_break_if(for_each_fun_const &&fun)const{
		auto guard = read_guard<CONCURRENCY>();
		for(auto itor=begin<CONCURRENCY>(),_end=end<CONCURRENCY>();itor!=_end;++itor){
			if(fun(*itor))
				break;
		}
	}
	/* 遍历所有节点执行指定的函数fun,当fun返回true时,中止遍历 */
	inline void for_each_break_if(for_each_fun &&fun){
		auto guard = write_guard<CONCURRENCY>();
		for(auto itor=begin<CONCURRENCY>(),_end=end<CONCURRENCY>();itor!=_end;++itor){
			if(fun(*itor))
				break;
		}
	}
	/* 遍历表中所有节点, fun返回为true时删除该节点 */
	inline HASH_TABLE_SIZE_TYPE erase_if(for_each_fun &&fun){
		auto guard = write_guard<CONCURRENCY>();
		HASH_TABLE_SIZE_TYPE count=0;
		for(auto itor=begin<CONCURRENCY>(),_end=end<CONCURRENCY>();itor!=_end;++itor){
			if(fun(*itor)){
				itor=erase(itor);
				++count;
			}
		}
		return count;
	}
	/* 删除itor指定的节点，返回指向itor前一个的节点的迭代器(itorator) */
	iterator erase(iterator &itor){
		assert(itor.m_table == std::addressof(this->m_table));
		_remove(itor.m_cur.first,*(itor.m_cur.second));
		return iterator(m_table,itor.m_prev);
	}
	/* 删除itor指定的节点，返回指向itor前一个的节点的迭代器(const_iterator) */
	const_iterator erase(const_iterator &itor){
		return const_iterator(erase(static_cast<iterator&>(itor)));
	}
	/* 生成哈希表统计信息 */
	string  stat_info()const noexcept{
		auto guard = read_guard<CONCURRENCY>();
		return _stat_info();
	}

	/* 允许移动构造 */
	HashTableAbstract(HashTableAbstract&& )=default;
	virtual ~HashTableAbstract() {
		if (m_isCopy) {
			// 遍历每个节点,释放所有分配的空间
			for_each_break_if([](const V& v) {
				assert(nullptr != std::addressof(v));
				delete (static_cast<const V*>(std::addressof(v)));
				return false;
			});
		}
	}
};

} //end namespace gdface
#endif /* FEATURE_SE_HASHTABLEABSTRACT_H_ */
