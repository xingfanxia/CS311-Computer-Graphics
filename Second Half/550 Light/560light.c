


#define lightOMNI 0
#define lightSPOT 1
#define lightDIRECTIONAL 2

/* Feel free to read from this struct's members, but don't write to them, 
except through accessor functions. */
typedef struct lightLight lightLight;
struct lightLight {
	GLdouble translation[3];
	GLdouble color[3];
	GLdouble attenuation[3];
	GLuint lightType;
	GLdouble rotation[3][3];
	GLdouble spotAngle;
};

/* Sets the light's rotation. */
void lightSetRotation(lightLight *light, GLdouble rot[3][3]) {
	vecCopy(9, (GLdouble *)rot, (GLdouble *)(light->rotation));
}

/* Sets the light's translation. */
void lightSetTranslation(lightLight *light, GLdouble transl[3]) {
	vecCopy(3, transl, light->translation);
}

/* Sets the light type, to one of lightDIRECTIONAL, etc. */
void lightSetType(lightLight *light, GLuint lightType) {
	light->lightType = lightType;
}

/* Sets the light's RGB color. */
void lightSetColor(lightLight *light, GLdouble rgb[3]) {
	vecCopy(3, rgb, light->color);
}

/* Sets the light's attenuation coefficients. The light intensity at distance d 
from the light is 1 / (a0 + a1 d + a2 d^2) times whatever it would be 
unattenuated. So, to deactivate attenuation, use values 1.0, 0.0, 0.0. */
void lightSetAttenuation(lightLight *light, GLdouble atten[3]) {
	vecCopy(3, atten, light->attenuation);
}

/* Sets the full (not half) angle of a spot light. */
void lightSetSpotAngle(lightLight *light, GLdouble fullAngle) {
	light->spotAngle = fullAngle;
}

/* Sets the light's rotation and translation. The light is positioned at the 
world coordinates position. From that position, the light shines in the 
direction described by the spherical coordinates phi and theta (as in 
vec3Spherical). */
void lightShineFrom(lightLight *light, GLdouble position[3], GLdouble phi, 
		GLdouble theta) {
	GLdouble negZ[3], y[3];
	GLdouble yStd[3] = {0.0, 1.0, 0.0}, negZStd[3] = {0.0, 0.0, -1.0};
	vec3Spherical(1.0, phi, theta, negZ);
	vec3Spherical(1.0, M_PI / 2.0 - phi, theta + M_PI, y);
	mat33BasisRotation(yStd, negZStd, y, negZ, light->rotation);
	vecCopy(3, position, light->translation);
}



/*** OpenGL ***/

/* The '...Loc' arguments are shader locations. This function loads those 
locations with the light's settings. cosLoc gets the cosine of half of the 
spot light angle. */
void lightRender(lightLight *light, GLint positionLoc, GLint colorLoc, 
		GLint attenLoc, GLint dirLoc, GLint cosLoc) {
	GLfloat vec[3];
	vecOpenGL(3, light->color, vec);
	glUniform3fv(colorLoc, 1, vec);
	if (light->lightType == lightOMNI || light->lightType == lightSPOT) {
		vecOpenGL(3, light->translation, vec);
		glUniform3fv(positionLoc, 1, vec);
		vecOpenGL(3, light->attenuation, vec);
		glUniform3fv(attenLoc, 1, vec);
	}
	if (light->lightType == lightDIRECTIONAL || light->lightType == lightSPOT) {
		vec[0] = -light->rotation[0][2];
		vec[1] = -light->rotation[1][2];
		vec[2] = -light->rotation[2][2];
		glUniform3fv(dirLoc, 1, vec);
	}
	if (light->lightType == lightSPOT) {
		vec[0] = cos(0.5 * light->spotAngle);
		glUniform1fv(cosLoc, 1, vec);
	}
}

