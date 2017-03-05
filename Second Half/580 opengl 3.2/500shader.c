


/* This file helps you compile and link OpenGL shader programs. The process is 
simpler than it looks here. In pseudocode, the core idea is 10 steps:
	glCreateShader(vertexShader);
	glShaderSource(vertexShader, vertexCode);
	glCompileShader(vertexShader);
	glCreateShader(fragmentShader);
	glShaderSource(fragmentShader, fragmentCode);
	glCompileShader(fragmentShader);
	glCreateProgram(program);
	glAttachShader(vertexShader);
	glAttachShader(fragmentShader);
	glLinkProgram(program);
The code below is much longer than 10 steps, only because it has lots of error 
checking. */

/* Compiles a shader from GLSL source code. type is either GL_VERTEX_SHADER or 
GL_FRAGMENT_SHADER. If an error occurs, then returns 0. Otherwise, returns a 
compiled shader, which the user must eventually deallocate with glDeleteShader. 
(But usually this function is called from makeProgram, which calls 
glDeleteShader on the user's behalf.) */
GLuint makeShader(GLenum type, const GLchar *shaderCode) {
	GLuint shader = glCreateShader(type);
	if (shader == 0) {
		fprintf(stderr, "makeShader: glCreateShader failed\n");
		return 0;
	}
	glShaderSource(shader, 1, &shaderCode, NULL);
	glCompileShader(shader);
	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE) {
		GLsizei length;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
		GLchar *infoLog = (GLchar *)malloc(length);
		glGetShaderInfoLog(shader, length, &length, infoLog);
		fprintf(stderr, "makeShader: glGetShaderInfoLog:\n%s\n", infoLog);
		free(infoLog);
		glDeleteShader(shader);
		return 0;
	}
	return shader;
}

/* Compiles and links a shader program from two pieces of GLSL source code. If 
an error occurs, then returns 0. Otherwise, returns a shader program, which the 
user should eventually deallocate using glDeleteProgram. */
GLuint makeProgram(GLchar *vertexCode, GLchar *fragmentCode) {
	GLuint vertexShader, fragmentShader, program;
	vertexShader = makeShader(GL_VERTEX_SHADER, vertexCode);
	if (vertexShader == 0)
		return 0;
	fragmentShader = makeShader(GL_FRAGMENT_SHADER, fragmentCode);
	if (fragmentShader == 0) {
		glDeleteShader(vertexShader);
		return 0;
	}
	program = glCreateProgram();
	if (program == 0) {
		fprintf(stderr, "makeProgram: glCreateProgram failed\n");
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		return 0;
	}
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    GLint status;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (status != GL_TRUE) {
		GLsizei length;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
		GLchar *infoLog = (GLchar *)malloc(length);
		glGetProgramInfoLog(program, length, &length, infoLog);
		fprintf(stderr, "makeProgram: glGetProgramInfoLog:\n%s\n", infoLog);
		free(infoLog);
    	glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		glDeleteProgram(program);
		return 0;
    }
    /* Success. The shaders are built into the program and don't need to be 
    remembered separately, so delete them. */
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return program;
}

/* Checks the validity of a shader program against the rest of the current 
OpenGL state. Call it optionally after makeProgram, setting up textures, etc. 
Returns 0 if okay, non-zero if error. */
int validateProgram(GLuint program) {
	GLint validation;
    glValidateProgram(program);
	glGetProgramiv(program, GL_VALIDATE_STATUS, &validation);
	if (validation != GL_TRUE) {
		GLsizei length;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
		GLchar *infoLog = (GLchar *)malloc(length);
		glGetProgramInfoLog(program, length, &length, infoLog);
		fprintf(stderr, "validateProgram: glGetProgramInfoLog:\n%s\n", infoLog);
		free(infoLog);
		return 1;
	}
	return 0;
}


