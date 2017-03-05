


/*** Shadow program ***/

/* Do not alter the members of this data type. */
typedef struct shadowProgram shadowProgram;
struct shadowProgram {
	GLuint program;
	GLint *attrLocs;
	GLint viewingLoc, modelingLoc;
};

/* Creates a shadow-mapping shader program. attrNum is the number of attributes 
in meshes that will be drawn using the shadow program. Assumes that the 0th 
attribute is 3D position. Provides uniforms for the modeling and viewing 
matrices. Assumes that no other attributes or uniforms affect the placement of 
geometry. One shadow program can be used with multiple shadow maps, as long as 
attrNum is correct. In particular, if all meshes in the application have the 
same attrNum, then the application needs only one shadow program. Returns 0 on 
success, non-zero on failure. On success, the user must call 
shadowProgramDestroy when finished with the shadow program. */
int shadowProgramInitialize(shadowProgram *prog, GLuint attrNum) {
	prog->attrLocs = (GLint *)malloc(attrNum * sizeof(GLint));
	if (prog->attrLocs == NULL) {
		fprintf(stderr, "shadowProgramInitialize: malloc failed.\n");
		return 1;
	}
	/* The vertex shader produces just geometry --- no texture coordinates, 
	lighting information, or any of that. */
	GLchar vertexCode[] = "\
		#version 140\n\
		uniform mat4 viewing;\
		uniform mat4 modeling;\
		in vec3 position;\
		void main(void) {\
			gl_Position = viewing * modeling * vec4(position, 1.0);\
		}";
	/* We must have a fragment shader, but we don't actually care what colors 
	it produces. */
	GLchar fragmentCode[] = "\
		#version 140\n\
		out vec4 fragColor;\
		void main(void) {\
			fragColor = vec4(1.0, 1.0, 1.0, 1.0);\
		}";
	prog->program = makeProgram(vertexCode, fragmentCode);
	if (prog->program == 0) {
		free(prog->attrLocs);
		return 2;
	}
	/* Set up the locations. Only one of the attribute locations is meaningful. 
	The others are harmless dummy locations. */
	glUseProgram(prog->program);
	prog->attrLocs[0] = glGetAttribLocation(prog->program, "position");
	for (int i = 1; i < attrNum; i += 1)
		prog->attrLocs[i] = -1;
	prog->viewingLoc = glGetUniformLocation(prog->program, "viewing");
	prog->modelingLoc = glGetUniformLocation(prog->program, "modeling");
	return 0;
}

/* Deallocates the resources backing the shadow program. */
void shadowProgramDestroy(shadowProgram *prog) {
	free(prog->attrLocs);
	glDeleteProgram(prog->program);
}



/*** Shadow map ***/

/* Feel free to read from this struct's members, but don't alter them. */
typedef struct shadowMap shadowMap;
struct shadowMap {
	GLuint width, height;
	GLuint texture, fbo;
	camCamera camera;
};

/* The application needs one shadow map per active light (unless the light 
doesn't cast shadows). The width and height must be powers of 2. Returns 0 on 
success, non-zero on failure. On success, the user must call shadowMapDestroy 
when finished with the shadow map. */
int shadowMapInitialize(shadowMap *map, GLuint width, GLuint height) {
	/* Create a texture. */
	map->width = width;
	map->height = height;
	glGenTextures(1, &(map->texture));
	glBindTexture(GL_TEXTURE_2D, map->texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, width, height, 0, 
		GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	/* Set the filtering, comparison, and wrapping modes. */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, 
		GL_COMPARE_REF_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	/* We are finished configuring the texture. */
	glBindTexture(GL_TEXTURE_2D, 0);
	/* Create a framebuffer object, attach the texture, and disable color. */
	glGenFramebuffers(1, &(map->fbo));
	glBindFramebuffer(GL_FRAMEBUFFER, map->fbo);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, map->texture, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE) {
    	fprintf(stderr, 
    		"shadowMapInitialize: glCheckFramebufferStatus: %d\n", status);
    	return 1;
	} 
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return 0;
}

/* Deallocates the resources backing the shadow map. */
void shadowMapDestroy(shadowMap *map) {
	glDeleteTextures(1, &(map->texture));
	glDeleteFramebuffers(1, &(map->fbo));
}



/*** Rendering **/

/* Prepares a shadow map for rendering into it. */
void shadowMapRender(shadowMap *map, shadowProgram *prog, lightLight *light, 
		GLdouble far, GLdouble near) {
	glViewport(0, 0, map->width, map->height);
	glBindFramebuffer(GL_FRAMEBUFFER, map->fbo);
	glClear(GL_DEPTH_BUFFER_BIT);
	glUseProgram(prog->program);
	/* Configure the shadow map's camera to match the light. Invert the focal 
	length and ratio parameters from far and near. Only the ratio of width and 
	height matters --- not width and height. */
	camSetRotation(&(map->camera), light->rotation);
	camSetTranslation(&(map->camera), light->translation);
	GLdouble focal = sqrt(near * far);
	GLdouble ratio = -far / focal;
	camSetFrustum(&(map->camera), camPERSPECTIVE, light->spotAngle, focal, 
		ratio, 1.0, 1.0);
	camRender(&(map->camera), prog->viewingLoc);
	/* Use OpenGL's polygon offset feature to push the depth values slightly 
	away from the light. This prevents a lit triangle from fighting with itself 
	to be lit. It might cause slight over-lighting in some places. */
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(2.0, 4.0);
	/* Another trick is to switch from culling backfaces to culling frontfaces. 
	That causes the z-fighting to happen on faces that aren't lit anyway. But 
	it assumes closed surfaces, and we don't seem to need it right now. */
}

void shadowMapUnrender() {
	glDisable(GL_POLYGON_OFFSET_FILL);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void shadowRender(shadowMap *map, GLint viewingLoc, GLenum textureUnit, 
		GLint textureUnitIndex, GLint textureLoc) {
	camRender(&(map->camera), viewingLoc);
	glActiveTexture(textureUnit);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, map->texture);
    glUniform1i(textureLoc, textureUnitIndex);
}

void shadowUnrender(GLenum textureUnit) {
	glActiveTexture(textureUnit);
    glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
}


