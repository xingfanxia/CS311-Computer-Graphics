


/* On macOS, compile with...
    clang 510mainMesh.c -lglfw -framework OpenGL
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdarg.h>
#include <GLFW/glfw3.h>

#include "500shader.c"
#include "510vector.c"
#include "510mesh.c"

GLdouble alpha = 0.0;
GLuint program;
GLint attrLocs[3];
meshGLMesh meshGL;

void handleError(int error, const char *description) {
	fprintf(stderr, "handleError: %d\n%s\n", error, description);
}

void handleResize(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

/* Returns 0 on success, non-zero on failure. */
int initializeMesh(void) {
	int error;
	meshMesh mesh;
	error = meshInitializeCapsule(&mesh, 0.5, 2.0, 16, 32);
	if (error != 0)
		return 1;
	meshGLInitialize(&meshGL, &mesh);
	meshDestroy(&mesh);
	return 0;
}

/* Returns 0 on success, non-zero on failure. */
int initializeShaderProgram(void) {
	GLchar vertexCode[] = "\
		attribute vec3 position;\
		attribute vec2 texCoords;\
		attribute vec3 normal;\
		varying vec4 rgba;\
		void main() {\
			gl_Position = gl_ModelViewProjectionMatrix * vec4(position, 1.0);\
			rgba = vec4(texCoords, 1.0, 1.0) + vec4(normal, 1.0);\
		}";
	GLchar fragmentCode[] = "\
		varying vec4 rgba;\
		void main() {\
			gl_FragColor = rgba;\
		}";
	program = makeProgram(vertexCode, fragmentCode);
	if (program != 0) {
		glUseProgram(program);
		attrLocs[0] = glGetAttribLocation(program, "position");
		attrLocs[1] = glGetAttribLocation(program, "texCoords");
		attrLocs[2] = glGetAttribLocation(program, "normal");
	}
	return (program == 0);
}

void render(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-2.0, 2.0, -2.0, 2.0, -2.0, 2.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	alpha += 0.1;
	glRotatef(alpha, 1.0, 1.0, 1.0);
	glUseProgram(program);
	/* Drawing the mesh is now simple. */
	GLuint attrDims[3] = {3, 2, 3};
	meshGLRender(&meshGL, 3, attrDims, attrLocs);
}

int main(void) {
    glfwSetErrorCallback(handleError);
    if (glfwInit() == 0)
        return 1;
    GLFWwindow *window;
    window = glfwCreateWindow(512, 512, "Abstracting the Mesh", NULL, NULL);
    if (window == NULL) {
        glfwTerminate();
        return 2;
    }
    glfwSetWindowSizeCallback(window, handleResize);
    glfwMakeContextCurrent(window);
    fprintf(stderr, "main: OpenGL %s, GLSL %s.\n", 
		glGetString(GL_VERSION), glGetString(GL_SHADING_LANGUAGE_VERSION));
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    /* This version of initializeMesh has a chance of failure. */
    if (initializeMesh() != 0)
    	return 3;
    if (initializeShaderProgram() != 0)
    	return 4;
    while (glfwWindowShouldClose(window) == 0) {
        render();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glDeleteProgram(program);
    /* Don't forget to destroy the OpenGL mesh. */
	meshGLDestroy(&meshGL);
	glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}


