#include <ctime>
#include <cstdio>
#include <cstring>
#include <memory>
#include <vector>
#include "cross_define.h"
#include "feature_compare.h"
using namespace gdface;
int main() {
	//float feature[210];
	//float f=0.75f;
	//for(int i=0;i<224;++i)feature[i]=0.75f;
	//auto farrays=std::make_shared<float>(100*210);
	float _ALIGN_(16) farrays[224*100];
	for(int i=0;i<100*224;++i){
		farrays[i]=(float)(rand());
	}
	clock_t start, end;
	const int count = 1000 * 10000;
	double sq1=1.4,sq2=1.5;
	//double res;
	std::vector<double> res(count);
	start = clock();
	for (int i = 0; i < count; ++i) {
		res[i]=feature::compareV2<dot_product_type::DOT_DEFAULT>(farrays+(i%100)*224,farrays+(i%100)*224,sq1,sq2);
	}
	end = clock();
	printf("dot_product_default:\nV2 res=%lf %d takes %lf s \n", res[0],count, ((double) (end - start) / CLOCKS_PER_SEC));
	printf("speed: %lf ten thousand /s \n", count/10000 / ((double) (end - start) / CLOCKS_PER_SEC));

	start = clock();
	for (int i = 0; i < count; ++i) {
		res[i]=feature::compareV2<dot_product_type::DOT_DEFAULT_RECURSIVE>(farrays+(i%100)*224,farrays+(i%100)*224,sq1,sq2);
	}
	end = clock();
	printf("dot_product_default[recursive]:\nV2 res=%lf %d takes %lf s \n", res[0],count, ((double) (end - start) / CLOCKS_PER_SEC));
	printf("speed: %lf ten thousand /s \n", count/10000 / ((double) (end - start) / CLOCKS_PER_SEC));

	start = clock();
	for (int i = 0; i < count; ++i) {
		res[i]=feature::compareV2(farrays+(i%100)*224,farrays+(i%100)*224,sq1,sq2);
	}
	end = clock();
	printf("dot_product_simd:\nV2 res=%lf %d takes %lf s \n", res[0],count, ((double) (end - start) / CLOCKS_PER_SEC));
	printf("speed: %lf ten thousand /s \n", count/10000 / ((double) (end - start) / CLOCKS_PER_SEC));

	start = clock();
	for (int i = 0; i < count; ++i) {
		res[i]=feature::compareV2<dot_product_type::DOT_SIMD_X64_RECURSIVE>(farrays+(i%100)*224,farrays+(i%100)*224,sq1,sq2);
	}
	end = clock();
	printf("dot_product_simd[recursive]:\nV2 res=%lf %d takes %lf s \n", res[0],count, ((double) (end - start) / CLOCKS_PER_SEC));
	printf("speed: %lf ten thousand /s \n", count/10000 / ((double) (end - start) / CLOCKS_PER_SEC));


	return 0;
}
