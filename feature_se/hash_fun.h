/*
 * hash_fun.h
 *
 *  Created on: 2015年12月14日
 *      Author: guyadong
 */

#ifndef FEATURE_SE_HASH_FUN_H_
#define FEATURE_SE_HASH_FUN_H_
#include <functional>
#include <type_traits>
namespace gdface{
template<size_t TAIL>
typename std::enable_if< (TAIL==0)>::type hash_code_tail(uint64_t &hash, const uint64_t* u,uint64_t seed) noexcept {
}
template<size_t TAIL>
typename std::enable_if< (TAIL>0)>::type hash_code_tail(uint64_t &hash, const uint64_t* u,uint64_t seed) noexcept {
	#if defined( _MSC_VER) || (defined(__GNUC__) && __BYTE_ORDER__ ==__ORDER_LITTLE_ENDIAN__)
	hash = hash * seed + ((*u) & ((1ULL << (TAIL << 3)) - 1)); // 小端模式
	#elif defined(__GNUC__) && __BYTE_ORDER__ ==__ORDER_BIG_ENDIAN__
	hash = hash * seed + ((*u) >> ((sizeof(uint64_t) - TAIL) << 3)); // 大端模式
	#else
	#error unexpected c complier (msc/gcc)
	#endif
}
/* 哈希函数实现 */
template<typename T>
size_t hash_code( const T& t) noexcept {
	uint64_t hash = 0;
	auto p = reinterpret_cast<uint64_t*>(const_cast<T*>(std::addressof(t)));
	auto  const u = p + (sizeof(T)>>3);//sizeof(uint64_t);
	uint64_t seed = 131; // 31 131 1313 13131 131313 etc..BKDRHash
	for (; p < u; p ++)	hash = hash * seed + *p;
	hash_code_tail<sizeof(T)%sizeof(uint64_t)>(hash,u,seed);
	return (size_t) hash;
}
/*template<typename T>
size_t hash_code( const T& t) noexcept {
	size_t hash = 0;
	auto p = reinterpret_cast<const int8_t*>(std::addressof(t));
	size_t seed = 131; // 31 131 1313 13131 131313 etc..BKDRHash
	for (auto  u = p +sizeof(T); p < u; ++p)	hash = hash * seed + *p;
	return (size_t) hash;
}*/
/* 返回获取hash值的一元函数,
 * 如果T有std::hash<T>特例实现返回std::hash<T>,否则提供缺省的hash实现
 */
template<typename T>
struct hash_fun{
	/* 缺省的hash实现 */
	struct default_hash {
		typedef T argument_type;
		typedef std::size_t result_type;
		result_type operator()(argument_type const& t) const noexcept {return hash_code<T>(t);	}
	};
	/* SFINAE 判断T有没有std::hash<T>特例实现 */
	template<typename U> static std::hash<U> test(decltype(std::declval<std::hash<U>>().operator()(std::declval<U>())));
	template<typename U> static default_hash test(...);
	using type =decltype(test<T>(0));
	type fun;
};
/* 判断有没有std::hash<T>实现 */
template <typename T>
struct has_hash_specific{
    template<typename U> static auto test(int)->	decltype(std::declval<std::hash<U>>().operator()(std::declval<U>()));
	template<typename U> static void test(...);
	enum{value=!std::is_void<decltype(test<T>(0))>::value};
	//通过判断test<T>(0)返回值是否为void来判断是否有hash<T>特例
};

} /* namespace gdface*/



#endif /* FEATURE_SE_HASH_FUN_H_ */
