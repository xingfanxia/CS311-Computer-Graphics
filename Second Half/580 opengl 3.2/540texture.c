


/* These pre-compiler directives need to be in this order. */
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STBI_FAILURE_USERMSG

/* Feel free to read from this struct's members, but don't write to them except 
through the accessor functions. */
typedef struct texTexture texTexture;
struct texTexture {
	GLuint width, height, texelDim;
	GLuint openGL;
};

/* minification and magnification should be GL_NEAREST or GL_LINEAR. leftRight 
and bottomTop should be one of GL_CLAMP, GL_REPEAT, etc. */
void texSetFilteringBorder(texTexture *tex, GLint minification, 
		GLint magnification, GLint leftRight, GLint bottomTop) {
	glBindTexture(GL_TEXTURE_2D, tex->openGL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minification);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magnification);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, leftRight);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, bottomTop);
}

/* Loads the given image file into an OpenGL texture. The width and height of 
the image must be powers of 2. For other parameter meanings, see 
texSetFilteringBorder. Returns 0 on success, non-zero on failure. On success, 
the user must call texDestroy when finished with the texture. */
int texInitializeFile(texTexture *tex, char *path, GLint minification, 
		GLint magnification, GLint leftRight, GLint bottomTop) {
	/* Use STB Image to load the texture data from the file. */
	int width, height, texelDim;
	unsigned char *rawData;
	rawData = stbi_load(path, &width, &height, &texelDim, 0);
	if (rawData == NULL) {
		fprintf(stderr, "texInitializeFile: failed to load %s\n", path);
		fprintf(stderr, "with STB Image reason: %s.\n", stbi_failure_reason());
		return 1;
	}
	/* Right now we support only 3-channel images. Supporting other texelDims 
	is not hard --- maybe just changing certain parameters to glTexImage2D 
	below, and then using the data correctly in the shader. */
	if (texelDim != 3) {
		fprintf(stderr, "texInitializeFile: %d != 3 channels.\n", texelDim);
		return 2;
	}
	/* Load the data into OpenGL. */
	glGenTextures(1, &(tex->openGL));
	texSetFilteringBorder(tex, minification, magnification, leftRight, 
		bottomTop);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, 
		GL_UNSIGNED_BYTE, rawData);
	stbi_image_free(rawData);
	if (glGetError() != GL_NO_ERROR) {
		fprintf(stderr, "texInitializeFile: OpenGL error.\n");
		glDeleteTextures(1, &(tex->openGL));
		return 3;
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	tex->width = width;
	tex->height = height;
	tex->texelDim = texelDim;
	return 0;
}

/* Deallocates the resources backing the texture. */
void texDestroy(texTexture *tex) {
	glDeleteTextures(1, &(tex->openGL));
}

/* At the start of rendering a frame, the renderer calls this function, to hook 
the texture into a certain texture unit. textureUnit is something like 
GL_TEXTURE0. textureUnitIndex would then be 0. */
void texRender(texTexture *tex, GLenum textureUnit, GLint textureUnitIndex, 
		GLint textureLoc) {
	glActiveTexture(textureUnit);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, tex->openGL);
    glUniform1i(textureLoc, textureUnitIndex);
}

/* At the end of rendering a frame, the renderer calls this function, to unhook 
the texture from a certain texture unit. textureUnit is something like 
GL_TEXTURE0. */
void texUnrender(texTexture *tex, GLenum textureUnit) {
	glActiveTexture(textureUnit);
    glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
}


