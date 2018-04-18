/*
 * topk_base.h
 *
 *  Created on: 2015年10月22日
 *      Author: guyadong
 */

#ifndef FEATURE_SE_TOPK_BASE_H_
#define FEATURE_SE_TOPK_BASE_H_
#include <cstdlib>
#include <cstring>
#include <cassert>
#include <iostream>
#include <sstream>
#include <memory>
#include <functional>
#include <type_traits>
#include "gf_utility.h"
using namespace std;
namespace gdface {
/* 计算比较函数的返回值类型 */
template<typename N>
struct compare_result{
	using type=typename std::conditional	<std::is_arithmetic<N>::value,decltype(declval<N>()-declval<N>()),int>::type;
};

/* 计算比较器对象(Callable Object)的类型 */
template<typename N>
struct compartor{
	using type=std::function<typename gdface::compare_result<N>::type(N,N)const noexcept>;
};
/* 定义比较器类型,并根据N,CMP,DEC选择合适的比较器类型
 * dec为降序,inc为升序
  */
template <typename N,typename compartor<N>::type *CMP,bool DEC>
struct comparator{
	/* 缺省的算术比较器,只适用于arithmetic类型 */
	struct arithmetic{
		struct dec{
			typename compare_result<N>::type inline value(N n1,N n2) const noexcept{return n1-n2;};
		};
		struct inc{
			typename compare_result<N>::type inline value(N n1,N n2) const noexcept{return n2-n1;};
		};
	};
	/* 使用CMP提供的自定义比较器  */
	struct other{
		struct dec{
			static_assert(nullptr != CMP,"nullptr==CMP");
			const typename compartor<N>::type&_comparator = *CMP;
			typename compare_result<N>::type inline value (N n1,N n2)const noexcept {return _comparator(n1-n2);};
		};
		struct inc{
			static_assert(nullptr != CMP, "nullptr==CMP");
			const typename compartor<N>::type&_comparator = *CMP;
			typename compare_result<N>::type inline value (N n1,N n2)const noexcept {return _comparator(n2-n1);};
		};
	};
	using type=typename std::conditional<
			nullptr!=CMP,
			typename std::conditional<DEC,typename other::dec,typename other::inc>::type,
			typename std::conditional<DEC,typename arithmetic::dec,typename arithmetic::inc>::type
			>::type;
};
template <typename T,typename N,typename compartor<N>::type *CMP=nullptr,bool DEC=true,typename Enable=void>
class topk_base;//通例

/*
 * topk排序(不稳定排序)类模板,
 * 对象内部有一个容量为m_capacity的排序缓冲区m_sort(初始化时生成),
 * 用insert方法每次加入一个要排序的对象T,当T的排序值N大于阀值时，将T按排序值插入m_sort(始终是一个有序数组)
 * T为要排序的对象类型,
 * N为排序依据的值类型,
 * CMP为比较器函数对象(Callable Object)指针
 * DEC为排序类型，true为降序,
 * N为数字/算术(arithmetic)类型时,CMP可以为nullptr,否则必须提供CMP,
 * 非算术类型的CMP函数返回值为int,函数签名int(N n1,N n2),
 * 如果希望n1要排在n2前面,则返回>0的整数,n1==n2,返回0,否则小0;
 * 特例: 禁止N为bool类型,比较值为bool类型无意义
 * + 操作符 将两个对象合并排序成新对象
 * += 操作符将参数对象合并到当前对象
 * 调用result()后会置为0，对象就不能再执行添加(insert)和合并(merge)操作
 */
template <typename T,typename N,typename compartor<N>::type *CMP,bool DEC>
class topk_base<T,N,CMP,DEC,typename std::enable_if<!is_same<N,bool>::value>::type>{
	//静态断言:N为非算术类型时,CMP不能为nullptr
	static_assert(std::is_arithmetic<N>::value||nullptr!=CMP,"FUN must not be nullptr while N is not arithmetic");
public:
	using _Self=topk_base<T,N,CMP,DEC>;
	using SIZE_TYPE =size_t;
	/* 排序节点 */
	struct CompareNode{
		const T* t;
		N  n;
	};
protected:
private:
	/* 初始阀值 */
	const N m_threshold;
	/* 排序缓冲区容量 */
	const SIZE_TYPE m_capacity;
	/* 排序缓冲区 */
	unique_ptr<CompareNode[]> m_sort;
	/* 缓冲区中已经排序元素个数,初始为零 */
	SIZE_TYPE m_size=0;
	/* 当前阀值指针,初始值指向m_threshold */
	const N *mp_threshold=std::addressof(m_threshold);
public:
	using M_SIZE_TYPE=decltype(m_size);
private:
	/* 计算比较器类型 */
	using _CMP_TYPE=typename comparator<N,CMP,DEC>::type;
	/* 比较器对象 _compatator.value(n1,n2)返回值>0则n1排在n2前面 */
	_CMP_TYPE _compatator;
	/* 二分法获取新排序节点node在排序缓冲区的排序位置(循环) */
	auto rank(const CompareNode &node, SIZE_TYPE from, SIZE_TYPE size)const noexcept->decltype(from){
		auto start=from;
		auto len=size;
		decltype(len>>1) mid;
		typename compare_result<N>::type cmp_res;
		do{
			if(len>1){
				mid=len >> 1;
				cmp_res=_compatator.value(node.n, m_sort[start+mid].n);
				if(cmp_res>0){
					len-=mid;// 前半部搜索
				}else	if(cmp_res<0){
					start+=++mid,len-=mid; // 后半部搜索
				}else
					return start+mid;
			}else if(len==1){
				return _compatator.value(node.n,m_sort[start].n)>=0?start:start+1;
			}else
				return start;
		}while(true);
	}
	/* 更新当前阀值指针 */
	void inline update_threshold()noexcept{
		if (m_capacity > 0 && m_size == m_capacity) {
			mp_threshold=&m_sort[m_size - 1].n;//当排序缓冲区满的时候,用最后一个节点的值更新当前阀值指针
		}
	}
	/* 断言两个对象类型相同 */
	static void inline assert_same_type(const topk_base &t1,const topk_base &t2) {
		if(t1.m_capacity!=t2.m_capacity||t1.m_threshold!=t2.m_threshold)
			throw invalid_argument("t1 has different type with t2");
	}
public:
	/* 构造函数 */
	topk_base(SIZE_TYPE maxRows,N threshold) noexcept:
		m_capacity(maxRows),
		m_threshold(threshold),
		m_sort(maxRows>0?make_unique<CompareNode[]>(maxRows):unique_ptr<CompareNode[]>()){
	}
	/* 默认构造函数 */
	topk_base():_Self((SIZE_TYPE)0,N()){}
	/* 复制构造函数 */
	/*topk_base(const topk_base& v):_Self(v.m_capacity,v.m_threshold){
		m_size=v.m_size;
		memcpy(m_sort.get(),v.m_sort.get(),sizeof(CompareNode)*v.m_size);//只复制有效数据
		update_threshold();
	};*/
	/* 移动构造函数 */
	topk_base(topk_base&& rv)noexcept=default;
	/* 移动赋值操作符 */
	topk_base& operator=(topk_base&& rv) {
		modify_const(m_capacity,rv.m_capacity);
		modify_const(m_threshold,rv.m_threshold);
		m_size=rv.m_size;
		m_sort=std::move(rv.m_sort);
		mp_threshold=std::addressof(m_threshold);
		update_threshold();
		return *this;
	};

	virtual ~topk_base()=default;

	/* 将obj加入topk,如果cmpValue小于当前阀值,则返回false */
	bool insert(const T& obj,const N cmpValue)noexcept {
		auto c=_compatator.value(cmpValue,*mp_threshold);
		// 大于阀值才进一步执行比较排序，小于阀值则返回
		if(c<0||m_capacity==0||(c==0&&m_size==m_capacity))
			return false;
		//将排序节node点加入排序缓冲区
		CompareNode node{std::addressof<const T>(obj),cmpValue};
		auto offset = rank(node, 0, m_size);
		auto sort=m_sort.get();
		// 在offset位置插入节点,offset(含)之后的数据向后移一位
		memmove(sort + offset + 1, sort + offset,
				sizeof(CompareNode) * (m_size < m_capacity ? m_size++ - offset : m_capacity - offset-1));
		sort[offset] = node;
		update_threshold();
		return true;
	}

	M_SIZE_TYPE size()const noexcept{return m_size;}
	/* 返回排序结果缓冲区的引用,执行本函数后，再执行insert/merge将无效 */
	auto result() noexcept->typename std::add_lvalue_reference<decltype(m_sort)>::type{
		modify_const(m_capacity,(decltype(m_capacity))0);//将常量m_capacity置为0，阻止添加添加(insert)和合并(merge)操作
		return m_sort;
	}
	/* 排序缓冲区清零 */
	void reset(){
		m_size=0;
		mp_threshold=&m_threshold;
	}

	/* 将from中排序的数据合并到当前对象中
	 * buf_in为排序用的临时对象,可以为nullptr,如果不为nullptr,类型必须与当前对象相同
	 * 返回当前对象引用
	 */
	topk_base& merge(const topk_base &from,topk_base * buf_in=nullptr) noexcept{
		if(0<from.m_size){
			if(nullptr==buf_in||!buf_in->m_sort){
				// buf_in为nullptr时只需要右值拷贝给this
				*this=merge_to(from,{this->m_capacity,this->m_threshold});
			}else{
				buf_in->reset();
				merge_to(from,std::move(*buf_in));
				// buf_in不为nullptr时将this与buf_in交换
				std::swap(*this,*buf_in);
			}
		}
		return *this;
	}

	/* 将当前对象和from合并排序到目标对象to
	 * 返回to右值引用
	 */
	topk_base&& merge_to(const topk_base &from,topk_base &&to) {
		assert_same_type(from,*this);
		assert_same_type(from,to);
		auto buf = to.m_sort.get();
		typename std::remove_const<decltype(m_capacity)>::type idx = 0;
		M_SIZE_TYPE idx_this = 0, idx_from = 0;
		decltype(rank(m_sort[0],0,0)) offset_this,offset_from;
		while ( idx < m_capacity&&idx_this < m_size&&idx_from < from.m_size) {
			offset_this=rank(from.m_sort[idx_from],idx_this,m_size-idx_this);
			if(offset_this==idx_this){
				offset_from = from.rank(m_sort[idx_this], idx_from + 1, from.m_size - idx_from - 1);
				auto copy_size=min(this->m_capacity-idx,offset_from-idx_from);
				memcpy(buf+idx,from.m_sort.get()+idx_from,copy_size*sizeof(CompareNode));
				idx+=copy_size;
				idx_from += copy_size;
				if (idx < m_capacity) {
					buf[idx++] = m_sort[idx_this++];
				}
			}else{
				auto copy_size=min(this->m_capacity-idx,offset_this-idx_this);
				memcpy(buf+idx,m_sort.get()+idx_this,copy_size*sizeof(CompareNode));
				idx+=copy_size;
				idx_this += copy_size;
				if (idx < m_capacity) {
					buf[idx++] = from.m_sort[idx_from++];
				}
			}
		}
		if (idx < m_capacity) {
			if (idx_from < from.m_size)
				memcpy(buf + idx, from.m_sort.get() + idx_from, sizeof(CompareNode) * min(m_capacity-idx, from.m_size - idx_from));
			else if (idx_this < this->m_size)
				memcpy(buf + idx, m_sort.get()         + idx_this,   sizeof(CompareNode) * min(m_capacity-idx, this->m_size - idx_this));
		}
		to.m_size = min(to.m_capacity, from.m_size + m_size);
		to.update_threshold();
		return std::move(to);
	}

	/* += 操作符 将参数对象合并到当前对象 */
	topk_base&	operator+=(const topk_base &from) noexcept{
		return merge(from);
	}
	/* + 操作符 将两个对象合并排序成新对象 */
	topk_base  operator+(const topk_base &from) noexcept{
		return merge_to(from,{this->m_capacity,this->m_threshold});
	}
	string debug()const {
		stringstream debugstr;
		for (decltype (m_size)i = 0; i < m_size; ++i) {
			debugstr << "["<<i<<"]" << m_sort[i].n << endl;
		}
		return debugstr.str();
	}
};/* topk_base */
}/* namespace gdface */

#endif /* FEATURE_SE_TOPK_BASE_H_ */
