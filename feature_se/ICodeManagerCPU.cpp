/*
 * ICodeManagerCPU.cpp
 *
 *  Created on: 2015年10月14日
 *      Author: guyadong
 */
#include <mutex>
#include <future>
#include <iostream>
#include <sstream>
#include "ICodeManagerCPU.h"
#include "MD5SetImpl.h"

namespace gdface {

ICodeManagerCPU::ICodeManagerCPU(HASH_TABLE_SIZE_TYPE initCapacity, bool isCopy, float loadFactor, size_t overBlockCapacity):m_pool(){
	//初始化所有并行表
	for(auto i = CONCURRENCY;i>0;--i){
		m_maps.emplace_back(initCapacity/CONCURRENCY,isCopy, loadFactor, overBlockCapacity);
	}
}
ICodeManagerCPU::~ICodeManagerCPU()=default;
void ICodeManagerCPU::_release(){
	static std::once_flag oc;
	if(nullptr != m_instance){
		std::call_once(oc, [] {  delete m_instance;m_instance=nullptr;});
	}
}

void ICodeManagerCPU::init(HASH_TABLE_SIZE_TYPE initCapacity, bool isCopy, float loadFactor, size_t overBlockCapacity) {
	static std::once_flag oc;
	std::call_once(oc, [&] {  m_instance = new ICodeManagerCPU(initCapacity, isCopy, loadFactor, overBlockCapacity);});
}

uint32_t ICodeManagerCPU::size()const noexcept{
	assert(m_maps.size()>0);
	auto count=m_maps[0].size();
	for(auto i=m_maps.size();i>1;--i){
		count+=m_maps[i-1].size();
	}
	return count;
}
inline void ICodeManagerCPU::release() {
	_release();
}
inline const code_bean& ICodeManagerCPU::addBean(const code_bean& bean){
	return m_maps[mapIndexOf(bean.id)].put(bean);
}

inline const code_bean* ICodeManagerCPU::getBean(const MD5 &md5)const noexcept{
	return m_maps[mapIndexOf(md5)].get(md5);
}
size_t ICodeManagerCPU::getBeansByImgMD5(const MD5 &imgMD5,out_fun out)const{
	if(nullptr == out){
		throw std::invalid_argument("the argument fun must not be nullptr");
	}
	using return_type=decltype(m_maps[0].getBeansByImgMD5(imgMD5));
	std::vector< std::future<return_type> > futures(m_maps.size());
	// 向线程池加入任务，并行查找
	for (auto i = futures.size(); i > 0; --i){
		futures[i - 1] = m_pool.enqueue([this,imgMD5,i](){ return m_maps[i - 1].getBeansByImgMD5(imgMD5);});
	}
	size_t count=0;
	for (auto i = futures.size(); i > 0; --i){
		auto v=futures[i - 1].get();
		for( auto const b:v)out(b);
		count+=v.size();
	}
	return count;
}
inline bool ICodeManagerCPU::removeBean(const MD5 &md5){
	return m_maps[mapIndexOf(md5)].remove(md5);
}
HASH_TABLE_SIZE_TYPE ICodeManagerCPU::removeBeansByImgMD5(const MD5 &imgMD5){
	std::vector< std::future<HASH_TABLE_SIZE_TYPE> > futures(m_maps.size());
	// 向线程池加入任务，并行删除
	for (auto i = futures.size(); i > 0; --i){
		futures[i - 1] = m_pool.enqueue([this,imgMD5,i](){ return m_maps[i - 1].removeBeansByImgMD5(imgMD5);});
	}
	HASH_TABLE_SIZE_TYPE count=0;
	for (auto i = futures.size(); i > 0; --i){
		count+=futures[i - 1].get();
	}
	return count;
}
std::string ICodeManagerCPU::statInfo(){
	stringstream out;
	for(auto i=m_maps.size();i>0;--i){
		out<<"map["<<i-1<<"]"<<endl;
		out<<m_maps[i-1].stat_info();
	}
	return out.str();
}
TopKCodeBean ICodeManagerCPU::searchCode(const face_code &code, TopKCodeBean::cmp_type threshold, size_t rows, const MD5Set *md5set)const {
	std::vector< std::future<TopKCodeBean> > futures(m_maps.size());
	// 向线程池加入搜索任务，并行搜索
	for (auto i = futures.size(); i > 0; --i) {
		futures[i - 1] = const_cast<MiniThreadPool&>(m_pool).enqueue(
			[code, threshold, rows, md5set, i, this]() {
			//cout<<"working m_maps["<<i-1<<"]"<<endl;
			return this->m_maps[i - 1].searchCode(code, threshold, rows,
				nullptr == md5set ? nullptr : static_cast<HashMapCl::MD5Set*>(md5set->m_set));
		});

	}
	// 合并排序结果
	TopKCodeBean top(rows, threshold), buf(rows, threshold);
	for (auto i = futures.size(); i > 0; --i) {
		top.merge(futures[i - 1].get(), std::addressof(buf));
	}
	return top;
}
size_t ICodeManagerCPU::searchCode(const face_code &code, double threshold, size_t rows, code_bean*out, const MD5Set *md5set)const{
	assert(nullptr!=out);
	auto v=searchCode(code,threshold,rows,md5set);
	v.result_to(out);
	return v.size();
}
ICodeManagerCPU* ICodeManagerCPU::m_instance=nullptr;
const ICodeManagerCPU::GC ICodeManagerCPU::gc;
const decltype(std::thread::hardware_concurrency()) ICodeManagerCPU::CONCURRENCY=std::thread::hardware_concurrency();
} /* namespace gdface */
