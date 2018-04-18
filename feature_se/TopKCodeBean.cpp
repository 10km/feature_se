/*
 * TopKCodeBean.cpp
 *
 *  Created on: 2015年10月23日
 *      Author: guyadong
 */

#include "TopKCodeBean.h"

namespace gdface {
auto TopKCodeBean::result_to(code_bean* out) noexcept -> decltype(declval<TopKCodeBean>().size())
{
	assert(nullptr!=out);
	auto& sort = result();
	for (auto i = size(); i > 0; --i) {
		out[i-1] = *sort[i - 1].t;
		out[i-1].similarity = sort[i - 1].n;
	}
	return size();
}

auto TopKCodeBean::result_vector() noexcept -> decltype(make_unique<vector<code_bean> >(0))
{
	auto v = make_unique<vector<code_bean> >(size());
	result_to(v.get()->data());
	return v;
}

} /* namespace gdface */
