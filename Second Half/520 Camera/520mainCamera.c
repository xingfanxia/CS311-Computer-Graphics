


/* On macOS, compile with...
    clang 520mainCamera.c -lglfw -framework OpenGL
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdarg.h>
#include <GLFW/glfw3.h>

#include "500shader.c"
#include "510vector.c"
#include "510mesh.c"
#include "520matrix.c"
#include "520camera.c"

GLdouble alpha = 0.0, beta = 0.0;
GLuint program;
GLint attrLocs[3];
GLint viewingLoc, modelingLoc;
meshGLMesh meshGL;
/* Make a camera. We'll use the high-level interface. */
camCamera cam;

void handleError(int error, const char *description) {
	fprintf(stderr, "handleError: %d\n%s\n", error, description);
}

/* When the window changes shape, we update not just the viewport but also the 
projection, so that our view of the world is not distorted. Because we're using 
fovy (not fovx), the vertical extent of our view is fixed, and the horizontal 
extent changes to compensate. */
void handleResize(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
    camSetWidthHeight(&cam, width, height);
}

/* action is GLFW_PRESS, GLFW_RELEASE, or GLFW_REPEAT. mods is a bit field 
expressing the state of the four modifier keys. key is GLFW_KEY_A, 
GLFW_KEY_ENTER, GLFW_KEY_RIGHT, etc. Ignore scancode. */
void handleKey(GLFWwindow *window, int key, int scancode, int action,
		int mods) {
	int shiftIsDown = mods & GLFW_MOD_SHIFT;
	int controlIsDown = mods & GLFW_MOD_CONTROL;
	int altOptionIsDown = mods & GLFW_MOD_ALT;
	int superCommandIsDown = mods & GLFW_MOD_SUPER;
	if (action == GLFW_PRESS && key == GLFW_KEY_L) {
		camSwitchProjectionType(&cam);
	} else if (action == GLFW_PRESS || action == GLFW_REPEAT) {
		if (key == GLFW_KEY_O)
			camAddTheta(&cam, -0.1);
		else if (key == GLFW_KEY_P)
			camAddTheta(&cam, 0.1);
		else if (key == GLFW_KEY_I)
			camAddPhi(&cam, -0.1);
		else if (key == GLFW_KEY_K)
			camAddPhi(&cam, 0.1);
		else if (key == GLFW_KEY_U)
			camAddDistance(&cam, -0.1);
		else if (key == GLFW_KEY_J)
			camAddDistance(&cam, 0.1);
	}
}

/* Just as in 510mainMesh.c. */
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

/* A combination of the code from 510mainMesh.c and 500openGL20b.c. Returns 0 
on success, non-zero on failure. */
int initializeShaderProgram(void) {
	GLchar vertexCode[] = "\
		uniform mat4 viewing;\
		uniform mat4 modeling;\
		attribute vec3 position;\
		attribute vec2 texCoords;\
		attribute vec3 normal;\
		varying vec4 rgba;\
		void main() {\
			gl_Position = viewing * modeling * vec4(position, 1.0);\
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
		viewingLoc = glGetUniformLocation(program, "viewing");
		modelingLoc = glGetUniformLocation(program, "modeling");
	}
	return (program == 0);
}

void render(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(program);
	/* As in 500openGL20b.c, send M to the shaders. */
	GLfloat modeling[4][4];
	double trans[3] = {0.0, 0.0, 0.0}, axis[3] = {1.0, 1.0, 1.0};
	double rot[3][3], model[4][4];
	vecUnit(3, axis, axis);
	alpha += 0.01;
	mat33AngleAxisRotation(alpha, axis, rot);
	mat44Isometry(rot, trans, model);
	mat44OpenGL(model, modeling);
	glUniformMatrix4fv(modelingLoc, 1, GL_FALSE, (GLfloat *)modeling);
	/* Unlike in 500openGL20b.c, send P C^-1 to the shaders like this. */
	camRender(&cam, viewingLoc);
	/* As in 510mainMesh.c, render the mesh. */
	GLuint attrDims[3] = {3, 2, 3};
	meshGLRender(&meshGL, 3, attrDims, attrLocs);
}

int main(void) {
    glfwSetErrorCallback(handleError);
    if (glfwInit() == 0)
        return 1;
    GLFWwindow *window;
    window = glfwCreateWindow(768, 512, "Abstracting the Camera", NULL, NULL);
    if (window == NULL) {
        glfwTerminate();
        return 2;
    }
    glfwSetWindowSizeCallback(window, handleResize);
    glfwSetKeyCallback(window, handleKey);
    glfwMakeContextCurrent(window);
    fprintf(stderr, "main: OpenGL %s, GLSL %s.\n", 
		glGetString(GL_VERSION), glGetString(GL_SHADING_LANGUAGE_VERSION));
    glEnable(GL_DEPTH_TEST);
    glDepthRange(1.0, 0.0);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    if (initializeMesh() != 0)
    	return 3;
    if (initializeShaderProgram() != 0)
    	return 4;
    /* Set up the camera. */
	GLdouble target[3] = {0.0, 0.0, 0.0};
	camSetControls(&cam, camPERSPECTIVE, M_PI / 6.0, 10.0, 768.0, 512.0, 10.0, 
		M_PI / 4.0, M_PI / 4.0, target);
    while (glfwWindowShouldClose(window) == 0) {
        render();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glDeleteProgram(program);
	meshGLDestroy(&meshGL);
	glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}


