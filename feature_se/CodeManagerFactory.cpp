/*
 * CodeManagerFactory.cpp
 *
 *  Created on: 2015年12月6日
 *      Author: guyadong
 */

#include "CodeManagerFactory.h"
#include "ICodeManagerCPU.h"
namespace gdface {

CodeManagerFactory::CodeManagerFactory()=default;

CodeManagerFactory::~CodeManagerFactory() =default;

ICodeManager * CodeManagerFactory::getICodeManagerCPU(HASH_TABLE_SIZE_TYPE initCapacity, bool isCopy, float loadFactor, size_t overBlockCapacity){
	return &ICodeManagerCPU::getInstance(initCapacity,isCopy,loadFactor,overBlockCapacity);
}
} /* namespace gdface */
