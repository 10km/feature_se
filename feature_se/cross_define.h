/*
 * cross_platform_def.h
 *
 *  Created on: 2015年11月6日
 *      Author: guyadong
 */

#ifndef FEATURE_SE_CROSS_DEFINE_H_
#define FEATURE_SE_CROSS_DEFINE_H_

#ifdef _MSC_VER
#define _ALIGN_(n) __declspec( align(n) )
#elif defined(__GNUC__)
#define _ALIGN_(n) __attribute__((aligned(n)))
#elif __cplusplus>=201103L
#define _ALIGN_(n) alignas(n)
#else
// 不支持的编译器类型
#error unexpected c complier (msc/gcc)
#endif /*_MSC_VER*/
#define _ALIGN_16 _ALIGN_(16)

#if __cplusplus<201103L
#ifdef _MSC_VER
#define thread_local __declspec( thread )
#elif defined(__GNUC__)
#define thread_local  __thread
#else
// 不支持的编译器类型
#error unexpected c complier (msc/gcc)
#endif
#endif /*__cplusplus>=201103L*/

#endif /* FEATURE_SE_CROSS_DEFINE_H_ */
