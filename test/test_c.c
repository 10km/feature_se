#include <stdlib.h>
//#include <stdio.h>
#include "feature_se/feature_se.h"
int main() {	
	fse_init(16777216L,0.75f,1024L);
	face_code code;
	code_bean out[20];
	fse_searchCode(&code, 0.75, 20, out, NULL, 0);
	code_bean added;
	fse_addFeature(&added);
	MD5 md5;
	fse_removeFeature(&md5);
	fse_release();
}