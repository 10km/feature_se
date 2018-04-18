/*
 * feature_compare_cas.h
 *
 *  Created on: 2016年7月12日
 *      Author: guyadong
 */

#ifndef FEATURE_SE_FEATURE_COMPARE_CAS_H_
#define FEATURE_SE_FEATURE_COMPARE_CAS_H_
#if __linux
#include "FiStdDefEx.h"
#elif _WIN32
#include <windows.h>
#endif
#include "THFeature_i.h"
#include "codemgr.h"
namespace gdface{
namespace feature{
/* CASSDK特征码比对 */
inline float compare(const face_code &f1,const face_code&f2){
	// 本函数被调用前必须确保已经执行 EF_Init()
	return EF_Compare((BYTE*)f1.element,(BYTE*)f2.element);
}
} /* namespace feature*/
} /* namespace gdface*/

#endif /* FEATURE_SE_FEATURE_COMPARE_CAS_H_ */
