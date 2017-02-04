


/* On macOS, compile with...
    clang 140mainClipping.c 080pixel.o -lglfw -framework OpenGL
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GLFW/glfw3.h>
#include <stdarg.h>

#include "000pixel.h"
#include "100vector.c"
#include "130matrix.c"
#include "040texture.c"

#define renVERTNUMBOUND 1000
#define renVARYDIMBOUND 16
#define renHUGEDEPTH -10000000.0



#define renATTRX 0
#define renATTRY 1
#define renATTRZ 2
#define renATTRS 3
#define renATTRT 4
#define renATTRN 5
#define renATTRO 6
#define renATTRP 7

#define renVARYX 0
#define renVARYY 1
#define renVARYZ 2
#define renVARYW 3
#define renVARYS 4
#define renVARYT 5

#define renUNIFR 0
#define renUNIFG 1
#define renUNIFB 2
#define renUNIFTRANSX 3
#define renUNIFTRANSY 4
#define renUNIFTRANSZ 5
#define renUNIFALPHA 6
#define renUNIFPHI 7
#define renUNIFTHETA 8
#define renUNIFISOMETRY 9
#define renUNIFVIEWING (renUNIFISOMETRY + 16)

#define renTEXR 0
#define renTEXG 1
#define renTEXB 2

#include "110depth.c"
#include "130renderer.c"
#include "110triangle.c"
#include "140clipping.c"
#include "140mesh.c"
#include "090scene.c"
depthBuffer depthBuf;
texTexture textureHoriz, textureVert, textureWater, textureTrunk, textureTree;
texTexture *texHoriz, *texVert, *texWater, *texTrunk, *texTree;
renRenderer ren;
meshMesh meshLand, meshHoriz, meshVert, meshWater, meshTrunk, meshTree;
double unifA[3 + 3 + 3 + 16 + 16] = {
	1.0, 1.0, 1.0, 
	0.0, 0.0, 0.0, 
	0.0, M_PI / 4.0, M_PI / 4.0, 
	1.0, 0.0, 0.0, 0.0, 
	0.0, 1.0, 0.0, 0.0, 
	0.0, 0.0, 1.0, 0.0, 
	0.0, 0.0, 0.0, 1.0, 
	1.0, 0.0, 0.0, 0.0, 
	0.0, 1.0, 0.0, 0.0, 
	0.0, 0.0, 1.0, 0.0, 
	0.0, 0.0, 0.0, 1.0};
double unifB[3 + 3 + 3 + 16 + 16] = {
	1.0, 1.0, 1.0, 
	10.0, 30.0, 5.0, 
	0.0, 0.0, 0.0, 
	1.0, 0.0, 0.0, 0.0, 
	0.0, 1.0, 0.0, 0.0, 
	0.0, 0.0, 1.0, 0.0, 
	0.0, 0.0, 0.0, 1.0, 
	1.0, 0.0, 0.0, 0.0, 
	0.0, 1.0, 0.0, 0.0, 
	0.0, 0.0, 1.0, 0.0, 
	0.0, 0.0, 0.0, 1.0};
double unifC[3 + 3 + 3 + 16 + 16] = {
	1.0, 1.0, 1.0, 
	0.0, 0.0, 7.0, 
	0.0, 0.0, 0.0, 
	1.0, 0.0, 0.0, 0.0, 
	0.0, 1.0, 0.0, 0.0, 
	0.0, 0.0, 1.0, 0.0, 
	0.0, 0.0, 0.0, 1.0, 
	1.0, 0.0, 0.0, 0.0, 
	0.0, 1.0, 0.0, 0.0, 
	0.0, 0.0, 1.0, 0.0, 
	0.0, 0.0, 0.0, 1.0};
sceneNode nodeHoriz, nodeVert, nodeWater, nodeTrunk, nodeTree;
double cameraTarget[3] = {30.0, 30.0, 5.0};
double cameraRho = 30.0, cameraPhi = M_PI / 4.0, cameraTheta = M_PI / 4.0;

void draw(void) {
	pixClearRGB(0.0, 0.0, 0.0);
	depthClearZs(&depthBuf, renHUGEDEPTH);
	renLookAt(&ren, cameraTarget, cameraRho, cameraPhi, cameraTheta);
	renUpdateViewing(&ren);
	sceneRender(&nodeHoriz, &ren, NULL);
}

void handleKeyUp(int key, int shiftIsDown, int controlIsDown, 
		int altOptionIsDown, int superCommandIsDown) {
	if (key == GLFW_KEY_ENTER) {
		if (textureHoriz.filtering == texQUADRATIC)
			texSetFiltering(&textureHoriz, texNEAREST);
		else
			texSetFiltering(&textureHoriz, texQUADRATIC);
	} else if (key == GLFW_KEY_L) {
		if (ren.projectionType == renPERSPECTIVE)
			renSetFrustum(&ren, renORTHOGRAPHIC, M_PI / 3.0, cameraRho, 10.0);
		else
			renSetFrustum(&ren, renPERSPECTIVE, M_PI / 3.0, cameraRho, 10.0);
	} else if (key == GLFW_KEY_LEFT)
		cameraTarget[0] -= 1.0;
	else if (key == GLFW_KEY_RIGHT)
		cameraTarget[0] += 1.0;
	else if (key == GLFW_KEY_DOWN)
		cameraTarget[1] -= 1.0;
	else if (key == GLFW_KEY_UP)
		cameraTarget[1] += 1.0;
	else if (key == GLFW_KEY_O)
		cameraTheta -= 0.1;
	else if (key == GLFW_KEY_P)
		cameraTheta += 0.1;
	else if (key == GLFW_KEY_I)
		cameraPhi -= 0.1;
	else if (key == GLFW_KEY_K)
		cameraPhi += 0.1;
	else if (key == GLFW_KEY_U)
		cameraRho -= 1.0;
	else if (key == GLFW_KEY_J)
		cameraRho += 1.0;
	draw();
}

void handleTimeStep(double oldTime, double newTime) {
	if (floor(newTime) - floor(oldTime) >= 1.0)
		printf("handleTimeStep: %f frames/sec\n", 1.0 / (newTime - oldTime));
	//sceneSetOneUniform(&nodeA, renUNIFALPHA, 
	//	nodeA.unif[renUNIFALPHA] + (newTime - oldTime) / 4.0);
	//draw();
}

/* Sets rgbz, based on the other parameters, which are unaltered. vary is an 
interpolated varying (transformed attribute) vector. */
void colorPixel(renRenderer *ren, double unif[], texTexture *tex[], 
		double vary[], double rgbz[]) {
	texSample(tex[0], vary[renVARYS], vary[renVARYT]);
	rgbz[0] = tex[0]->sample[renTEXR] * unif[renUNIFR];
	rgbz[1] = tex[0]->sample[renTEXG] * unif[renUNIFG];
	rgbz[2] = tex[0]->sample[renTEXB] * unif[renUNIFB];
	rgbz[3] = vary[renVARYZ];
}

/* Sets varying, based on the other parameters, which are unaltered. */
void transformVertex(renRenderer *ren, double unif[], double attr[], 
		double vary[]) {
	double homog[4] = {attr[renATTRX], attr[renATTRY], attr[renATTRZ], 1.0};
	double model[4];
	mat441Multiply((double(*)[4])(&unif[renUNIFISOMETRY]), homog, model);
	mat441Multiply((double(*)[4])(&unif[renUNIFVIEWING]), model, vary);
	vary[renVARYS] = attr[renATTRS];
	vary[renVARYT] = attr[renATTRT];
}

/* If unifParent is NULL, then sets the uniform matrix to the 
rotation-translation M described by the other uniforms. If unifParent is not 
NULL, but instead contains a rotation-translation P, then sets the uniform 
matrix to the matrix product P * M. */
void updateUniform(renRenderer *ren, double unif[], double unifParent[]) {
	double axis[3] = {0.0, 0.0, 1.0};
	vec3Spherical(1.0, unif[renUNIFPHI], unif[renUNIFTHETA], axis);
	double rot[3][3];
	mat33AngleAxisRotation(unif[renUNIFALPHA], axis, rot);
	double trans[3] = {unif[renUNIFTRANSX], unif[renUNIFTRANSY], 
		unif[renUNIFTRANSZ]};
	if (unifParent == NULL)
		mat44Isometry(rot, trans, (double(*)[4])(&unif[renUNIFISOMETRY]));
	else {
		double m[4][4];
		mat44Isometry(rot, trans, m);
		mat444Multiply((double(*)[4])(&unifParent[renUNIFISOMETRY]), m, 
			(double(*)[4])(&unif[renUNIFISOMETRY]));
	}
	vecCopy(16, (double *)(ren->viewing), &unif[renUNIFVIEWING]);
}

int main(void) {
	texHoriz = &textureHoriz;
	texVert = &textureVert;
	texWater = &textureWater;
	texTrunk = &textureTrunk;
	texTree = &textureTree;
	ren.unifDim = 3 + 3 + 3 + 16 + 16;
	ren.attrDim = 3 + 2 + 3;
	ren.varyDim = 4 + 2;
	ren.texNum = 1;
	ren.transformVertex = transformVertex;
	ren.colorPixel = colorPixel;
	ren.updateUniform = updateUniform;
	ren.depth = &depthBuf;
	double zs[12][12] = {
		{5.0, 5.0, 5.0, 5.0, 5.0, 5.0, 5.0, 5.0, 5.0, 5.0, 5.0, 20.0}, 
		{5.0, 5.0, 5.0, 5.0, 5.0, 5.0, 5.0, 5.0, 5.0, 5.0, 20.0, 25.0}, 
		{5.0, 5.0, 10.0, 12.0, 5.0, 5.0, 5.0, 5.0, 5.0, 5.0, 20.0, 25.0}, 
		{5.0, 5.0, 10.0, 10.0, 5.0, 5.0, 5.0, 5.0, 5.0, 20.0, 25.0, 27.0}, 
		{0.0, 0.0, 5.0, 5.0, 5.0, 0.0, 0.0, 0.0, 0.0, 20.0, 20.0, 25.0}, 
		{0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 20.0, 25.0}, 
		{0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 
		{0.0, 0.0, 0.0, 0.0, 0.0, 5.0, 5.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 
		{0.0, 0.0, 0.0, 0.0, 0.0, 5.0, 7.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 
		{0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 20.0, 20.0}, 
		{5.0, 5.0, 5.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 20.0, 20.0, 20.0}, 
		{10.0, 10.0, 5.0, 5.0, 0.0, 0.0, 0.0, 5.0, 10.0, 15.0, 20.0, 25.0}};
	double ws[12][12] = {
		{1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0}, 
		{1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0}, 
		{1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0}, 
		{1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0}, 
		{1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0}, 
		{1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0}, 
		{1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0}, 
		{1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0}, 
		{1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0}, 
		{1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0}, 
		{1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0}, 
		{1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0}};
	if (pixInitialize(512, 512, "Pixel Graphics") != 0)
		return 1;
	else if (depthInitialize(&depthBuf, 512, 512) != 0)
		return 2;
	else if (texInitializeFile(&textureHoriz, "grass.jpg") != 0)
		return 3;
	else if (texInitializeFile(&textureVert, "granite.jpg") != 0)
		return 4;
	else if (texInitializeFile(&textureWater, "water.jpg") != 0)
		return 4;
	else if (texInitializeFile(&textureTrunk, "trunk.jpg") != 0)
		return 4;
	else if (texInitializeFile(&textureTree, "tree.jpg") != 0)
		return 4;
	else if (meshInitializeLandscape(&meshLand, 12, 12, 5.0, (double *)zs) != 0)
		return 5;
	else if (meshInitializeDissectedLandscape(&meshHoriz, &meshLand, M_PI / 3.0, 1) != 0)
		return 6;
	else if (meshInitializeDissectedLandscape(&meshVert, &meshLand, M_PI / 3.0, 0) != 0)
		return 6;
	else if (meshInitializeLandscape(&meshWater, 12, 12, 5.0, (double *)ws) != 0)
		return 7;
	else if (meshInitializeSphere(&meshTree, 5.0, 4, 8) != 0)
		return 7;
	else if (meshInitializeCapsule(&meshTrunk, 1.0, 10.0, 1, 8) != 0)
		return 7;
	else if (sceneInitialize(&nodeTree, &ren, unifC, &texTree, &meshTree, NULL, NULL) != 0)
		return 5;
	else if (sceneInitialize(&nodeTrunk, &ren, unifB, &texTrunk, &meshTrunk, &nodeTree, NULL) != 0)
		return 5;
	else if (sceneInitialize(&nodeWater, &ren, unifA, &texWater, &meshWater, NULL, &nodeTrunk) != 0)
		return 5;
	else if (sceneInitialize(&nodeVert, &ren, unifA, &texVert, &meshVert, NULL, &nodeWater) != 0)
		return 7;
	else if (sceneInitialize(&nodeHoriz, &ren, unifA, &texHoriz, &meshHoriz, &nodeVert, NULL) != 0)
		return 8;
	else {
		meshDestroy(&meshLand);
		int i;
		double *vert, normal[2];
		for (i = 0; i < meshVert.vertNum; i += 1) {
			vert = meshGetVertexPointer(&meshVert, i);
			normal[0] = -vert[renATTRO];
			normal[1] = vert[renATTRN];
			vert[renATTRS] = (vert[renATTRX] * normal[0] + vert[renATTRY] * normal[1]) / 20.0;
			vert[renATTRT] = vert[renATTRZ] / 20.0;
		}
		textureHoriz.leftRight = texREPEAT;
		textureHoriz.topBottom = texREPEAT;
		textureVert.leftRight = texREPEAT;
		textureVert.topBottom = texREPEAT;
		textureWater.leftRight = texREPEAT;
		textureWater.topBottom = texREPEAT;
		renSetFrustum(&ren, renORTHOGRAPHIC, M_PI / 3.0, cameraRho, 10.0);
		draw();
		pixSetKeyUpHandler(handleKeyUp);
		pixSetTimeStepHandler(handleTimeStep);
		pixRun();
		meshDestroy(&meshTrunk);
		meshDestroy(&meshTree);
		meshDestroy(&meshHoriz);
		meshDestroy(&meshVert);
		meshDestroy(&meshWater);
		texDestroy(&textureTrunk);
		texDestroy(&textureTree);
		texDestroy(&textureHoriz);
		texDestroy(&textureVert);
		texDestroy(&textureWater);
		sceneDestroyRecursively(&nodeHoriz);
		depthDestroy(&depthBuf);
		return 0;
	}
}


