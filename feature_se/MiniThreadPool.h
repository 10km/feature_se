/*
 * MiniThreadPool.h
 *
 *  Created on: 2015年11月24日
 *      Author: guyadong
 */

#ifndef FEATURE_SE_MINITHREADPOOL_H_
#define FEATURE_SE_MINITHREADPOOL_H_
#include "ThreadPool.h"
namespace gdface {
/* 线程池 */
class MiniThreadPool:public ThreadPool {
public:
	/* 线程池构造函数,线程数默认初始化为与CPU核数相同 */
	MiniThreadPool(size_t threads=0):ThreadPool(0==threads?std::thread::hardware_concurrency():threads) {
	}
	virtual ~MiniThreadPool()=default;
};

} /* namespace gdface */

#endif /* FEATURE_SE_MINITHREADPOOL_H_ */
