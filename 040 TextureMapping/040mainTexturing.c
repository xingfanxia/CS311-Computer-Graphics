//Xingfan Xia, Test for 040texture.c
/* Torun:
clang 040mainTexturing.c 000pixel.o -lglfw -framework OpenGL; ./a.out

Note:
All vertices must be entered in a counter-clockwise order
*/

#include "000pixel.h"
#include "030matrix.c"
#include "040triangle.c"
#include "030vector.c"
#include <stdio.h>
#include <math.h>

int main(void) {
	//Code taken from 000linking.c Basically drawing the canvas here
	if (pixInitialize(512, 512, "Pixel Graphics") != 0)
		return 1;
	else {
		//Then paint the screen dark and draw triangles using triRender
		pixClearRGB(0.0, 0.0, 0.0);

		texTexture *tex;
		tex->filtering = 1;
		double a[2] = {300,300};
		double b[2] = {50,300};
		double c[2] = {200,0};
		double bg[3] = {1.0,1.0,1.0};
		double alpha[2] = {0.0,0.5};
		double beta[2] = {1.0,1.0};
	    double gamma[2] = {0.0,1.0};
	    texInitializeFile(tex, "sf.png");
		triRender(a,b,c,bg,tex,alpha,beta,gamma);
		texDestroy(tex);
		pixRun();
		return 0;
	}	
}