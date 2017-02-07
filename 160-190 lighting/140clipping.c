//Xingfan Xia Feb 3rd, get help from Yanhan Lyu for the structure of cases
//vecScale by renVaryDim and view
void scale(renRenderer *ren, double vector[renVARYDIMBOUND], double out[renVARYDIMBOUND]) {
	double view[4] = {vector[renVARYX], vector[renVARYY], vector[renVARYZ], vector[renVARYW]};
	vecScale(4, 1/view[renVARYW], view, view);
	mat441Multiply(ren->viewport, view, view);
	vecCopy(renVARYDIMBOUND, vector, out);
	out[renVARYX] = view[renVARYX];
	out[renVARYY] = view[renVARYY];
	out[renVARYZ] = view[renVARYZ];
	out[renVARYW] = view[renVARYW];
	out[renVARYS] = vector[renVARYS];
	out[renVARYT] = vector[renVARYT];
}

//calculate T value from clipped vertexes
void calculateInterpolatedT(double start[renVARYDIMBOUND], double end[renVARYDIMBOUND], double result[renVARYDIMBOUND]) {
	double t = (start[3] - start[2])/(start[3] - start[2] - end[3] + end[2]);
	double end_to_start[renVARYDIMBOUND];
	vecSubtract(renVARYDIMBOUND, end, start, end_to_start);
	vecScale(renVARYDIMBOUND, t, end_to_start, end_to_start);
	vecAdd(renVARYDIMBOUND, start, end_to_start, result);
}

void clipRender(renRenderer *ren, double unif[], texTexture *tex[], double a[], 
        double b[], double c[]) {
	

	double tempVecA[renVARYDIMBOUND];
	double tempVecB[renVARYDIMBOUND];
	double tempVecC[renVARYDIMBOUND];

	vecCopy(renVARYDIMBOUND, a, tempVecA);
	vecCopy(renVARYDIMBOUND, b, tempVecB);
	vecCopy(renVARYDIMBOUND, c, tempVecC);

	// int clipFlag = 0;
	// int VertexAclipped = 0;
	// int VertexBclipped = 0;
	// int VertexCclipped = 0;

	// if (viewA[3] <= 0 || viewA[2] > viewA[3]) {
	// 	clipFlag += 1;
	// 	VertexAclipped += 1;
	// } else if (viewB[3] <= 0 || viewB[2] > viewB[3]) {
	// 	clipFlag += 1;
	// 	VertexBclipped += 1;
	// } else if (viewC[3] <= 0 || viewC[2] > viewC[3]) {
	// 	clipFlag += 1;
	// 	VertexCclipped += 1;
	// }

	// if (clipFlag == 3) {
	// 	return;
	// } else if (clipFlag == 2) {

	// } else if (clipFlag == 1) {

	// }

    double one[renVARYDIMBOUND];
    double two[renVARYDIMBOUND];
    double ClippedOne[renVARYDIMBOUND];
    double ClippedTwo[renVARYDIMBOUND];
    double newA[renVARYDIMBOUND];
    double newB[renVARYDIMBOUND];
    double newC[renVARYDIMBOUND];

    if ((tempVecA[3] <= 0 || tempVecA[2]>tempVecA[3]) && 
        !(tempVecB[3] <= 0 || tempVecB[2]>tempVecB[3]) && !(tempVecC[3] <= 0 || tempVecC[2]>tempVecC[3])){
        calculateInterpolatedT(tempVecA, tempVecB, one);
        calculateInterpolatedT(tempVecA, tempVecB, two);

        scale(ren, one, ClippedOne);
        scale(ren, two, ClippedTwo);
        scale(ren, tempVecB, newB);
        scale(ren, tempVecC, newC);

        triRender(ren, unif, tex, ClippedOne, newB, ClippedTwo);
        triRender(ren, unif, tex, ClippedTwo, newB, newC);
    } else if ((tempVecB[3] <= 0 || tempVecB[2]>tempVecB[3]) && 
        !(tempVecA[3] <= 0 || tempVecA[2]>tempVecA[3]) && !(tempVecC[3] <= 0 || tempVecC[2]>tempVecC[3])){
        calculateInterpolatedT(tempVecA,tempVecB,one);
        calculateInterpolatedT(tempVecB,tempVecC,two);

        scale(ren, one, ClippedOne);
        scale(ren, two, ClippedTwo);
        scale(ren, tempVecA, newA);
        scale(ren, tempVecC, newC);

        triRender(ren, unif, tex, newA, ClippedOne, newC);
        triRender(ren, unif, tex, ClippedOne, ClippedTwo, newC);
    } else if ((tempVecC[3] <= 0 || tempVecC[2]>tempVecC[3]) && 
        !(tempVecA[3] <= 0 || tempVecA[2]>tempVecA[3]) && !(tempVecB[3] <= 0 || tempVecB[2]>tempVecB[3])){
        calculateInterpolatedT(tempVecB, tempVecC, one);
        calculateInterpolatedT(tempVecA, tempVecC, two);

        scale(ren, one, ClippedOne);
        scale(ren, two, ClippedTwo);
        scale(ren, tempVecA, newA);
        scale(ren, tempVecB, newB);

        triRender(ren, unif, tex, newB, ClippedOne, ClippedTwo);
        triRender(ren, unif, tex, newB, ClippedTwo, newA);
    }else if ((tempVecA[3] <= 0 || tempVecA[2]>tempVecA[3]) && (tempVecB[3] <= 0 || tempVecB[2]>tempVecB[3]) 
        && !(tempVecC[3] <= 0 || tempVecC[2]>tempVecC[3])){
        calculateInterpolatedT(tempVecA, tempVecC, one);
        calculateInterpolatedT(tempVecB, tempVecC, two);

        scale(ren, one, ClippedOne);
        scale(ren, two, ClippedTwo);
        scale(ren, tempVecC, newC);

        triRender(ren, unif, tex, ClippedOne, ClippedTwo, newC);
    }else if ((tempVecA[3] <= 0 || tempVecA[2]>tempVecA[3]) && (tempVecC[3] <= 0 || tempVecC[2]>tempVecC[3]) 
        && !(tempVecB[3] <= 0 || tempVecB[2]>tempVecB[3])){
        calculateInterpolatedT(tempVecA,tempVecB,one);
        calculateInterpolatedT(tempVecB,tempVecC,two);

        scale(ren, one, ClippedOne);
        scale(ren, two, ClippedTwo);
        scale(ren, tempVecB, newB);

        triRender(ren, unif, tex, ClippedOne, newB, ClippedTwo);
    } else if ((tempVecB[3] <= 0 || tempVecB[2]>tempVecB[3]) && (tempVecC[3] <= 0 || tempVecC[2]>tempVecC[3]) 
        && !(tempVecA[3] <= 0 || tempVecA[2]>tempVecA[3])){
        calculateInterpolatedT(tempVecA,tempVecB,one);
        calculateInterpolatedT(tempVecA,tempVecC,two);

        scale(ren, one, ClippedOne);
        scale(ren, two, ClippedTwo);
        scale(ren, tempVecA, newA);

        triRender(ren, unif, tex, ClippedTwo, newA, ClippedOne);
    } else if (!(tempVecB[3] <= 0 || tempVecB[2]>tempVecB[3]) && !(tempVecC[3] <= 0 || tempVecC[2]>tempVecC[3]) 
        && !(tempVecA[3] <= 0 || tempVecA[2]>tempVecA[3])){
        scale(ren, tempVecA, newA);
        scale(ren, tempVecB, newB);
        scale(ren, tempVecC, newC);
        triRender(ren, unif, tex, newA, newB, newC);
    }
} 


