/*
 * feature_compare.h
 *
 *  Created on: 2015年12月16日
 *      Author: guyadong
 */

#ifndef FEATURE_SE_FEATURE_COMPARE_H_
#define FEATURE_SE_FEATURE_COMPARE_H_

#include <math.h>
#include "dot_product.h"
#include "codemgr.h"
#ifndef CODE_FLOAT_NUM
// 特征码默认长度，调用层应该在外部根据实际长度定义该宏
#define CODE_FLOAT_NUM 210
#endif

namespace gdface{
namespace feature{
/* V2版本特征码比对 */
template<typename E,dot_product_type TYPE=DOT_TYPE_CONST>
inline double compareV2(const E *f1, const E *f2, double sssum1, double sssum2) {
	return  dot_product<CODE_FLOAT_NUM, TYPE>(f1, f2) / sqrt(sssum1 * sssum2);
}
/* V1版本特征码比对 */
template<typename E, dot_product_type TYPE=DOT_TYPE_CONST>
inline double compareV1(const E *f1, const E *f2) {
	return compareV2(f1,f2,dot_product<CODE_FLOAT_NUM,TYPE>(f1,f1),dot_product<CODE_FLOAT_NUM,TYPE>(f2,f2));
}
/* V2版本特征码比对 */
template<dot_product_type TYPE=DOT_TYPE_CONST>
inline double compare(const face_code &f1,const face_code&f2){
	return compareV2<double,TYPE>(f1.element,f2.element,f1.sum,f2.sum);
}
} /* namespace feature*/
} /* namespace gdface*/

#endif /* FEATURE_SE_FEATURE_COMPARE_H_ */
