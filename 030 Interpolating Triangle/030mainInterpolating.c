//Xingfan Xia, Test for 030triangle.c
/* Torun:
clang 030mainInterpolating.c 000pixel.o -lglfw -framework OpenGL; ./a.out

Note:
All vertices must be entered in a counter-clockwise order
*/
#include <stdio.h>
#include <math.h>
#include "000pixel.h"
#include "030matrix.c"
#include "030vector.c"
#include "030triangle.c"

int main(void) {
	//Code taken from 000linking.c Basically drawing the canvas here
	if (pixInitialize(512, 512, "Pixel Graphics") != 0)
		return 1;
	else {
		//Then paint the screen dark and draw triangles using triRender
		pixClearRGB(0.0, 0.0, 0.0);

		// double a[2] = {0, 0};
		// double b[2] = {100, 200};
		// double c[2] = {50, 300};
		// double bg[3] = {1.0, 1.0, 1.0};
		// double alpha[3] = {1.0,0.0,0.0};
		// double beta[3] = {0.0,1.0,0.0};
	 //    double gamma[3] = {0.0,0.0,1.0};
		// triRender(a,b,c,bg,alpha,beta,gamma);

		double a[2] = {300,300};
		double b[2] = {50,300};
		double c[2] = {200,0};
		double bg[3] = {1.0,1.0,1.0};
		double alpha[3] = {1.0,0.0,0.0};
		double beta[3] = {0.0,1.0,0.0};
	    double gamma[3] = {0.0,0.0,1.0};
		triRender(a,b,c,bg,alpha,beta,gamma);
		pixRun();		
		return 0;
	}	
}