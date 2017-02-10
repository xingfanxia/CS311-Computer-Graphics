//Xingfan Xia Jan 14th
//clang 070mainMesh.c 000pixel.o -lglfw -framework OpenGL; ./a.out
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdarg.h>
#include "000pixel.h"
#include "030matrix.c"
#include "070vector.c"
#include "040texture.c"
#include "080renderer.c"


#define renATTRDIMBOUND 16

#define renATTRX 0
#define renATTRY 1
#define renATTRS 2
#define renATTRT 3
#define renATTRR 4
#define renATTRG 5
#define renATTRB 6
#define renUNIFR 0
#define renUNIFG 1
#define renUNIFB 2
#define renTEXR 0
#define renTEXG 1
#define renTEXB 2

/* Sets rgb, based on the other parameters, which are unaltered. attr is an 
interpolated attribute vector. */
void colorPixel(renRenderer *ren, double unif[], texTexture *tex[], 
        double attr[], double rgb[]) {
    texSample(tex[0], attr[renATTRS], attr[renATTRT]);
    rgb[0] = tex[0]->sample[renTEXR] * unif[renUNIFR];
    rgb[1] = tex[0]->sample[renTEXG] * unif[renUNIFG];
    rgb[2] = tex[0]->sample[renTEXB] * unif[renUNIFB];
}

#include "050triangle.c"
#include "070mesh.c"
texTexture texture;
texTexture *tex_0;
renRenderer renderer;
renRenderer *ren;
meshMesh mesh_mesh;
meshMesh *mesh;
// void draw(renRenderer *ren, unif[], *tex[], a[], b[], c[]) {
// 	pixClearRGB(0.0, 0.0, 0.0);
// 	triRender(renRenderer *ren, unif[], *tex[], a[], b[], c[])；
// }

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

		// meshInitializeRectangle(mesh, 200, 300, 200, 300);
		meshInitializeEllipse(mesh, 200, 200, 50, 100, 300);
		double unif[renATTRDIMBOUND] = {1.0, 1.0, 1.0};
		tex_0 = &texture;
		if (texInitializeFile(tex_0, "avatar.jpg") != 0) {
			return 1;
		} else {
			pixClearRGB(0.0, 0.0, 0.0);
			texTexture *tex[renATTRDIMBOUND];
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