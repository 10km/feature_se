/*
 * FaceCodeManager.h
 *
 *  Created on: 2015年10月14日
 *      Author: guyadong
 */

#ifndef FEATURE_SE_ICODEMANAGERCPU_H_
#define FEATURE_SE_ICODEMANAGERCPU_H_
#include <cstdlib>
#include <atomic>
#include <vector>
#include "HashMapCl.h"
#include "ICodeManager.h"
#include "MiniThreadPool.h"
namespace gdface {

class ICodeManagerCPU:public ICodeManager {
private:
	static ICodeManagerCPU* m_instance;
	vector<HashMapCl> m_maps;
	mutable MiniThreadPool m_pool;
	/* CPU核数 */
	static const decltype(std::thread::hardware_concurrency()) CONCURRENCY;
	ICodeManagerCPU(HASH_TABLE_SIZE_TYPE initCapacity, bool isCopy, float loadFactor, size_t overBlockCapacity);
	class GC {
	public:
		~GC() noexcept{
			// singleton模式下自动销毁实例
			_release();
		}
	};
	/* 计算指定MD5所在的表的下标 */
	inline static decltype(CONCURRENCY) mapIndexOf(const MD5 &md5)noexcept{
		return md5.c[INDEX_CORE]%CONCURRENCY;
	}
	static const GC gc;
public:
	static ICodeManagerCPU& getInstance(HASH_TABLE_SIZE_TYPE initCapacity=DEFAULT_FACETABLE_CAPACITY, bool isCopy=true, float loadFactor=0, size_t overBlockCapacity=0) {
		init(initCapacity, isCopy, loadFactor, overBlockCapacity);
		return *m_instance;
	}
	static void init(HASH_TABLE_SIZE_TYPE initCapacity=DEFAULT_FACETABLE_CAPACITY, bool isCopy=true, float loadFactor=0, size_t overBlockCapacity=0);
	static void _release();
	// 禁止复制构造函数
	ICodeManagerCPU(const ICodeManagerCPU&) = delete;
	virtual ~ICodeManagerCPU();
	virtual void release();
	virtual uint32_t size()const noexcept;
	/* 向表中加入一个特征码对象code */
	virtual const code_bean& addBean(const code_bean& bean);
	virtual const code_bean* getBean(const MD5 &md5)const noexcept;
	virtual size_t getBeansByImgMD5(const MD5 &imgMD5,out_fun out)const;
	virtual bool removeBean(const MD5 &md5);
	virtual HASH_TABLE_SIZE_TYPE removeBeansByImgMD5(const MD5 &imgMD5);
	virtual std::string statInfo();
	virtual size_t searchCode(const face_code &code, double threshold, size_t rows, code_bean*out, const MD5Set *md5set=nullptr)const;
	TopKCodeBean searchCode(const face_code &code, TopKCodeBean::cmp_type threshold, size_t rows, const MD5Set *imgMD5Filter=nullptr)const;
};



} /* namespace gdface */

#endif /* FEATURE_SE_ICODEMANAGERCPU_H_ */
