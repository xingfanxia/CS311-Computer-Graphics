


/* This is a C header file. It declares the names, input parameters, and return 
values for all public functions in the 000pixel.o library. It does not show how 
those functions are implemented. The implementation details are hidden in 
000pixel.c, which I have already compiled to 000pixel.o. */

/* A pixel system program usually proceeds through these five steps:
	A. pixInitialize is invoked and terminates.
	B. Some pixel system functions, other than pixInitialize and pixRun, are 
		invoked. Often these register user interface callbacks and draw an 
		initial picture.
	C. pixRun is invoked.
	D. While pixRun runs, some pixel system functions, other than pixInitialize 
		and pixRun, are invoked, automatically by the user event callbacks.
	E. The user elects to quit the program, thus causing pixRun to terminate.
Sometimes a pixel system program makes multiple loops through these five steps. 
The important thing is that invocations of pixInitialize and pixRun come in 
non-overlapping pairs. The pixel system is designed to support only one active 
window at a time. */



/*** Miscellaneous ***/

/* Initializes the pixel system. This function must be called before any other 
pixel system functions. The width and height parameters describe the size of 
the window. They should be powers of 2. The name parameter is a string for the 
window's title. Returns an error code, which is 0 if no error occurred. */
int pixInitialize(int width, int height, char *name);

/* Runs the event loop. First, any pending user events are processed by their 
corresponding callbacks. Second, the time step callback is invoked. Third, if 
any drawing has occurred, then the screen is updated to reflect that drawing. 
When the user elects to quit, the resources supporting the window are 
deallocated, and this function terminates; pixInitialize must be called again, 
before any further use of the pixel system. */
void pixRun(void);

/* Returns the red channel of the pixel at coordinates (x, y). Coordinates are 
relative to the lower left corner of the window. */
double pixGetR(int x, int y);

/* Returns the green channel of the pixel at coordinates (x, y). Coordinates 
are relative to the lower left corner of the window. */
double pixGetG(int x, int y);

/* Returns the blue channel of the pixel at coordinates (x, y). Coordinates are 
relative to the lower left corner of the window. */
double pixGetB(int x, int y);

/* Sets the pixel at coordinates (x, y) to the given RGB color. Coordinates are 
relative to the lower left corner of the window. */
void pixSetRGB(int x, int y, double red, double green, double blue);

/* Sets all pixels to the given RGB color. */
void pixClearRGB(double red, double green, double blue);



/*** Callbacks ***/

/* Sets a callback function for keys' being pressed. Invoked using something 
like
	pixSetKeyDownHandler(myKeyDownHandler);
where myKeyDownHandler is defined something like 
	void myKeyDownHandler(int key, int shiftIsDown, int controlIsDown,
		int altOptionIsDown, int superCommandIsDown);
The key parameter is GLFW_KEY_A, GLFW_KEY_B, ..., or GLFW_KEY_UNKNOWN, which 
are defined in GLFW/glfw3.h. The other parameters are flags describing the 
state of the modifier keys when the key was pressed. */
void pixSetKeyDownHandler(void (*handler)(int, int, int, int, int));

/* Sets a callback function for keys' being released. For details, see 
pixSetKeyDownHandler. */
void pixSetKeyUpHandler(void (*handler)(int, int, int, int, int));

/* Sets a callback function for keys' being held down. For details, see 
pixSetKeyDownHandler. */
void pixSetKeyRepeatHandler(void (*handler)(int, int, int, int, int));

/* Sets a callback function for mouse buttons' being pressed. Invoked using 
something like
	pixSetMouseDownHandler(myMouseDownHandler);
where myMouseDownHandler is defined something like 
	void myMouseDownHandler(int button, int shiftIsDown, int controlIsDown,
		int altOptionIsDown, int superCommandIsDown);
The button parameter is GLFW_MOUSE_BUTTON_LEFT, GLFW_MOUSE_BUTTON_RIGHT, or 
something like that. Those values are defined in GLFW/glfw3.h. The other 
parameters are flags describing the state of the modifier keys when the key was 
released. */
void pixSetMouseDownHandler(void (*handler)(int, int, int, int, int));

/* Sets a callback function for mouse buttons' being released. For details, see 
pixSetMouseDownHandler. */
void pixSetMouseUpHandler(void (*handler)(int, int, int, int, int));

/* Sets a callback function for the mouse's being moved. Invoked using 
something like
	pixSetMouseMoveHandler(myMouseMoveHandler);
where myMouseMoveHandler is defined something like 
	void myMouseMoveHandler(double x, double y);
The x and y parameters are relative to the lower left corner of the window. */
void pixSetMouseMoveHandler(void (*handler)(double, double));

/* Sets a callback function for the mouse's being scrolled. Invoked using 
something like 
	pixSetMouseScrollHandler(myMouseScrollHandler);
where myMouseScrollHandler is defined something like 
	void myMouseScrollHandler(double xOffset, double yOffset);
A vertical scroll wheel will always report 0.0 for xOffset. A scrolling gesture 
on a trackpad might generate non-zero values for both xOffset and yOffset. */
void pixSetMouseScrollHandler(void (*handler)(double, double));

/* Sets a callback function that fires once per animation frame, after all of 
the user interface callbacks. Invoked using something like 
	pixSetTimeStepHandler(myTimeStepHandler);
where myTimeStepHandler is defined something like
	void myTimeStepHandler(double oldTime, double newTime);
oldTime was the time for the previous frame; newTime is the time for the 
current frame. Both times are in seconds since the epoch (something like 1970). 
*/
void pixSetTimeStepHandler(void (*handler)(double, double));


