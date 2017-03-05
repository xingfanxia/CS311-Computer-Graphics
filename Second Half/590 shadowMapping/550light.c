


/* Feel free to read from this struct's members, but don't write to them, 
except through accessor functions. */
typedef struct lightLight lightLight;
struct lightLight {
	GLdouble translation[3];
	GLdouble color[3];
	GLdouble attenuation[3];
};

/* Sets the light's translation. */
void lightSetTranslation(lightLight *light, GLdouble transl[3]) {
	vecCopy(3, transl, light->translation);
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

/*** OpenGL ***/

/* The '...Loc' arguments are shader locations. This function loads those 
locations with the light's settings. */
void lightRender(lightLight *light, GLint positionLoc, GLint colorLoc, 
		GLint attenLoc) {
	GLfloat vec[3];
	vecOpenGL(3, light->translation, vec);
	glUniform3fv(positionLoc, 1, vec);
	vecOpenGL(3, light->color, vec);
	glUniform3fv(colorLoc, 1, vec);
	vecOpenGL(3, light->attenuation, vec);
	glUniform3fv(attenLoc, 1, vec);
}


