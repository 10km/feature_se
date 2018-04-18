/*
 * codemgr.h
 *
 *  Created on: 2015年10月1日
 *      Author: guyadong
 */

#ifndef FEATURE_SE_CODEMGR_H_
#define FEATURE_SE_CODEMGR_H_
#include <cstdlib>
#include <utility>
#include <functional>
#include "common_utilits.h"
#include "feature_data_types.h"
#include "intrinsic_wrapper.h"
namespace gdface{
typedef signed char FACE_CODE[sizeof(face_code)];

inline string MD5toMD5_STR(MD5 const &md5char) {
	return com_utilits::bytes_to_hex_string(&md5char,sizeof(MD5));
}

inline void PSTRtoMD5(const char * pStr,MD5 &md5) {
	com_utilits::hex_to_bytes(pStr,&md5,sizeof(MD5));
}
#ifdef CASSDK
#define FACE_CODE_CONVERT(code) code
#elif  ((_M_X64||_M_AMD64||_M_IX86) && _MSC_VER) || (__GNUC__&& (__BYTE_ORDER__ ==__ORDER_LITTLE_ENDIAN__))
#define FACE_CODE_CONVERT(code) code
#elif __GNUC__ && __BYTE_ORDER__ ==__ORDER_BIG_ENDIAN__
inline face_code _code_reverse(const face_code& code) {
	//当系统为大端时把字节序转换为小端
	//gcc下用内置函数转换
	face_code new_code;
	for(int i=0;i<CODE_FLOAT_NUM;++i)
		new_code.element[i]=(float)__builtin_bswap32((uint32_t)code.element[i]);
	new_code.sum=(double)__builtin_bswap64((uint64_t)code.sum);
	return new_code;
}
#define FACE_CODE_CONVERT(code) _code_reverse(code)
#else
//不支持的编译器类型
#error unexpected c complier (msc/gcc)
//以下代码保留
template<typename T>
inline typename std::enable_if<2==sizeof(T),T>::type _reverse_bytes(const T &src) {
	T dst=src;
	char*ptr=reinterpret_cast<char*>(std::addressof(dst));
	std::swap(*ptr,*(ptr+1));
	return dst;
}
template<typename T>
inline typename std::enable_if<4==sizeof(T),T>::type _reverse_bytes(const T &src) {
	T dst=src;
	char*ptr=reinterpret_cast<char*>(std::addressof(dst));
	std::swap(*ptr,*(ptr+3));
	std::swap(*(ptr+1),*(ptr+2));
	return dst;
}
template<typename T>
inline typename std::enable_if<8==sizeof(T),T>::type _reverse_bytes(const T &src) {
	T dst=src;
	char*ptr=reinterpret_cast<char*>(std::addressof(dst));
	std::swap(*ptr,*(ptr+7));
	std::swap(*(ptr+1),*(ptr+6));
	std::swap(*(ptr+2),*(ptr+5));
	std::swap(*(ptr+3),*(ptr+4));
	return dst;
}

inline face_code  _code_reverse(const face_code& code) {
	//当系统为大端时把字节序转换为小端
	face_code new_code;
	for(int i=0;i<CODE_FLOAT_NUM;++i) {
		new_code.element[i]=_reverse_bytes(code.element[i]);
	}
	new_code.sum=_reverse_bytes(code.sum);
	return new_code;
}
#define FACE_CODE_CONVERT(code) _code_reverse(&code)
#endif
} /* namespace gdface */

/* 为MD5提供std::hash<MD5>特例化实现 */
namespace std
{
    template<>
    struct hash<MD5> {
        typedef MD5 argument_type;
        typedef std::size_t result_type;
        result_type operator()(argument_type const& md5) const noexcept {
            return (result_type)(md5.l[0]^md5.l[1]);
        }
    };
} /* namespace std */

#endif /* FEATURE_SE_CODEMGR_H_ */
