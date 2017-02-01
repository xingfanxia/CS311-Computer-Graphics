/**
 * 130mainProjection.c
 * Kaixing Wu
 * Jan 28 2017
 */
#define renATTRX 0
#define renATTRY 1
#define renATTRZ 2
#define renATTRS 3
#define renATTRT 4
#define renATTRN 5
#define renATTRO 6
#define renATTRP 7
#define renATTRDIMBOUND 42
#define renVARYX 0
#define renVARYY 1
#define renVARYZ 2
#define renVARYS 3
#define renVARYT 4
#define renVARYDIMBOUND 16
#define renVERTNUMBOUND 999
#define renUNIFR 0
#define renUNIFG 1
#define renUNIFB 2
#define renUNIFTRANSX 3
#define renUNIFTRANSY 4
#define renUNIFTRANSZ 5
#define renUNIFTHETA 6
#define renUNIFAXX 7
#define renUNIFAXY 8
#define renUNIFAXZ 9
#define renUNIFISOMETRY 10
#define renUNIFCamera 26
#define renTEXR 0
#define renTEXG 1
#define renTEXB 2

#define GLFW_KEY_ENTER 257
#define W 87
#define SKEY 83
#define A 65
#define D 68
#define PI 3.1415926535

#include <stdio.h>
#include <stdarg.h>
#include <math.h>
#include <float.h>
#include "000pixel.h"
#include "100vector.c"
#include "130matrix.c"
#include "040texture.c"
#include "110depth.c"
#include "130renderer.c"

/* If unifParent is NULL, then sets the uniform matrix to the 
rotation-translation M described by the other uniforms. If unifParent is not 
NULL, but instead contains a rotation-translation P, then sets the uniform 
matrix to the matrix product P * M. */
void updateUniform(renRenderer *ren, double unif[], double unifParent[]) {
    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 4; j++){
            unif[renUNIFCamera + i * 4 + j] = ren->viewing[i][j];
        }
    }
    double rot[3][3];
    double axis[3] = {unif[renUNIFAXX], unif[renUNIFAXY], unif[renUNIFAXZ]};
    vecUnit(3, axis, axis);
    double trans[3] = {unif[renUNIFTRANSX], unif[renUNIFTRANSY], unif[renUNIFTRANSZ]};
    mat33AngleAxisRotation(unif[renUNIFTHETA], axis, rot);
    mat44Isometry(rot, trans, (double(*)[4])(&unif[renUNIFISOMETRY]));
    double unifCopy[4][4];
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            unifCopy[i][j] = unif[renUNIFISOMETRY + i * 4 + j];
        }
    }
    if (unifParent != NULL) {
        mat444Multiply((double(*)[4])(&unifParent[renUNIFISOMETRY]), unifCopy, (double(*)[4])(&unif[renUNIFISOMETRY]));
    }
}

/* Writes the vary vector, based on the other parameters. */
// void transformVertex(renRenderer *ren, double unif[], double attr[], double vary[]) {
//     double isometry[4][4];
//     for (int i = 0; i < 4; i++) {
//         for (int j = 0; j < 4; j++){
//             isometry[i][j] = unif[renUNIFISOMETRY + (i * 4) + j];
//         }
//     }
//     double viewing[4][4];
//     for (int i = 0; i < 4; i++) {
//         for (int j = 0; j < 4; j++){
//             viewing[i][j] = unif[renUNIFCamera + (i * 4) + j];
//         }
//     }
//     double scene[4][4];
//     mat444Multiply(isometry, viewing, scene);
//     double V[4];
//     double N[4];
//     double attrXYZ[4] = {attr[renATTRX], attr[renATTRY], attr[renATTRZ], 1};
//     mat441Multiply(scene, attrXYZ, V);
//     V[0] = V[0]/ (V[3]); 
//     V[1] = V[1]/ (V[3]);
//     V[2] = V[2]/ (V[3]);
//     mat441Multiply(ren->viewport, V, vary);
//     vary[renVARYS] = attr[renATTRS];
//     vary[renVARYT] = attr[renATTRT];

//     for (int i = 0; i< 3;i++){
//         printf("%d    %f\n",i, vary[i]);
//     }
// }

void transformVertex(renRenderer *ren, double unif[], double attr[], 
        double vary[]) {
    /*Then perform the viewport transformation. */
    /* First, copy attr S and T to vary. */
    vary[renVARYS] = attr[renATTRS];
    vary[renVARYT] = attr[renATTRT];
    /* Then, do the modeling and viewing transformation. */
    double toBeTransformed[4] = {attr[renATTRX], attr[renATTRY], attr[renATTRZ], 1};
    double transformed1[4], transformed2[4], transformed3[4];
    mat441Multiply((double(*)[4])(&unif[renUNIFISOMETRY]), toBeTransformed, transformed1);
    mat441Multiply((double(*)[4])(&unif[renUNIFCamera]), transformed1, transformed2); 
    
    printf("Transform2 is: {%f, %f, %f, %f}\n", transformed2[0], transformed2[1], transformed2[2], transformed2[3]);
    vecScale(4, 1/transformed2[3], transformed2, transformed2);
    mat441Multiply(ren->viewport, transformed2, transformed3);
    //printf("Viewport is: {%f, %f, %f, %f}\n", ren->viewport[0][0], ren->viewport[0][1], ren->viewport[0][2], ren->viewport[0][3]);
    vary[renVARYX] = transformed3[0];
    vary[renVARYY] = transformed3[1];
    vary[renVARYZ] = transformed3[2];
    vary[3] = transformed3[3];
    //printf("Vary is: {%f, %f, %f, %f}\n", vary[0], vary[1], vary[2], vary[3]);
    vary[renVARYS] = attr[renATTRS];
    vary[renVARYT] = attr[renATTRT];
    /* Assign the transformed screen coordinates to vary. */
    // vary[renVARYX] = transformed2[0];
    // vary[renVARYY] = transformed2[1];
    // vary[renVARYZ] = transformed2[2];
    // vary[renVARYW] = transformed2[3];
    //printf("Vary is: {%f, %f, %f, %f}\n", vary[0], vary[1], vary[2], vary[3]);
}

/* Sets rgb, based on the other parameters, which are unaltered. attr is an 
interpolated attribute vector. */
void colorPixel(renRenderer *ren, double unif[], texTexture *tex[], double vary[], double rgbz[]) {
    texSample(tex[0], vary[renVARYS], vary[renVARYT]);
    rgbz[0] = tex[0]->sample[renTEXR] * unif[renUNIFR];
    rgbz[1] = tex[0]->sample[renTEXG] * unif[renUNIFG];
    rgbz[2] = tex[0]->sample[renTEXB] * unif[renUNIFB];
    rgbz[3] = vary[renVARYZ];
}

#include "110triangle.c"
#include "100mesh.c"
#include "090scene.c"

// textures
texTexture t1, t2, t3;
texTexture *texRoot[1] = {&t1};
texTexture *texChild[1] = {&t2};
texTexture *texGrandChild[1] = {&t3};

// meshes
meshMesh meshRoot;
meshMesh meshChild;
meshMesh meshGrandChild;

// nodes
sceneNode root;
sceneNode child;
sceneNode grandChild;

// depth buffer
depthBuffer depth;

// init renderer
renRenderer ren = {
    .unifDim = 42, 
    .texNum = 1, 
    .attrDim = 3 + 2 + 3,
    .varyDim = 3 + 2,
    .colorPixel = colorPixel,
    .transformVertex = transformVertex,
    .updateUniform = updateUniform,
    .depth = &depth,
    .cameraRotation = {
        {1, 0, 0},
        {0, 1, 0},
        {0, 0, 1}
    },
    .viewing = {
        {1, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 1, 0},
        {0, 0, 0, 1}
    },
    .cameraTranslation = {0, 0, 0},
    .projection = {0, 512, 0, 512, 0, 512},
    .projectionType = 0,
    .viewport = {
        {1, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 1, 0},
        {0, 0, 0, 1}
    },
};

// free the memory for this node
void destroyAllForNode(sceneNode *node) {
    for (char i = 0; i < ren.texNum; i++) {
        texDestroy(node->tex[i]);
    }
    meshDestroy(node->mesh);
}

void draw() {
    pixClearRGB(0.0, 0.0, 0.0);
    depthClearZs(&depth, -DBL_MAX);
    sceneRender(&root, &ren, NULL);
}

void handleKeyUp(int key, int shiftIsDown, int controlIsDown,
        int altOptionIsDown, int superCommandIsDown) {
    // W means up, S means down, A means right, D means Left
    if (!shiftIsDown && !controlIsDown && !altOptionIsDown && !superCommandIsDown && key == W) {
        // rasterization again
        ren.cameraTranslation[0] += 10;
    }
    if (!shiftIsDown && !controlIsDown && !altOptionIsDown && !superCommandIsDown && key == SKEY) {
        // rasterization again
        ren.cameraTranslation[0] -= 10;
    }
    if (!shiftIsDown && !controlIsDown && !altOptionIsDown && !superCommandIsDown && key == A) {
        // rasterization again
        ren.cameraTranslation[1] -= 10;
    }
    if (!shiftIsDown && !controlIsDown && !altOptionIsDown && !superCommandIsDown && key == D) {
        // rasterization again
        ren.cameraTranslation[1] += 10;
    }


}

void handleTimeStep(double oldTime, double newTime) {
    // for animations
    renUpdateViewing(&ren);
    static char inc = 1;
    root.unif[renUNIFTHETA] += 1;
    child.unif[renUNIFTHETA] += 2;
    grandChild.unif[renUNIFTHETA] += 4;
    if (inc) {
        root.unif[renUNIFR] += 0.1;
        child.unif[renUNIFB] -= 0.1;
        grandChild.unif[renUNIFR] += 0.1;
        grandChild.unif[renUNIFG] += 0.1;
        // root.unif[renUNIFTRANSX] += 10;
        if (root.unif[renUNIFR] >= 1.0) {
            inc = 0;
        }
    } else {
        root.unif[renUNIFR] -= 0.1;
        child.unif[renUNIFB] += 0.1;
        grandChild.unif[renUNIFR] -= 0.1;
        grandChild.unif[renUNIFG] -= 0.1;
        // root.unif[renUNIFTRANSX] -= 10;
        if (root.unif[renUNIFR] <= 0.0) {
            inc = 1;
        }
    }
    draw();
}

int main() {
	if (pixInitialize(512, 512, "Rasterizing a Rectangle") != 0) {
		return 1;
	} else { 
        // init textures
        if (texInitializeFile(texRoot[0], "avatar.jpg") != 0) {
            return 0;
        }

        // init meshes
        // meshInitializeBox(&meshRoot, 0, 200, 0, 200, 0, 200);
        meshInitializeSphere(&meshRoot, 100, 40, 40);
        meshInitializeSphere(&meshChild, 50, 40, 40);
        meshInitializeSphere(&meshGrandChild, 25, 40, 40);
        // init depth buffer
        depthInitialize(&depth, 512, 512);
        // inti ren->projection
        //renSetFrustum(&ren, 1, 3.1415/6.0, 10.0, 10.0);
        // init unif
        double unifRoot[renATTRDIMBOUND] = {
            0.0, 0.0, 0.0,
            250, 250, 0,
            0,
            1, -1, 0,
            0, 0, 0, 0, 
            0, 0, 0, 0,
            0, 0, 0, 0,
            0, 0, 0, 0,
            0, 0, 0, 0, 
            0, 0, 0, 0,
            0, 0, 0, 0,
            0, 0, 0, 0
        };
        double unifChild[renATTRDIMBOUND] = {
            0.0, 0.0, 1.0,
            200, 200, 0,
            0,
            1, -1, 0,
            0, 0, 0, 0, 
            0, 0, 0, 0,
            0, 0, 0, 0,
            0, 0, 0, 0,
            0, 0, 0, 0, 
            0, 0, 0, 0,
            0, 0, 0, 0,
            0, 0, 0, 0
        };
        double unifGrandChild[renATTRDIMBOUND] = {
            0.0, 0.0, 0.0,
            50, 50, 0,
            0,
            1, -1, 0,
            0, 0, 0, 0, 
            0, 0, 0, 0,
            0, 0, 0, 0,
            0, 0, 0, 0,
            0, 0, 0, 0, 
            0, 0, 0, 0,
            0, 0, 0, 0,
            0, 0, 0, 0
        };
        // init scenes
        sceneInitialize(&grandChild, &ren, unifGrandChild, texGrandChild, &meshGrandChild, NULL, NULL);
        sceneInitialize(&child, &ren, unifChild, texChild, &meshChild, &grandChild, NULL);
        sceneInitialize(&root, &ren, unifRoot, texRoot, &meshRoot, &child, NULL);
        //mat44Print(ren.viewing);

        // run it
        pixSetKeyUpHandler(handleKeyUp);
        pixSetTimeStepHandler(handleTimeStep);
		pixRun();

        // destroy all stuff
        destroyAllForNode(&root);
        destroyAllForNode(&child);
        destroyAllForNode(&grandChild);
        sceneDestroyRecursively(&root);
        depthDestroy(ren.depth);
		return 0;
	}
}