//Xingfan Xia Jan 20th
//clang 090mainScene.c 000pixel.o -lglfw -framework OpenGL; ./a.out
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdarg.h>
#include "000pixel.h"
#include "090matrix.c"
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
#define renUNIFTHETA 3
#define renUNIFTRANSX 4
#define renUNIFTRANSY 5
#define renUNIFISOMETRY 6

#define renTEXR 0
#define renTEXG 1
#define renTEXB 2

//first [0, 1, 2] background rgb, [3] angle theta, [4,5] translation vector
double unif[3+3+9] = {1.0, 1.0, 1.0, 45.0, 10.0, 10.0};

/* Sets rgb, based on the other parameters, which are unaltered. attr is an 
interpolated attribute vector. */
void colorPixel(renRenderer *ren, double unif[], texTexture *tex[], 
        double vary[], double rgb[]) {
    texSample(tex[0], vary[renVARYS], vary[renVARYT]);
    rgb[0] = tex[0]->sample[renTEXR] * unif[renUNIFR];
    rgb[1] = tex[0]->sample[renTEXG] * unif[renUNIFG];
    rgb[2] = tex[0]->sample[renTEXB] * unif[renUNIFB];
}

// void rotateMatrix(double angle, double matrix[2][2]) {
// 	double rad = M_PI/180.0 * angle;
// 	matrix[0][0] = cos(rad);
// 	matrix[0][1] = -sin(rad);
// 	matrix[1][0] = sin(rad);
// 	matrix[1][1] = cos(rad);
// }

/* Writes the vary vector, based on the other parameters. */
void transformVertex(renRenderer *ren, double unif[], double attr[], 
        double vary[]) {
    /* For now, just copy attr to varying. Baby steps. */
 //    double angle = 45.0;
 //    double x, y  = 10.0, 10.0;

 //   	double isom[3][3];
	// mat33Isometry(a, x, y, isom);
	// mat33Isometry(unif[renUNIFTHETA], unif[renUNIFTRANSX], 
	// 	unif[renUNIFTRANSY], (double(*)[3])(&unif[renUNIFISOMETRY]));
	
	double original[3] = {attr[renATTRX], attr[renATTRY], 1};

    mat331Multiply((double(*)[3])(&unif[renUNIFISOMETRY]), original, vary);
    // vary[renVARYX] = attr[renATTRX];
    // vary[renVARYY] = attr[renATTRY];
    vary[renVARYS] = attr[renATTRS];
    vary[renVARYT] = attr[renATTRT];
}

/* If unifParent is NULL, then sets the uniform matrix to the 
rotation-translation M described by the other uniforms. If unifParent is not 
NULL, but instead contains a rotation-translation P, then sets the uniform 
matrix to the matrix product P * M. */
void updateUniform(renRenderer *ren, double unif[], double unifParent[]) {
    if (unifParent == NULL)
        /* The nine uniforms for storing the matrix start at index 
        renUNIFISOMETRY. So &unif[renUNIFISOMETRY] is an array containing those 
        nine numbers. We use '(double(*)[3])' to cast it to a 3x3 matrix. */
        mat33Isometry(unif[renUNIFTHETA], unif[renUNIFTRANSX], 
            unif[renUNIFTRANSY], (double(*)[3])(&unif[renUNIFISOMETRY]));
    else {
        double m[3][3];
        mat33Isometry(unif[renUNIFTHETA], unif[renUNIFTRANSX], 
            unif[renUNIFTRANSY], m);
        mat333Multiply((double(*)[3])(&unifParent[renUNIFISOMETRY]), m, 
            (double(*)[3])(&unif[renUNIFISOMETRY]));
    }
}



#include "090triangle.c"
#include "090mesh.c"
#include "090scene.c"
texTexture texture;
meshMesh mesher_1;
meshMesh mesher_2;
meshMesh mesher_3;
meshMesh mesher_4;

sceneNode nodeA;
sceneNode nodeB;
sceneNode nodeC;
sceneNode nodeD;
renRenderer renderer = {
	.unifDim = 16,
	.texNum = 2,
	.varyDim = 4,
	.transformVertex = transformVertex,
	.colorPixel = colorPixel,
	.updateUniform = updateUniform
};


/*set one uniform in the unif*/
void sceneSetOneUniform(sceneNode *node, int i, double unif){
        node -> unif[i] = unif;
}

void draw(void){
    pixClearRGB(0.0, 0.0, 0.0);
    sceneRender(&nodeA, &renderer, NULL);
}

void handleTimeStep(double oldTime, double newTime) {
    //redraw the scene by a new unifAnge theta as time changes
    if (floor(newTime) - floor(oldTime) >= 1.0)
        printf("handleTimeStep: %f frames/sec\n", 1.0 / (newTime - oldTime));
        sceneSetOneUniform(&nodeA, renUNIFTHETA, newTime*1);
        draw();
}

int main(void) {
	renRenderer *ren = &renderer;
	meshMesh *mesh1 = &mesher_1;
	meshMesh *mesh2 = &mesher_2;
	meshMesh *mesh3 = &mesher_3;
	meshMesh *mesh4 = &mesher_4;
	texTexture *tex[renVARYDIMBOUND];
	tex[0] = &texture;
	tex[0]->filtering = texQUADRATIC;

    //init unif for each node
	double unifA[3+1+2+9+1] = {1.0, 1.0, 1.0, 0.5, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0};
    double unifB[3+1+2+9+1] = {1.0, 1.0, 1.0, 0.9, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0};
    double unifC[3+1+2+9+1] = {1.0, 1.0, 1.0, 0.2, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0};
    double unifD[3+1+2+9+1] = {1.0, 1.0, 1.0, 0.8, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0};
	
    //init mesh, tex and scene nodes
    if (pixInitialize(512, 512, "Pixel Graphics") != 0) {
		return 1;
	} else if (meshInitializeRectangle(mesh1, 0, 400, 0, 400) != 0){
        return 2;
    } else if (meshInitializeEllipse(mesh2, 250.0, 200.0, 50.0, 50.0, 50.0) != 0){
        return 3;
    } else if (meshInitializeEllipse(mesh3, 150.0, 150.0, 60.0, 60.0, 50.0) != 0){
        return 4;
    } else if (meshInitializeEllipse(mesh4, 350.0, 350.0, 70.0, 70.0, 50.0) != 0){
        return 5;
    } else if (texInitializeFile(&texture, "avatar.jpg") != 0) {
    	return 6;
    } else if (sceneInitialize(&nodeA, ren, unifA, tex, mesh1, &nodeB, NULL) != 0){
        return 7;
    } else if (sceneInitialize(&nodeB, ren, unifB, tex, mesh2, &nodeC, NULL) != 0){
        return 8;
    } else if (sceneInitialize(&nodeC, ren, unifC, tex, mesh3, NULL, &nodeD) != 0){
        return 9;
    } else if (sceneInitialize(&nodeD, ren, unifD, tex, mesh4, NULL, NULL) != 0){
        return 10;
	}else {
        //draw the scene
		draw();
        pixSetTimeStepHandler(handleTimeStep);
		pixRun();

        //destroy
		texDestroy(tex[0]);
		meshDestroy(mesh1);
		meshDestroy(mesh2);
		meshDestroy(mesh3);
		meshDestroy(mesh4);
		sceneDestroy(&nodeA);
        sceneDestroy(&nodeB);
        sceneDestroy(&nodeC);
        sceneDestroy(&nodeD);
		return 0;
	}	
}