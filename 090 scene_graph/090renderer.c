//Xingfan Xia, January 20th
#include <stdio.h>

typedef struct renRenderer renRenderer;

struct renRenderer{
	int unifDim;
	int texNum;
	int attrDim;
	int varyDim;
	void (*colorPixel)(renRenderer*, double[], texTexture*[], 
        double[], double[]);
	void (*transformVertex)(renRenderer*, double[], double[], 
        double[]);
	void (*updateUniform)(renRenderer*, double[], double[]);
};