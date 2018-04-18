/*
 * config.h
 *
 *  Created on: 2015年12月6日
 *      Author: guyadong
 */

#ifndef FEATURE_SE_CONFIG_H_
#define FEATURE_SE_CONFIG_H_
#ifndef FSE_IS_A_DLL
#define FSE_IS_A_DLL  1   /* not set if you're statically linking */
#endif // !FSE_IS_A_DLL

#ifndef FSE_DLL_DECL
#if defined( _WIN32) && FSE_IS_A_DLL 
#ifdef FSE_EXPORTS
#define FSE_DLL_DECL  __declspec(dllexport)
#define FSE_API __declspec(dllexport)
#else
#define FSE_DLL_DECL  __declspec(dllimport)
#define FSE_API __declspec(dllimport)
#endif
#else
#define FSE_DLL_DECL
#define FSE_API
#endif
#endif

#endif /* FEATURE_SE_CONFIG_H_ */
