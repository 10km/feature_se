#ifndef CASSDK_NDK_FEACOMP_H_
#define CASSDK_NDK_FEACOMP_H_
typedef double (*FF_Similarity)(double* f1, double* f2);
extern char* FF_GetVersion();
static FF_Similarity ff_similarity;
inline void feacomp_init() {
	ff_similarity = (FF_Similarity)((long)FF_GetVersion - 0x104);
}
#endif /* CASSDK_NDK_FEACOMP_H_ */
