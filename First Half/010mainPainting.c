


/* In this tutorial our compiler invocation becomes more complicated. On macOS, 
we compile with...
    clang 000linking.c 000pixel.o -lglfw -framework OpenGL
This invocation causes our compiled code to be 'linked' with three other pieces 
of software, that have been compiled elsewhere: OpenGL, GLFW, and pixel.o. 
OpenGL is a cross-platform, standardized library for interacting with graphics 
hardware. It comes pre-installed on macOS and Linux, and is easily installed on 
Windows. GLFW is a cross-platform, minimalist user interface toolkit, that we 
can use to build simple OpenGL applications. Often it is not pre-installed, but 
it is easy to install. pixel.o is a library that I have written, to provide a 
simple foundation for this graphics course. You must have pixel.o and pixel.h 
in the same directory as linking.c. */

/* Notice that we include pixel.h, enclosed in quotation marks rather than 
angle brackets. Feel free to read pixel.h. It documents all of the public 
functionality of the library pixel.o. */
#include <stdio.h>
#include <math.h>
#include "000pixel.h"
#include <GLFW/glfw3.h>
double red;
double green;
double blue;
double mouseClick;


/* This function is a user interface 'callback'. Once we register it with the 
user interface infrastructure, it is called whenever the user releases a 
keyboard key. For details, see the pixSetKeyUpHandler documentation. */
void handleKeyUp(int key, int shiftIsDown, int controlIsDown,
		int altOptionIsDown, int superCommandIsDown) {
	printf("key up %d, shift %d, control %d, altOpt %d, supComm %d\n",
		key, shiftIsDown, controlIsDown, altOptionIsDown, superCommandIsDown);
    /* white */
    if (key == 87){
        red = 1.0;
        green = 1.0;
        blue = 1.0;
    /* cyan*/
    } else if (key == 67){
        red = 0;
        green = 1;
        blue = 1;
    /* blue*/
    } else if (key == 66 ){
        red = 0;
        green = 0;
        blue = 1;
    /* red*/
    } else if (key == 82){
        red = 1;
        green = 0;
        blue = 0;
    /* green*/
    } else if (key == 71){
        red = 0;
        green = 1;
        blue = 0;
    /* magenta*/
    } else if (key == 77){
        red =1;
        green = 0;
        blue = 1;
    /* yellow*/
    } else if (key == 89){
        red = 1;
        green = 1;
        blue = 0;
    }
    
}

/* Similarly, the following callbacks handle some mouse interactions. */
void handleMouseUp(int button, int shiftIsDown, int controlIsDown,
		int altOptionIsDown, int superCommandIsDown) {
	printf("mouse up %d, shift %d, control %d, altOpt %d, supComm %d\n",
		button, shiftIsDown, controlIsDown, altOptionIsDown, 
		superCommandIsDown);
    if (button == 0){
        mouseClick = 1;
    }
}

void handleMouseMove(double x, double y) {
	printf("mouse move x %f y %f\n", x, y);
    if (mouseClick == 0){
        pixSetRGB(x,y,red,green,blue);
    }
   
    

}

void handleMouseScroll(double xOffset, double yOffset) {
	printf("mouse scroll xOffset %f yOffset %f\n", xOffset, yOffset);
}

void handleMouseDown(int button, int shiftIsDown, int controlIsDown,
    int altOptionIsDown, int superCommandIsDown) {
    printf("mouse down %d, shift %d, control %d, altOpt %d, supComm %d\n",
           button, shiftIsDown, controlIsDown, altOptionIsDown,superCommandIsDown);
    if (button == 0){
        mouseClick = 0;
    } else {
        mouseClick = 1;
    }
    
    
}
/* This callback is called once per animation frame. As parameters it receives 
the time for the current frame and the time for the previous frame. Both times 
are measured in seconds since some distant past time. */ 
void handleTimeStep(double oldTime, double newTime) {
	if (floor(newTime) - floor(oldTime) >= 0.1)
		printf("handleTimeStep: %f frames/sec\n", 1.0 / (newTime - oldTime));
}

/* You can also set callbacks for key down, key repeat, and mouse down. See 
000pixel.h for details. */

int main(void) {
	/* Make a 512 x 512 window with the title 'Pixel Graphics'. This function 
	returns 0 if no error occurred. */
	if (pixInitialize(512, 512, "Pixel Graphics") != 0)
		return 1;
	else {
		/* Register the callbacks (defined above) with the user interface, so 
		that they are called as needed during pixRun (invoked below). */
		pixSetKeyUpHandler(handleKeyUp);
        pixSetMouseDownHandler(handleMouseDown);
        pixSetMouseMoveHandler(handleMouseMove);
		pixSetMouseUpHandler(handleMouseUp);
		pixSetMouseScrollHandler(handleMouseScroll);
		pixSetTimeStepHandler(handleTimeStep);
        pixSetMouseDownHandler(handleMouseDown);
		/* Clear the window to black. */
		pixClearRGB(0.0, 0.0, 0.0);
		
		/* Run the event loop. The callbacks that were registered above are 
		invoked as needed. At the end, the resources supporting the window are 
		deallocated. */
		pixRun();
		return 0;
	}
}


