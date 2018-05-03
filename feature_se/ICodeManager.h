/*
 * ICodeManager.h
 *
 *  Created on: 2015年11月24日
 *      Author: guyadong
 */

#ifndef FEATURE_SE_ICODEMANAGER_H_
#define FEATURE_SE_ICODEMANAGER_H_
#include <string>
#include <functional>
#include "config.h"
#include "codemgr.h"

namespace gdface {
#define  DEFAULT_FACETABLE_CAPACITY (1024*1024)
class MD5SetImpl;
class FSE_DLL_DECL MD5Set{
public:
	MD5SetImpl * const m_set;
public:
	MD5Set(uint32_t initCapacity=0, bool isCopy=false, float loadFactor=0.0, size_t overBlockCapacity=0);
	MD5Set(MD5Set&&rv);
	bool has(const MD5&);
	const MD5& insert(const MD5&key);
	virtual ~MD5Set();
};
/* 特征码管理对象接口 */
class FSE_DLL_DECL ICodeManager {
public:
	using out_fun=std::function<void(const code_bean&)>;
	/* 用于确定特征码所在并行表位置的字节数下标 */
	static const uint8_t INDEX_CORE = 0;
	/* 用于确定特征码所在主机位置的字节数下标 */
	static const uint8_t INDEX_HOST = (INDEX_CORE+1);
	ICodeManager()=default;
	virtual ~ICodeManager()=default;
	/* 释放所有资源 */
	virtual void release()=0;
	/* 返回表中数据总数 */
	virtual uint32_t size()const noexcept=0;
	/* 向表中加入一个特征码对象code */
	virtual const code_bean& addBean(const code_bean& bean)=0;
	/* 向表中获取一个特征码对象code */
	virtual const code_bean* getBean(const MD5 &md5)const noexcept=0;
	/* 返回所有属于校验码为imgMD5的图片的特征码对象数组 */
	virtual size_t getBeansByImgMD5(const MD5 &imgMD5,out_fun out)const=0;
	/* 从表中删除一个特征码对象 */
	virtual bool removeBean(const MD5 &md5)=0;
	/* 从表中删除所有属于imgMD5的特征码对象 返回删除的记录数目 */
	virtual uint32_t removeBeansByImgMD5(const MD5 &imgMD5)=0;
	/* 返回哈希表统计信息 */
	virtual std::string statInfo()=0;
	/* 通过特征码比对查找表中与code相似度大于阀值threshold(>0&&<=1.0)的前rows个code_bean,
	 * md5set不为nullptr时,只对code_bean.imgMD5包含在md5set集合的对象进行特征值比对,否则全表查找
	 * 返回结果数目,out中返回的搜索结果数组
	 */
	virtual size_t searchCode(const face_code &code, double threshold, size_t rows, code_bean*out, const MD5Set *md5set=nullptr) const=0;
	inline bool removeBean(const code_bean &bean) {return removeBean(bean.id);}
	inline bool hasBean(const MD5 &md5) const noexcept{return nullptr != getBean(md5);}
	inline bool empty(){return 0==size();}
};

} /* namespace gdface */

#endif /* FEATURE_SE_ICODEMANAGER_H_ */
