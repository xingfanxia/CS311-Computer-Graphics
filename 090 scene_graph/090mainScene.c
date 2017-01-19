//Xingfan Xia Jan 14th
//clang 090mainScene.c 000pixel.o -lglfw -framework OpenGL; ./a.out
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdarg.h>
#include "000pixel.h"
#include "030matrix.c"
#include "070vector.c"
#include "040texture.c"
#include "090renderer.c"


#define renVARYDIMBOUND 16
#define renVERTNUMBOUND 16
#define renATTRX 0
#define renATTRY 1
#define renATTRS 2
#define renATTRT 3
#define renATTRR 4
#define renATTRG 5
#define renATTRB 6
#define renVARYX 0
#define renVARYY 1
#define renVARYS 2
#define renVARYT 3
#define renVARYR 4
#define renVARYG 5
#define renVARYB 6
#define renUNIFR 0
#define renUNIFG 1
#define renUNIFB 2
#define renTEXR 0
#define renTEXG 1
#define renTEXB 2

/* Sets rgb, based on the other parameters, which are unaltered. attr is an 
interpolated attribute vector. */
void colorPixel(renRenderer *ren, double unif[], texTexture *tex[], 
        double vary[], double rgb[]) {
    texSample(tex[0], vary[renVARYS], vary[renVARYT]);
    rgb[0] = tex[0]->sample[renTEXR] * unif[renUNIFR];
    rgb[1] = tex[0]->sample[renTEXG] * unif[renUNIFG];
    rgb[2] = tex[0]->sample[renTEXB] * unif[renUNIFB];
}

void rotateMatrix(double angle, double matrix[2][2]) {
	double rad = M_PI/180.0 * angle;
	matrix[0][0] = cos(rad);
	matrix[0][1] = -sin(rad);
	matrix[1][0] = sin(rad);
	matrix[1][1] = cos(rad);
}

/* Writes the vary vector, based on the other parameters. */
void transformVertex(renRenderer *ren, double unif[], double attr[], 
        double vary[]) {
    /* For now, just copy attr to varying. Baby steps. */
    double angle = 0.0;
    double rotationMat[2][2];
    double original[2];
    original[0] = attr[renATTRX];
    original[1] = attr[renATTRY];
    rotateMatrix(angle, rotationMat);

    vary[renVARYX] = attr[renATTRX];
    vary[renVARYY] = attr[renATTRY];
    mat221Multiply(rotationMat, original, vary);
    vary[renVARYS] = attr[renATTRS];
    vary[renVARYT] = attr[renATTRT];
}


#include "090triangle.c"
#include "090mesh.c"
texTexture texture;
texTexture *tex_0;



int main(void) {
	if (pixInitialize(512, 512, "Pixel Graphics") != 0)
		return 1;
	else {
		renRenderer renderer = {
			.unifDim = 3,
			.texNum = 2,
			.varyDim = 4,
			.transformVertex = transformVertex,
			.colorPixel = colorPixel 
		};

		renRenderer *ren = &renderer;

		meshMesh mesh_1 = {
			.triNum = 2,
			.vertNum = 4,
			.attrDim = 4
		};
		meshMesh *mesh = &mesh_1;


		// meshInitializeRectangle(mesh, 200, 300, 200, 300);
		meshInitializeEllipse(mesh, 300, 300, 100, 100, 30);
		double unif[renVARYDIMBOUND] = {1.0, 1.0, 1.0};
		tex_0 = &texture;
		if (texInitializeFile(tex_0, "avatar.jpg") != 0) {
			return 1;
		} else {
			pixClearRGB(0.0, 0.0, 0.0);
			texTexture *tex[renVARYDIMBOUND];
			tex_0->filtering = texQUADRATIC;
			tex[0] = tex_0;
			meshRender(mesh, ren, unif, tex);
			pixRun();
			texDestroy(tex[0]);
			meshDestroy(mesh);
			return 0;
		}
	}	
}