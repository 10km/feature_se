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
// 定义 CASSDK的特征码长度(byte)
#define CODE_BYTE_LENGTH 2560
#else
// 定义 FaceDK的特征码长度(flat)
#define CODE_FLOAT_NUM 210
#endif
#define MD5_SIZE 16
typedef union _MD5{
	uint8_t c[MD5_SIZE];
	uint32_t i[MD5_SIZE/sizeof(uint32_t)];
	uint64_t l[MD5_SIZE/sizeof(uint64_t)];
#ifdef __cplusplus
	bool operator==(const _MD5 src)const noexcept{
		return l[0]==src.l[0]&&l[1]==src.l[1];
	}
	bool operator!=(const _MD5 src)const noexcept{
		return l[0]!=src.l[0]||l[1]!=src.l[1];
	}
	bool is_null()const noexcept {
		return 0 == l[0] && 0 == l[1];
	}
#endif
}MD5;
typedef char MD5_STR[MD5_SIZE*2+1];
#ifdef CASSDK
// CASSDK 版特征码类型
typedef struct _ALIGN_16 _face_code{
	unsigned char element[CODE_BYTE_LENGTH];
}face_code;
#else
// FaceSDK 版特征码类型
typedef struct _ALIGN_16 _face_code{
	float element[CODE_FLOAT_NUM];
	double sum;
}face_code;
#endif
typedef struct _code_bean {
	MD5 id;
	MD5 imgMD5;
	face_code code;
	double similarity;
}code_bean;

#endif /* FEATURE_SE_FEATURE_DATA_TYPES_H_ */
