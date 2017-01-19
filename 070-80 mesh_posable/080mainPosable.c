//Xingfan Xia Jan 14th

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdarg.h>
#include "000pixel.h"
#include "030matrix.c"
#include "030vector.c"
#include "040texture.c"
#include "080renderer.c"

texTexture texture;
texTexture *tex_0;
renRenderer renderer;
renRenderer *ren;
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

/* Writes the vary vector, based on the other parameters. */
void transformVertex(renRenderer *ren, double unif[], double attr[], 
        double vary[]) {
    /* For now, just copy attr to varying. Baby steps. */
    vary[renVARYX] = attr[renATTRX];
    vary[renVARYY] = attr[renATTRY];
    vary[renVARYS] = attr[renATTRS];
    vary[renVARYT] = attr[renATTRT];
}

#include "080triangle.c"
#include "070mesh.c"
texTexture texture;
texTexture *tex_0;
renRenderer renderer;
renRenderer *ren;
meshMesh mesh_mesh;
meshMesh *mesh;


int main(void) {
	if (pixInitialize(512, 512, "Pixel Graphics") != 0)
		return 1;
	else {
		ren = &renderer;
		ren->unifDim = 3;
		ren->texNum = 2;
		ren->attrDim = 4;

		mesh = &mesh_mesh;
		mesh->triNum = 2;
		mesh->vertNum = 4;
		mesh->attrDim = 4;

		meshInitializeRectangle(mesh, 200, 300, 200, 300);
		// meshInitializeEllipse(mesh, 200, 200, 50, 100, 300);
		double unif[3] = {1.0, 1.0, 1.0};
		tex_0 = &texture;
		if (texInitializeFile(tex_0, "avatar.jpg") != 0) {
			return 1;
		} else {
			pixClearRGB(0.0, 0.0, 0.0);
			texTexture *tex[ren->texNum];
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