/*
 * data_types.h
 *
 *  Created on: 2018年4月14日
 *      Author: 10km
 */

#ifndef FEATURE_SE_FEATURE_DATA_TYPES_H_
#define FEATURE_SE_FEATURE_DATA_TYPES_H_
#include <stdint.h>
#include "cross_define.h"
#ifdef CASSDK
// CASSDK x86/64平台可见光算法
// 定义 CASSDK的特征码长度(byte)
#define CODE_BYTE_LENGTH 2560
#define CODE_END_WITH_SUM 0
#elif defined CUSTOM_FEACOMP
// 使用自定义算法特征比较函数,暂时不支持
#ifndef CODE_BYTE_LENGTH
#error undefine CODE_BYTE_LENGTH
#endif
#define CODE_END_WITH_SUM 0
#else
// 基于欧氏距离计算相似度的内置比较算法
// 定义特征码数据类型,如果未定义默认为double
#ifndef CODE_ELEM_TYPE
#define CODE_ELEM_TYPE double
#endif
#ifndef CODE_ELEM_LEN
// 定义 特征码总长度(浮点类型)
#define CODE_FLOAT_NUM 210
// 旧版本face sdk
#define OLD_FACE_SDK 1
#else
#define CODE_FLOAT_NUM CODE_ELEM_LEN
#endif
// 定义特征值末尾是否有点积和(double),默认没有
#ifndef CODE_END_WITH_SUM
#define CODE_END_WITH 0
#endif
// 特征码长度(byte)
#if CODE_END_WITH_SUM
#define CODE_BYTE_LENGTH CODE_FLOAT_NUM*sizeof(CODE_ELEM_TYPE) + sizeof(double)
#else
#define CODE_BYTE_LENGTH CODE_FLOAT_NUM*sizeof(CODE_ELEM_TYPE)
#endif
#endif
#define MD5_SIZE 16
typedef union _MD5{
	uint8_t c[MD5_SIZE];
	uint32_t i[MD5_SIZE/sizeof(uint32_t)];
	uint64_t l[MD5_SIZE/sizeof(uint64_t)];
#ifdef __cplusplus
	bool operator==(const _MD5 &src)const noexcept{
		return l[0] == src.l[0] && l[1] == src.l[1];
	}
	bool operator!=(const _MD5 &src)const noexcept{
		return l[0] != src.l[0] || l[1] != src.l[1];
	}
	bool is_null()const noexcept {
		return 0 == l[0] && 0 == l[1];
	}
#endif
}MD5;
typedef char MD5_STR[MD5_SIZE*2+1];
#if defined(CUSTOM_FEACOMP) || defined(CASSDK)
// CASSDK 或其他自定义算法特征码类型
typedef struct _ALIGN_16 _face_code{
	unsigned char element[CODE_BYTE_LENGTH];
}face_code;
#else
// 默认基于点积(欧氏距离)计算相似度的特征码类型
typedef struct _ALIGN_16 _face_code {
	CODE_ELEM_TYPE element[CODE_FLOAT_NUM];
	double sum;/** element的点积和 */
}face_code;
#endif
typedef struct _code_bean {
	MD5 id;
	MD5 imgMD5;
	face_code code;
	double similarity;
}code_bean;

#endif /* FEATURE_SE_FEATURE_DATA_TYPES_H_ */
