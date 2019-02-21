/*
 * dot_product.h
 *
 *  Created on: 2015年12月15日
 *      Author: guyadong
 */

#ifndef FEATURE_SE_DOT_PRODUCT_H_
#define FEATURE_SE_DOT_PRODUCT_H_

#include <math.h>
#include <type_traits>
#include <cassert>
#include <memory>
#include <stdint.h>
#if defined(__x86_64)|| defined(_M_X64) // x64 体系结构 使用SSE指令优化 gcc 和 MinGW 有定义 __x86_64, MinGW 和 msc 有定义 _M_X64
	#ifdef _MSC_VER
		#include <mmintrin.h>
	#elif defined(__GNUC__)
		#include <xmmintrin.h>
	#else
	// 不支持的编译器类型
	#error unexpected c complier (msc/gcc)
	#endif
#endif

namespace gdface{
/* 定义点积函数类型  */
enum dot_product_type{DOT_DEFAULT,DOT_DEFAULT_RECURSIVE,DOT_SIMD_X64,DOT_SIMD_X64_RECURSIVE};

#if defined(__x86_64)|| defined(_M_X64) // x64 体系结构 使用SSE指令优化 gcc 和 MinGW 有定义 __x86_64, MinGW 和 msc 有定义 _M_X64
	#ifdef _MSC_VER
		const dot_product_type DOT_TYPE_CONST=DOT_SIMD_X64_RECURSIVE;//DOT_SIMD_X64;
		inline double _sum__m128(const __m128 & m1){
			return m1.m128_f32[0]+m1.m128_f32[1]+m1.m128_f32[2]+m1.m128_f32[3];
		}
		#define _SIMD_X64_
	#elif defined(__GNUC__)
		const dot_product_type DOT_TYPE_CONST=DOT_SIMD_X64_RECURSIVE;//DOT_SIMD_X64;
		inline double _sum__m128(const __m128 & m1){
			return m1[0]+m1[1]+m1[2]+m1[3];
		}
		#define _SIMD_X64_
	#else
	// 不支持的编译器类型
	#error unexpected c complier (msc/gcc)
	#endif
#else
		const dot_product_type DOT_TYPE_CONST=DOT_DEFAULT;
#endif

#ifdef _SIMD_X64_

template<size_t TAIL>
inline typename std::enable_if<TAIL==0>::type tail(__m128& m1,const __m128& src1, const __m128& src2) {}

template<size_t TAIL>
inline typename std::enable_if<TAIL==1>::type tail(__m128& m1,const __m128& src1, const __m128& src2) {
	__m128 t1=_mm_load_ss((float*)std::addressof(src1));
	__m128 t2=_mm_load_ss((float*)std::addressof(src2));
	m1=_mm_add_ps(m1,_mm_mul_ps(t1,t2));
}
template<size_t TAIL>
inline typename std::enable_if<TAIL==2>::type tail(__m128& m1,const __m128&src1, const __m128& src2) {
	__m128 z=_mm_setzero_ps();
	//__m128 t1=_mm_shuffle_ps(*src1,z,_MM_SHUFFLE(0,0,1,0));
	//__m128 t2=_mm_shuffle_ps(*src2,z,_MM_SHUFFLE(0,0,1,0));
	//m1=_mm_add_ps(m1,_mm_mul_ps(t1,t2));
	m1=_mm_add_ps(m1,_mm_mul_ps(_mm_shuffle_ps(src1,z,_MM_SHUFFLE(0,0,1,0)),_mm_shuffle_ps(src2,z,_MM_SHUFFLE(0,0,1,0))));
}
template<size_t TAIL>
inline typename std::enable_if<TAIL==3>::type tail(__m128& m1,const __m128& src1, const __m128& src2) {
	__m128 t1=src1;
	__m128 t2=src2;
#ifdef _MSC_VER
	t1.m128_f32[3]=0;
	t2.m128_f32[3]=0;
#elif defined(__GNUC__)
	t1[3]=0;
	t2[3]=0;
#else
// 暂不支持的编译器类型
#error unexpected c complier (msc/gcc)
#endif
	m1=_mm_add_ps(m1,_mm_mul_ps(t1,t2));
}

/* 递归方式计算__m128点积 */
template<size_t N>
inline typename std::enable_if<N==1,__m128>::type _dot_product__m128(const __m128* src1,const __m128* src2) {
	return _mm_mul_ps(*src1,*src2);
}
template<size_t N>
inline typename std::enable_if<(N>1),__m128>::type _dot_product__m128(const __m128* src1,const __m128* src2) {
	return _mm_add_ps(_dot_product__m128<1>(src1,src2),_dot_product__m128<N-1>(src1+1,src2+1));
}

/* SIMD版点积函数(递归版) f1,f2地址必须16字节对齐 */
template<size_t N,dot_product_type TYPE=DOT_TYPE_CONST>
inline typename std::enable_if<TYPE==DOT_SIMD_X64_RECURSIVE,double>::type dot_product(const float* f1, const float* f2) {
	assert(nullptr!=f1&&nullptr!=f2);
	__m128* src1=(__m128*)f1;
	__m128* src2=(__m128*)f2;
	__m128 m1=_dot_product__m128<(N>>2)>(src1,src2);
	tail<N%4>(m1,src1[N>>2],src2[N>>2]);
	return _sum__m128(m1);
}
/* SIMD版点积函数(循环版) f1,f2地址必须16字节对齐 */
template<size_t N,dot_product_type TYPE=DOT_TYPE_CONST>
inline typename std::enable_if<TYPE==DOT_SIMD_X64,double>::type dot_product(const float* f1, const float* f2) {
	assert(nullptr!=f1&&nullptr!=f2);
	__m128* src1=(__m128*)f1;
	__m128* src2=(__m128*)f2;
	__m128 m1=_mm_setzero_ps();
	for (size_t i = 0; i < N>>2; ++i){
		m1=_mm_add_ps(m1,_mm_mul_ps(src1[i],src2[i]));
	}
	tail<N%4>(m1,src1[N>>2],src2[N>>2]);
	return _sum__m128(m1);
}
#endif /* _SIMD_X64_ */

/* 默认点积函数(循环版) */
template<size_t N,dot_product_type TYPE=DOT_TYPE_CONST>
inline typename std::enable_if<TYPE==DOT_DEFAULT,double>::type dot_product(const float* f1, const float* f2) {
	assert(nullptr!=f1&&nullptr!=f2);
	float sum = 0.0;
	for (size_t i = 0; i < N; ++i)
		sum += f1[i] * f2[i];
	return (double)sum;
}
/* 默认点积函数(递归版) */
template<size_t N,dot_product_type TYPE=DOT_TYPE_CONST>
inline typename std::enable_if<(N==1)&&(TYPE==DOT_DEFAULT_RECURSIVE),double>::type dot_product(const float* f1, const float* f2) {
	assert(nullptr!=f1&&nullptr!=f2);
	return (*f1) * (*f1);
}
template<size_t N,dot_product_type TYPE=DOT_TYPE_CONST>
inline typename std::enable_if<(N>1)&&(TYPE==DOT_DEFAULT_RECURSIVE),double>::type dot_product(const float* f1, const float* f2) {
	assert(nullptr!=f1&&nullptr!=f2);
	return dot_product<1,TYPE>(f1,f2)+dot_product<N-1,TYPE>(f1+1,f2+1);
}

} /* namespace gdface */

#endif /* FEATURE_SE_DOT_PRODUCT_H_ */
