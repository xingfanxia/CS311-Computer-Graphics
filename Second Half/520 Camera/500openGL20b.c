


/* On macOS, compile with...
    clang 500openGL20b.c -lglfw -framework OpenGL
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

#include "500shader.c"

GLuint program;
GLint positionLoc, colorLoc;
/* Instead of using OpenGL's modelview and projection matrices, we're going to 
use our own transformation matrices, just as in our software graphics engine. 
So we need to record their locations. */
GLint viewingLoc, modelingLoc;

/* Returns 0 on success, non-zero on failure. */
int initializeShaderProgram(void) {
	/* The two matrices will be sent to the shaders as uniforms. */
	GLchar vertexCode[] = "\
		uniform mat4 viewing;\
		uniform mat4 modeling;\
		attribute vec3 position;\
		attribute vec3 color;\
		varying vec4 rgba;\
		void main() {\
			gl_Position = viewing * modeling * vec4(position, 1.0);\
			rgba = vec4(color, 1.0);\
		}";
	GLchar fragmentCode[] = "\
		varying vec4 rgba;\
		void main() {\
			gl_FragColor = rgba;\
		}";
	program = makeProgram(vertexCode, fragmentCode);
	if (program != 0) {
		glUseProgram(program);
		positionLoc = glGetAttribLocation(program, "position");
		colorLoc = glGetAttribLocation(program, "color");
		/* Record the uniform locations. */
		viewingLoc = glGetUniformLocation(program, "viewing");
		modelingLoc = glGetUniformLocation(program, "modeling");
	}
	return (program == 0);
}

#define BUFFER_OFFSET(bytes) ((GLubyte*) NULL + (bytes))

/* Back from the dead. */
#include "100vector.c"
#include "130matrix.c"

/* We want to pass matrices into OpenGL, but there are two obstacles. First, 
our matrix library uses double matrices, but OpenGL 2.x expects GLfloat 
matrices. Second, C matrices are implicitly stored one-row-after-another, while 
OpenGL expects matrices to be stored one-column-after-another. This function 
plows through both of those obstacles. */
void mat44OpenGL(double m[4][4], GLfloat openGL[4][4]) {
	for (int i = 0; i < 4; i += 1)
		for (int j = 0; j < 4; j += 1)
			openGL[i][j] = m[j][i];
}

void render(void) {
	/* Clear, and prepare to send our own transformations to the shaders. */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(program);
	/* Send our own modeling transformation M to the shaders. */
	GLfloat modeling[4][4];
	double trans[3] = {0.0, 0.0, 0.0}, axis[3] = {1.0, 1.0, 1.0};
	double rot[3][3], model[4][4];
	vecUnit(3, axis, axis);
	alpha += 0.01;
	mat33AngleAxisRotation(alpha, axis, rot);
	mat44Isometry(rot, trans, model);
	mat44OpenGL(model, modeling);
	glUniformMatrix4fv(modelingLoc, 1, GL_FALSE, (GLfloat *)modeling);
	/* Send our own viewing transformation P C^-1 to the shaders. */
	GLfloat viewing[4][4];
	double camInv[4][4], proj[4][4], projCamInv[4][4];
	trans[2] = 4.0;
	mat33AngleAxisRotation(0.0, axis, rot);
	mat44InverseIsometry(rot, trans, camInv);
	mat44Orthographic(-2.0, 2.0, -2.0, 2.0, -6.0, -2.0, proj);
	mat444Multiply(proj, camInv, projCamInv);
	mat44OpenGL(projCamInv, viewing);
	glUniformMatrix4fv(viewingLoc, 1, GL_FALSE, (GLfloat *)viewing);
	/* The rest of the process is just as in the preceding tutorial. */
	glEnableVertexAttribArray(positionLoc);
	glEnableVertexAttribArray(colorLoc);
	glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
	glVertexAttribPointer(positionLoc, 3, GL_DOUBLE, GL_FALSE, 
		attrDim * sizeof(GLdouble), BUFFER_OFFSET(0));
	glVertexAttribPointer(colorLoc, 3, GL_DOUBLE, GL_FALSE, 
		attrDim * sizeof(GLdouble), BUFFER_OFFSET(3 * sizeof(GLdouble)));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);
	glDrawElements(GL_TRIANGLES, triNum * 3, GL_UNSIGNED_INT, BUFFER_OFFSET(0));
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
    glEnable(GL_DEPTH_TEST);
    /* OpenGL's depth convention is the opposite of what we've used in our 
    software engine. There are at least three ways to fix this. First, we could 
    negate the third row (row 2) of our projection matrices. Second, we could 
    tell glDepthFunc to use GL_GEQUAL instead of GL_LEQUAL. Third, we could 
    flip the final depth mapping, as follows. */
    glDepthRange(1.0, 0.0);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    initializeMesh();
    if (initializeShaderProgram() != 0)
    	return 3;
    while (glfwWindowShouldClose(window) == 0) {
        render();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glDeleteProgram(program);
	glDeleteBuffers(2, buffers);
	glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}


