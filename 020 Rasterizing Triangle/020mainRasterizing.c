//Xingfan Xia, Test for 020triangle.c
//
//Note:
//All vertices must be entered in a counter-clockwise order
#include "020triangle.c"
int main(void) {
	//Code taken from 000linking.c Basically drawing the canvas here
	if (pixInitialize(512, 512, "Pixel Graphics") != 0)
		return 1;
	else {
		//Then paint the screen dark and draw triangles using triRender
		pixClearRGB(0.0, 0.0, 0.0);
		triRender(100,100,300,100,200,200,0.1,0.3,0.4);
		// triRender(400,500,200,200,300,300,0.1,0.3,0.4);
		// triRender(350,300,200,400,300,300,0.1,0.3,0.4);
		// triRender(180, 200, 190, 210, 200, 200, 1, 1, 1);
		pixRun();		
		return 0;
	}	
}