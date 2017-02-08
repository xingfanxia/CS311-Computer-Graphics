//Xingfan Xia Feb 7th//clang 170mainSpecular.c 000pixel.o -lglfw -framework OpenGL;./a.out
//clang -g 170mainSpecular.c 000pixel.o -lglfw -framework OpenGL;./a.out
//lldb a.out
//lldb -> run
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include "000pixel.h"
#include "110depth.c"
#include "100vector.c"
#include "130matrix.c"
#include "040texture.c"
#include "130renderer.c"

//Defining bounds
#define renVARYDIMBOUND 16
#define renVERTNUMBOUND 1000

//Attr indices
#define renATTRX 0
#define renATTRY 1
#define renATTRZ 2
#define renATTRS 3
#define renATTRT 4
#define renATTRNORMALX 5
#define renATTRNORMALY 6
#define renATTRNORMALZ 7
// #define renATTRR 8
// #define renATTRG 9
// #define renATTRB 10

//Vary indices
#define renVARYX 0
#define renVARYY 1
#define renVARYZ 2
#define renVARYW 3
#define renVARYS 4
#define renVARYT 5
#define renVARYWORLDX 6
#define renVARYWORLDY 7
#define renVARYWORLDZ 8
#define renVARYNORMALX 9
#define renVARYNORMALY 10
#define renVARYNORMALZ 11
#define renVARYR 12
#define renVARYG 13
#define renVARYB 14

//UNIF indices
#define renUNIFR 0
#define renUNIFG 1
#define renUNIFB 2
#define renUNIFTHETA 3
#define renUNIFTRANSX 4
#define renUNIFTRANSY 5
#define renUNIFTRANSZ 6
#define renUNIFAXIS 7
#define renUNIFAXISX 7
#define renUNIFAXISY 8
#define renUNIFAXISZ 9
#define renUNIFISOMETRY 10
#define renUNIFCAMERAVIEWING 26
#define renUNIFLIGHTPOSITIONX 42
#define renUNIFLIGHTPOSITIONY 43
#define renUNIFLIGHTPOSITIONZ 44
#define renUNIFLIGHTR 45
#define renUNIFLIGHTG 46
#define renUNIFLIGHTB 47
#define renUNIFCAMERAWORLDPOSITION 48
#define renUNIFSPECULARSHINENESS 51

//tex indices
#define renTEXR 0
#define renTEXG 1
#define renTEXB 2

//keys for control
#define W_KEY 87
#define S_KEY 83
#define A_KEY 65
#define D_KEY 68
#define One_KEY 49
#define TWO_KEY 50

//max macro
#define max(a,b) \
({ __typeof__ (a) _a = (a); \
   __typeof__ (b) _b = (b); \
 _a > _b ? _a : _b; })

void setLight(double unif[], double light_worldX, double light_worldY, double light_worldZ,
                double light_r, double light_g, double light_b) {
    unif[renUNIFLIGHTPOSITIONX] = light_worldX;
    unif[renUNIFLIGHTPOSITIONY] = light_worldY;
    unif[renUNIFLIGHTPOSITIONZ] = light_worldZ;
    unif[renUNIFLIGHTR] = light_r;
    unif[renUNIFLIGHTG] = light_g;
    unif[renUNIFLIGHTB] = light_b;
}

void setCameraPos(double unif[], double cameraPos[3]) {
    unif[renUNIFCAMERAWORLDPOSITION] = cameraPos[0];
    unif[renUNIFCAMERAWORLDPOSITION+1] = cameraPos[1];
    unif[renUNIFCAMERAWORLDPOSITION+1] = cameraPos[2];
}

void copyVecsforLights(int n, int startv, double original[], double copy[]) {
    for (int i = 0; i < n; i += 1) {
        copy[i] = original[startv+i];
    }
}
/* Sets rgb, based on the other parameters, which are unaltered. attr is an 
interpolated attribute vector. */
void colorPixel(renRenderer *ren, double unif[], texTexture *tex[], 
        double vary[], double rgb[]) {
    texSample(tex[0], vary[renVARYS], vary[renVARYT]);
    //surfaceRGB
    double s_rgb[3];
    s_rgb[0] = tex[0]->sample[renTEXR] * unif[renUNIFR];
    s_rgb[1] = tex[0]->sample[renTEXG] * unif[renUNIFG];
    s_rgb[2] = tex[0]->sample[renTEXB] * unif[renUNIFB];
    
    printf("s_rgb: %f %f %f\n", s_rgb[0], s_rgb[1], s_rgb[2]);
    /* Lighting 1: diffuse lighting
    R = d * lightR * surfaceR
    G = d * lightG * surfaceG
    B = d * lightB * surfaceB
    where d = max(0, vecDot(vectorNormal, vectorLight))
    */
    double lightRGB[3];
    double lightPosition[3];
    double tempNormal[3];
    double tempWorldPixel[3];
    double vectorLight[3];
    double d;
    double dotProduct;
    //copy light position and lightRGB for calculation
    vecCopy(3, &unif[renUNIFLIGHTPOSITIONX], lightPosition);
    vecCopy(3, &unif[renUNIFLIGHTR], lightRGB);
    vecCopy(3, &vary[renVARYNORMALX], tempNormal);
    vecCopy(3, &vary[renVARYWORLDX], tempWorldPixel);
    //calcualte vectorLight
    vecSubtract(3, lightPosition, tempWorldPixel, vectorLight);
    vecUnit(3, vectorLight, vectorLight);
    dotProduct = vecDot(3, tempNormal, vectorLight);
    d = max(0, dotProduct);
    for (int i = 0; i < 3; i += 1) {
        //set diffuse rgb
        rgb[i] = d * lightRGB[i] * s_rgb[i];
    }

    /* Lighting 2: specular lighting
    ReflectionDir = 2 * vecDot(vecN, vecL) * vecN - vecL
    specInt = vecDot(cameraDir, ReflectionDir)
    spec_rgb = sepcInt * LightRGB * ClearCoatRGB(white here)
    rgb = spec_rgb + diffuse_rgb
    */
    double UnitCameraDirection[3];
    double UnitReflectionDirection[3];
    double tempVec[3];
    double cameraPos[3] = {unif[renUNIFCAMERAWORLDPOSITION], unif[renUNIFCAMERAWORLDPOSITION+1], unif[renUNIFCAMERAWORLDPOSITION+2]};
    double ClearCoatRGB[3] = {1.0, 1.0, 1.0};
    double spec_rgb[3];
    double shineness = unif[renUNIFSPECULARSHINENESS];

    vecSubtract(3, cameraPos, tempWorldPixel, UnitCameraDirection);
    vecUnit(3, UnitCameraDirection, UnitCameraDirection);
    double tempDotProduct = vecDot(3, tempNormal, vectorLight);
    vecScale(3, tempDotProduct, tempNormal, tempVec);
    vecScale(3, 2, tempVec, tempVec);
    vecSubtract(3, tempVec, vectorLight, UnitReflectionDirection);
    vecUnit(3, UnitReflectionDirection, UnitReflectionDirection);

    double specIntensity = vecDot(3, UnitCameraDirection, UnitReflectionDirection);
    for (int i = 0; i < 3; i += 1) {
        //cal and add spec_rgb to overall rgb
        spec_rgb[i] = specIntensity*lightRGB[i]*ClearCoatRGB[i]*shineness;
        rgb[i] += spec_rgb[i];
    }


    // printf("rgb: %f %f %f\n", rgb[0], rgb[1], rgb[2]);
    
    // copy z value from unif
    rgb[3] = vary[renVARYZ];
}

/* Writes the vary vector, based on the other parameters. */
void transformVertex(renRenderer *ren, double unif[], double attr[], 
        double vary[]) {

    //temp holder for result of M (Viewing and Modelling transformation)
    double world[4];
    //then do it over the original coords
    double original[4] = {attr[renATTRX], attr[renATTRY], attr[renATTRZ], 1};

    //write in attributes to vary from attr
    double tempNormal[4] = {attr[renATTRNORMALX], attr[renATTRNORMALY], attr[renATTRNORMALZ], 0};
    mat441Multiply((double(*)[4])(&unif[renUNIFISOMETRY]), tempNormal, tempNormal);
    vecUnit(4, tempNormal, tempNormal);
    vary[renVARYNORMALX] = tempNormal[0];
    vary[renVARYNORMALY] = tempNormal[1];
    vary[renVARYNORMALZ] = tempNormal[2];   

    //original to world
    mat441Multiply((double(*)[4])(&unif[renUNIFISOMETRY]), original, world);
    //write world position of a traingle vertex to
    //varyings
    vary[renVARYWORLDX] = world[renATTRX];
    vary[renVARYWORLDY] = world[renATTRY];
    vary[renVARYWORLDZ] = world[renATTRZ];
    //world to eye
    mat441Multiply((double(*)[4])(&unif[renUNIFCAMERAVIEWING]), world, vary);

    vary[renVARYS] = attr[renATTRS];
    vary[renVARYT] = attr[renATTRT];
}

/* If unifParent is NULL, then sets the uniform matrix to the 
rotation-translation M described by the other uniforms. If unifParent is not 
NULL, but instead contains a rotation-translation P, then sets the uniform 
matrix to the matrix product P * M. */
void updateUniform(renRenderer *ren, double unif[], double unifParent[]) {
    //load viewing into UNIF
    for (int i = 0; i < 4; i+=1) {
        for (int j = 0; j<4; j+=1) {
            int pos = renUNIFCAMERAVIEWING + i*4 + j;
            // printf("%d, [%d, %d], %f\n", pos, i,j, ren->viewing[i][j]);
            unif[pos] = ren->viewing[i][j];
        }
    }
    double cameraWorldPos[3] = {0.0, 0.0, 0.0};

    setLight(unif, 0.75, 0.75, 0.5, 1.0, 1.0, 1.0);
    setCameraPos(unif, cameraWorldPos);

    double rotation33[3][3];
    vecUnit(3, &unif[renUNIFAXIS], &unif[renUNIFAXIS]);
    mat33AngleAxisRotation(unif[renUNIFTHETA], &unif[renUNIFAXIS], rotation33);
    double temp[16];

    if (unifParent == NULL) {
        /* The nine uniforms for storing the matrix start at index 
        renUNIFISOMETRY. So &unif[renUNIFISOMETRY] is an array containing those 
        nine numbers. We use '(double(*)[3])' to cast it to a 3x3 matrix. */
        mat44Isometry(rotation33, &unif[renUNIFTRANSX], (double(*)[4])(&unif[renUNIFISOMETRY]));
    } else {
        double m[4][4];
        mat44Isometry(rotation33, &unif[renUNIFTRANSX], m);
        mat444Multiply((double(*)[4])(&unifParent[renUNIFISOMETRY]), m, 
            (double(*)[4])(&unif[renUNIFISOMETRY]));
    }
}



#include "110triangle.c"
#include "140clipping.c"
#include "140mesh.c"
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

depthBuffer depth_z;

renRenderer renderer = {
    .unifDim = 52,
    .texNum = 1,
    .varyDim = 4+2+3+3+3,
    .attrDim = 3+2+3,
    .transformVertex = transformVertex,
    .colorPixel = colorPixel,
    .updateUniform = updateUniform,
    .depth = &depth_z,
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
    // .projection = {0, 512, 0, 512, 0, 512},
    .projectionType = 1,
    // .viewport = {
    //     {1, 0, 0, 0},
    //     {0, 1, 0, 0},
    //     {0, 0, 1, 0},
    //     {0, 0, 0, 1}
    // }
};

//move the camera
void handleKeyUp(int key, int shiftIsDown, int controlIsDown,
        int altOptionIsDown, int superCommandIsDown) {
    // W for up, S for down, A for right, D for Left like what it is set up
    // in most video games
    if (!shiftIsDown && !controlIsDown && !altOptionIsDown && !superCommandIsDown && key == W_KEY) {
        //change Camera Position
        renderer.cameraTranslation[0] += 2;
    }
    if (!shiftIsDown && !controlIsDown && !altOptionIsDown && !superCommandIsDown && key == S_KEY) {
        //change Camera Position
        renderer.cameraTranslation[0] -= 2;
    }
    if (!shiftIsDown && !controlIsDown && !altOptionIsDown && !superCommandIsDown && key == A_KEY) {
        //change Camera Position
        renderer.cameraTranslation[1] += 2;
    }
    if (!shiftIsDown && !controlIsDown && !altOptionIsDown && !superCommandIsDown && key == D_KEY) {
        //change Camera Position
        renderer.cameraTranslation[1] -= 2;
    }
    if (!shiftIsDown && !controlIsDown && !altOptionIsDown && !superCommandIsDown && key == One_KEY) {
        //change Camera Position
        renderer.cameraTranslation[2] += 2;
    }
    if (!shiftIsDown && !controlIsDown && !altOptionIsDown && !superCommandIsDown && key == TWO_KEY) {
        //change Camera Position
        renderer.cameraTranslation[2] -= 2;
    }
}


void draw(void){
    //draw scene from node A
    pixClearRGB(0.0, 0.0, 0.0);
    sceneRender(&nodeA, &renderer, NULL);
    
}

void handleTimeStep(double oldTime, double newTime) {
    depthClearZs(renderer.depth, -1);
    renUpdateViewing(&renderer);
    //redraw the scene by a new unifAnge theta as time changes
    if (floor(newTime) - floor(oldTime) >= 1.0)
        printf("handleTimeStep: %f frames/sec\n", 1.0 / (newTime - oldTime));
        //what to do to make it move
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

    depthBuffer *dp = &depth_z;

    //set camera
    double cameraPos[3] = {0, 0, 0};
    double objPos[3] =  {0, 0, 0};
    double cameraPhi = M_PI/3;
    double cameraTheta = M_PI/3;
    renLookAt(ren, objPos, 10, cameraPhi, cameraTheta);

    /*init unif for each scene node
    @first [0, 1, 2] background rgb
    @[3] angle theta
    @[4,5,6] translation vector, 
    @[7-9] rotation axis 
    @[10] isom of 4x4
    @[26] Camera isom
    @[42] light position xyz
    @[45] light rgb
    @[48] camera world position
    @[51] shineness (specular intensity between 0 and 128)
    */
    double unifA[3+1+3+3+16+16+6+3+1] = {
        1.0, 1.0, 1.0, 
        0.5, 0.0, 0.0, 0.0, 
        1.0, 1.0, 1.0, 
        1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
        1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
        1.0, 1.0, 1.0,
        1.0, 1.0, 1.0,
        0.0, 0.0, 0.0,
        0.3
    };
    double unifB[3+1+3+3+16+16+6+3+1] = {
        1.0, 1.0, 1.0, 
        0.5, 0.0, 0.0, 0.0, 
        0.0, 0.0, 0.0, 
        1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
        1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
        1.0, 1.0, 1.0,
        1.0, 1.0, 1.0,
        0.0, 0.0, 0.0,
        0.3
    };
    double unifC[3+1+3+3+16+16+6+3+1] = {
        1.0, 1.0, 1.0, 
        0.0, 0.0, 0.0, 0.0, 
        0.0, 0.0, 0.0, 
        1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
        1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
        1.0, 1.0, 1.0,
        1.0, 1.0, 1.0,
        0.0, 0.0, 0.0,
        0.3
    };
    double unifD[3+1+3+3+16+16+6+3+1] = {
        1.0, 1.0, 1.0, 
        0.0, 0.0, 0.0, 0.0, 
        0.0, 0.0, 0.0, 
        1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
        1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
        1.0, 1.0, 1.0,
        1.0, 1.0, 1.0,
        0.0, 0.0, 0.0,
        0.3
    }; 
    //init mesh, tex and scene nodes
    if (pixInitialize(512, 512, "Pixel Graphics") != 0) {
        return 1;
    } else if (depthInitialize(dp, 512, 512) != 0) {
        return 2;
    // } else if (meshInitializeRectangle(mesh1, 0, 400, 0, 400) != 0){
 //        return 2;
 //    } else if (meshInitializeEllipse(mesh2, 250.0, 200.0, 50.0, 50.0, 50.0) != 0){
 //        return 3;
 //    } else if (meshInitializeEllipse(mesh3, 150.0, 150.0, 60.0, 60.0, 50.0) != 0){
 //        return 4;
    // } else if (meshInitializeSphere(mesh1, 100, 40, 80) != 0){
    //     return 5;
    } else if (meshInitializeBox(mesh1, 0, 0.5, 0, 0.5, 0, 0.5) != 0){
        return 3;
    // } else if (meshInitializeBox(mesh2, 150.0, 200.0, 150.0, 200.0, 150.0, 200.0) != 0){
    //     return 4;
    //     //Why sphere can't be drawn
    // } else if (meshInitializeBox(mesh3, 200.0, 350.0, 200.0, 350.0, 200.0, 380.0) != 0) {
    //     return 4;
    } else if (meshInitializeSphere(mesh2, 0.5, 10, 20) != 0) {
        return 5;
    } else if (texInitializeFile(&texture, "avatar.jpg") != 0) {
        return 6;
    } else if (sceneInitialize(&nodeA, ren, unifA, tex, mesh1, &nodeB, NULL) != 0){
        return 7;
    } else if (sceneInitialize(&nodeB, ren, unifB, tex, mesh2, NULL, NULL) != 0){
        return 8;
    // } else if (sceneInitialize(&nodeC, ren, unifC, tex, mesh4, NULL, NULL) != 0){
    //     return 9;


    // Why can't render sphere in scene but its mesh init without trouble 
    // } else if (sceneInitialize(&nodeD, ren, unifD, tex, mesh4, NULL, NULL) != 0){
    //     return 10;
    }else {
        //draw the scene
        // draw();
        renSetFrustum(ren, renORTHOGRAPHIC, M_PI/6.0, 10.0, 10.0);
        pixSetKeyUpHandler(handleKeyUp);        
        pixSetTimeStepHandler(handleTimeStep);
        printf("Controls: W for moving the Camera Up\n");
        printf("Controls: S for moving the Camera Down\n");
        printf("Controls: A for moving the Camera Left\n");
        printf("Controls: D for moving the Camera Right\n");
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
        depthDestroy(&depth_z);
        return 0;
    }   
}