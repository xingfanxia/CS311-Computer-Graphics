//division then call TriRender
void clipRender(renRenderer *ren, double unif[], texTexture *tex[], double a[], 
        double b[], double c[]) {
	

	double viewA[4] = {a[renATTRX], a[renATTRY], a[renATTRZ],a[renVARYW]};
	double viewB[4] = {b[renATTRX], b[renATTRY], b[renATTRZ],b[renVARYW]};
	double viewC[4] = {c[renATTRX], c[renATTRY], c[renATTRZ],c[renVARYW]};

	double vectorA[renVARYDIMBOUND];
	double vectorB[renVARYDIMBOUND];
	double vectorC[renVARYDIMBOUND];

	vecScale(4, 1/a[renVARYW], a, a);
	vecScale(4, 1/b[renVARYW], b, b);
	vecScale(4, 1/c[renVARYW], c, c);
    
    vecScale(4, 1/viewA[renVARYW], viewA, viewA);
	vecScale(4, 1/viewB[renVARYW], viewB, viewB);
	vecScale(4, 1/viewC[renVARYW], viewC, viewC);

    mat441Multiply(ren->viewport, viewA, viewA);
    mat441Multiply(ren->viewport, viewB, viewB);
    mat441Multiply(ren->viewport, viewC, viewC);

    vectorA[renVARYX] = viewA[renVARYX];
    vectorA[renVARYY] = viewA[renVARYY];
    vectorA[renVARYZ] = viewA[renVARYZ];
    vectorA[renVARYW] = viewA[renVARYW];
    vectorA[renVARYS] = a[renVARYS];
    vectorA[renVARYT] = a[renVARYT];


    vectorB[renVARYX] = viewB[renVARYX];
    vectorB[renVARYY] = viewB[renVARYY];
    vectorB[renVARYZ] = viewB[renVARYZ];
    vectorB[renVARYW] = viewB[renVARYW];
    vectorB[renVARYS] = b[renVARYS];
    vectorB[renVARYT] = b[renVARYT];

    vectorC[renVARYX] = viewC[renVARYX];
    vectorC[renVARYY] = viewC[renVARYY];
    vectorC[renVARYZ] = viewC[renVARYZ];
    vectorC[renVARYW] = viewC[renVARYW];
    vectorC[renVARYS] = c[renVARYS];
    vectorC[renVARYT] = c[renVARYT];

    printf("a, %f, %f, %f, %f, %f, %f\n", vectorA[0],vectorA[1],vectorA[2], vectorA[3], vectorA[4], vectorA[5]);
    triRender(ren, unif, tex, vectorA, vectorB, vectorC);
}