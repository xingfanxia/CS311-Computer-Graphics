


/* On macOS, compile with...
    clang 500openGL20.c -lglfw -framework OpenGL
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdarg.h>
#include <GLFW/glfw3.h>

void handleError(int error, const char *description) {
	fprintf(stderr, "handleError: %d\n%s\n", error, description);
}

void handleResize(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

/* We're going to use the same GPU-side mesh as in the preceding tutorial. */
#define triNum 8
#define vertNum 6
#define attrDim 6
GLdouble alpha = 0.0;
GLuint buffers[2];

void initializeMesh(void) {
	GLdouble attributes[vertNum * attrDim] = {
		1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 
		-1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 
		0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 
		0.0, -1.0, 0.0, 0.0, 1.0, 1.0, 
		0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 
		0.0, 0.0, -1.0, 1.0, 0.0, 1.0};
	GLuint triangles[triNum * 3] = {
		0, 2, 4, 
		2, 1, 4, 
		1, 3, 4, 
		3, 0, 4, 
		2, 0, 5, 
		1, 2, 5, 
		3, 1, 5, 
		0, 3, 5};
	glGenBuffers(2, buffers);
	glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
	glBufferData(GL_ARRAY_BUFFER, vertNum * attrDim * sizeof(GLdouble),
		(GLvoid *)attributes, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, triNum * 3 * sizeof(GLuint),
		(GLvoid *)triangles, GL_STATIC_DRAW);
}

/* This file contains general-purpose code for building 'shader programs'. In 
OpenGL 2.x, a shader program consists of a vertex shader (like our old 
transformVertex) and a fragment shader (like our old colorPixel), written in 
OpenGL Shading Language (GLSL), compiled, and linked together. Probably you 
will want to include this file in all of your OpenGL 2.x programs. */
#include "500shader.c"

/* Once we build our shader program, we will refer to it using this variable. */
GLuint program;
/* We will pass data into the shader program through certain 'locations' in the 
GPU memory. So we need to keep track of those locations. */
GLint positionLoc, colorLoc;

/* This helper function builds a particular shader program, that renders the 
mesh that we built above (ignoring normals and lighting). Don't worry about the 
details of the GLSL code right now. Returns 0 on success, non-zero on error. On 
success, remember to call glDeleteProgram when you're finished with the 
program. */
int initializeShaderProgram(void) {
	/* A C trick: String literals placed next to each other are automatically 
	concatenated. And white space between expressions is ignored. So you can 
	build a string over multiple lines as follows. */
	GLchar vertexCode[] = 
		"attribute vec3 position;"
		"attribute vec3 color;"
		"varying vec4 rgba;"
		"void main() {"
		"	gl_Position = gl_ModelViewProjectionMatrix * vec4(position, 1.0);"
		"	rgba = vec4(color, 1.0);"
		"}";
	/* Another way to build a C string over multiple lines is to terminate each 
	line with a backslash. Use whichever way you prefer. */
	GLchar fragmentCode[] = "\
		varying vec4 rgba;\
		void main() {\
			gl_FragColor = rgba;\
		}";
	/* Just for educational purposes, print those two strings. Notice that the 
	strings don't actually contain any newline characters. You could make these 
	printouts more readable by inserting newlines into the code above, but then 
	the code itself would be less readable. */
	printf("initializeShaderProgram: vertexCode:\n%s\n", vertexCode);
	printf("initializeShaderProgram: fragmentCode:\n%s\n", fragmentCode);
	/* Make the program using the big helper function in 500shader.c. Then 
	record the key memory locations. */
	program = makeProgram(vertexCode, fragmentCode);
	if (program != 0) {
		glUseProgram(program);
		positionLoc = glGetAttribLocation(program, "position");
		colorLoc = glGetAttribLocation(program, "color");
	}
	return (program == 0);
}

#define BUFFER_OFFSET(bytes) ((GLubyte*) NULL + (bytes))

void render(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-2.0, 2.0, -2.0, 2.0, -2.0, 2.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	alpha += 0.1;
	glRotatef(alpha, 1.0, 1.0, 1.0);
	/* Make sure the intended shader program is active. (In a serious 
	application, we might switch among several shaders rapidly.) Connect our 
	attribute array to the attributes in the vertex shader. */
	glUseProgram(program);
	glEnableVertexAttribArray(positionLoc);
	glEnableVertexAttribArray(colorLoc);
	glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
	glVertexAttribPointer(positionLoc, 3, GL_DOUBLE, GL_FALSE, 
		attrDim * sizeof(GLdouble), BUFFER_OFFSET(0));
	glVertexAttribPointer(colorLoc, 3, GL_DOUBLE, GL_FALSE, 
		attrDim * sizeof(GLdouble), BUFFER_OFFSET(3 * sizeof(GLdouble)));
	/* Draw the triangles, exactly as in the preceding tutorial. */
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);
	glDrawElements(GL_TRIANGLES, triNum * 3, GL_UNSIGNED_INT, BUFFER_OFFSET(0));
	/* Disable the attributes when finished with them. */
	glDisableVertexAttribArray(positionLoc);
	glDisableVertexAttribArray(colorLoc);
}

int main(void) {
    glfwSetErrorCallback(handleError);
    if (glfwInit() == 0)
        return 1;
    GLFWwindow *window;
    window = glfwCreateWindow(768, 512, "Learning OpenGL 2.0", NULL, NULL);
    if (window == NULL) {
        glfwTerminate();
        return 2;
    }
    glfwSetWindowSizeCallback(window, handleResize);
    glfwMakeContextCurrent(window);
    fprintf(stderr, "main: OpenGL %s, GLSL %s.\n", 
		glGetString(GL_VERSION), glGetString(GL_SHADING_LANGUAGE_VERSION));
	/* Notice that we're not enabling as many OpenGL features as in the 
	preceding tutorials. The shaders subsume many fixed-function features. */
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    /* Initialize not just the mesh but also the shader program. */
    initializeMesh();
    if (initializeShaderProgram() != 0)
    	return 3;
    while (glfwWindowShouldClose(window) == 0) {
        render();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    /* Don't forget to deallocate the shader program. */
    glDeleteProgram(program);
	glDeleteBuffers(2, buffers);
	glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}


