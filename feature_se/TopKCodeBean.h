/*
 * TopKCodeBean.h
 *
 *  Created on: 2015年10月23日
 *      Author: guyadong
 */

#ifndef FEATURE_SE_TOPKCODEBEAN_H_
#define FEATURE_SE_TOPKCODEBEAN_H_
#include <memory>
#include <type_traits>
#include <vector>
#include <cassert>
#include "codemgr.h"
#include "topk_base.h"
#include "gf_utility.h"
namespace gdface {
class TopKCodeBean: public topk_base<code_bean, decltype(code_bean::similarity)> {
public:
	// 比较值类型 与code_bean::similarity类型保持同步
	using cmp_type = decltype(code_bean::similarity);
	// 基类型
	using base_type=topk_base<code_bean,cmp_type>;
	using base_type::base_type; // 继承基类构造函数
	/* 返回包含排序结果到out数组,并将相似度值填到 code_bean.similarity
	 * 参见父类result()说明
	 */
	auto result_to(code_bean* out) noexcept -> decltype(size());
	/* 返回包含排序结果的vector
	 * 参见父类result_to()说明
	 */
	auto result_vector() noexcept -> decltype(make_unique<vector<code_bean> >(0));

};


} /* namespace gdface */

#endif /* FEATURE_SE_TOPKCODEBEAN_H_ */
