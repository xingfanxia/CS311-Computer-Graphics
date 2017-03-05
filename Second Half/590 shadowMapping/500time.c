/* On macOS, compile with...
    clang 500time.c -lglfw -framework OpenGL
Because this program uses POSIX time functions, it will not compile on Windows. 
If you want to work on Windows, research how to control time there. But 
remember that your work will be graded on macOS.
*/

#include <stdio.h>
#include <GLFW/glfw3.h>
#include <math.h>
/* This is the POSIX (roughly speaking, Unix) time facility. */
#include <sys/time.h>

void handleError(int error, const char *description) {
	fprintf(stderr, "handleError: %d\n%s\n", error, description);
}

void handleResize(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

/* Returns the time, in seconds, since some specific moment in the distant 
past. Supposedly accurate down to microseconds, but I wouldn't count on it. */
double getTime(void) {
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return (double)tv.tv_sec + (double)tv.tv_usec * 0.000001;
}

void render(double oldTime, double newTime) {
	if (floor(newTime) - floor(oldTime) >= 1.0)
		fprintf(stderr, "render: %f frames/sec\n", 1.0 / (newTime - oldTime));
	double x = 0.1 * cos(newTime);
	double y = 0.1 * sin(newTime);
	glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_TRIANGLES);
	glColor3f(1.0, 1.0, 0.0);
	glVertex2f(x + 0.1, y + 0.1);
	glVertex2f(x + 0.9, y + 0.2);
	glVertex2f(x + 0.6, y + 0.9);
	glEnd();
}

int main(void) {
	/* Record the current time. On the first frame of animation, it will serve 
	as the 'old time'. */
	double oldTime;
	double newTime = getTime();
    glfwSetErrorCallback(handleError);
    if (glfwInit() == 0)
        return 1;
    GLFWwindow *window;
    window = glfwCreateWindow(768, 512, "Learning Time", NULL, NULL);
    if (window == NULL) {
        glfwTerminate();
        return 2;
    }
    glfwSetWindowSizeCallback(window, handleResize);
    glfwMakeContextCurrent(window);
    fprintf(stderr, "main: OpenGL %s, GLSL %s.\n", 
		glGetString(GL_VERSION), glGetString(GL_SHADING_LANGUAGE_VERSION));
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
    while (glfwWindowShouldClose(window) == 0) {
    	/* Update the two times. In 000pixel.o, I pass these times to whatever 
    	time step handler you've supplied. Mimic that if you want, in your own 
    	code. But here we'll just pass them to the render function, because 
    	we're rendering on every time step anyway. */
    	oldTime = newTime;
    	newTime = getTime();
        render(oldTime, newTime);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
	glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}


