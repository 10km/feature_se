/*
 * feature_se.h
 * (人脸)特征搜索引擎(Feature Search Engine)C接口定义
 * 所有返回int类型的函数返回值<0,代表异常，这里可以通过fse_error_msg()获取错误信息,只在当前线程有效
 *  Created on: 2018年4月14日
 *      Author: guyadong
 */

#ifndef FEATURE_SE_FEATURE_SE_H_
#define FEATURE_SE_FEATURE_SE_H_
#include "config.h"
#include "feature_data_types.h"
#ifdef __cplusplus
extern "C" {
#endif

/* 返回当前线程的错误信息 */
FSE_API const char* fse_error_msg();
/* (人脸)特征搜索引擎初始化
 * initCapacity 哈希表初始容量
 * loadFactor 哈希表扩容因子
 * overBlockCapacity 过载表容量
 * 正常返回0, <0 出错
 */
FSE_API int fse_init(uint32_t initCapacity, float loadFactor, size_t overBlockCapacity);
/* 释放所有资源
 * 正常返回0, <0 出错
 */
FSE_API int fse_release();
/* 在内存表中根据比对相似度进行特征码搜索
 * code 要比对的特征码
 * threshold 相似度阀值
 * rows  最大返回的记录数
 * out 结果输出缓冲区,长度由rows定义
 * md5Arrays 比对的图像范围(图像集)
 * md5Count 图像范围大小
 */
FSE_API int fse_searchCode(const face_code *code, double threshold, size_t rows, code_bean*out, const MD5 *md5Arrays, size_t md5Count);
/* 根据特征码ID在表中查找指定的记录
 * md5 特征码ID,为null时返回0
 * out 结果输出,为null时出错
 * 返回 0没有找到,1找到,<0 出错
 */
FSE_API int fse_getFeature(const MD5 *md5, code_bean* out);
/* 添加一组特征码到内存表
* 正常返回添加的特征记录数,beans为null时返回0,< 0 出错
*/
FSE_API int fse_addFeatures(const code_bean *beans, size_t beanCount = 1);

/* 删除md5s指定的一组特征
* 删除成功返回删除的记录数,< 0出错
*/
FSE_API int fse_removeFeatures(const MD5 *md5s, size_t md5Count = 1);
/* 根据图像imgMD5数组删除所有指定的记录
 * imgMD5s 特征码所在图像的MD5校验码数组
 * 返回返回删除的记录数,< 0 出错
 */
FSE_API int fse_removeFeaturesByImgMD5(const MD5 *imgMD5s, size_t md5Count = 1);
/* 返回内存表中元素个数 */
FSE_API int fse_size();
/* 返回哈希表统计信息字符串指针(当前线程有效,用于debug测试),出错返回nullptr */
FSE_API const char* fse_statInfo();
#ifdef __cplusplus
}
#endif
#endif /* FEATURE_SE_FEATURE_SE_H_ */
