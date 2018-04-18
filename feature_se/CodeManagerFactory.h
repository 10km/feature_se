/*
 * CodeManagerFactory.h
 *
 *  Created on: 2015年12月6日
 *      Author: guyadong
 */

#ifndef FEATURE_SE_CODEMANAGERFACTORY_H_
#define FEATURE_SE_CODEMANAGERFACTORY_H_
#include "ICodeManager.h"
namespace gdface {

class FSE_DLL_DECL CodeManagerFactory {
public:
	CodeManagerFactory();
	virtual ~CodeManagerFactory();
	static ICodeManager * getICodeManagerCPU(uint32_t initCapacity=DEFAULT_FACETABLE_CAPACITY, bool isCopy=true, float loadFactor=0, size_t overBlockCapacity=0);

};

} /* namespace gdface */

#endif /* FEATURE_SE_CODEMANAGERFACTORY_H_ */
