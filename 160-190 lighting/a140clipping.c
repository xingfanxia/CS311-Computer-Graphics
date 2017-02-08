// check if a vertex is clipped
int isClip(double vary[]) {
    return vary[renVARYZ] > vary[renVARYW] || vary[renVARYW] <= 0;
}

// get the intersection of p1 and p2, suppose one of them is clipped
void getIntersection(renRenderer *ren, double p1[], double p2[], double intersection[]) {
    double t = (p1[renVARYZ] - p1[renVARYY]) / (p1[renVARYZ] - p1[renVARYY] + p2[renVARYY] - p2[renVARYZ]);
    vecSubtract(ren->varyDim, p2, p1, intersection);
    vecScale(ren->varyDim, t, intersection, intersection);
    vecAdd(ren->varyDim, p1, intersection, intersection);
}

// Saves whether ab, bc and ca intersects into isIntersect[]. Save the intersections into intersections[].
// Returns the number of intersections
int clipTriangle(renRenderer *ren, double aVary[], double bVary[], double cVary[], int clipV[3], double intersections[]) {
    int noc = 0;
    int AB = 0;
    int BC = ren->varyDim;
    int AC = ren->varyDim * 2;
    int A = 0;
    int B = 1;
    int C = 2;
    if (isClip(aVary)) {
        noc++;
        clipV[A] = 1;
        if (isClip(bVary)) {
            noc++;
            clipV[B] = 1;
            if (isClip(cVary)) {
                // abc
                noc++;
                clipV[C] = 1;
            } else {
                // ab
                getIntersection(ren, aVary, cVary, &intersections[AC]);
                getIntersection(ren, bVary, cVary, &intersections[BC]);
            }
        } else {
            if (isClip(cVary)) {
                // ac
                noc++;
                clipV[C] = 1;
                getIntersection(ren, aVary, bVary, &intersections[AB]);
                getIntersection(ren, bVary, cVary, &intersections[BC]);
            } else {
                // a
                getIntersection(ren, aVary, cVary, &intersections[AC]);
                getIntersection(ren, bVary, aVary, &intersections[AB]);
            }
        }
    } else {
        if (isClip(bVary)) {
            noc++;
            clipV[B] = 1;
            if (isClip(cVary)) {
                // bc
                noc++;
                clipV[C] = 1;
                getIntersection(ren, aVary, cVary, &intersections[AC]);
                getIntersection(ren, bVary, aVary, &intersections[AB]);
            } else {
                // b
                getIntersection(ren, aVary, bVary, &intersections[AB]);
                getIntersection(ren, bVary, cVary, &intersections[BC]);
            }
        } else {
            if (isClip(cVary)) {
                // c
                noc++;
                clipV[C] = 1;
                getIntersection(ren, aVary, cVary, &intersections[AC]);
                getIntersection(ren, bVary, cVary, &intersections[BC]);
            } else {}
        }
    }
    return noc;
}

// suppose none of the points are clipped, do the division and save the results to a b and c
void division(renRenderer *ren, double aVary[], double bVary[], double cVary[], double a[], double b[], double c[]) {
    a[renVARYX] = aVary[renVARYX] / aVary[renVARYW];
    a[renVARYY] = aVary[renVARYY] / aVary[renVARYW];
    a[renVARYZ] = aVary[renVARYZ] / aVary[renVARYW];
    a[renVARYW] = 1.0;
    mat441Multiply(ren->viewport, a, a);
    b[renVARYX] = bVary[renVARYX] / bVary[renVARYW];
    b[renVARYY] = bVary[renVARYY] / bVary[renVARYW];
    b[renVARYZ] = bVary[renVARYZ] / bVary[renVARYW];
    b[renVARYW] = 1.0;
    mat441Multiply(ren->viewport, b, b);
    c[renVARYX] = cVary[renVARYX] / cVary[renVARYW];
    c[renVARYY] = cVary[renVARYY] / cVary[renVARYW];
    c[renVARYZ] = cVary[renVARYZ] / cVary[renVARYW];
    c[renVARYW] = 1.0;
    mat441Multiply(ren->viewport, c, c);
}

void clipRender(renRenderer *ren, double unif[], texTexture *tex[], double aVary[], double bVary[], double cVary[]) {
    double a[ren->varyDim];
    double b[ren->varyDim];
    double c[ren->varyDim];
    vecCopy(ren->varyDim, aVary, a);
    vecCopy(ren->varyDim, bVary, b);
    vecCopy(ren->varyDim, cVary, c);
    // get clipping information
    int clipV[3] = {0, 0, 0}; // a, b, c
    double intersections[3 * ren->varyDim];
    int noc = clipTriangle(ren, aVary, bVary, cVary, clipV, intersections);
    // index to access clipping information
    int AB = 0;
    int BC = ren->varyDim;
    int AC = ren->varyDim * 2;
    int A = 0;
    int B = 1;
    int C = 2;
    switch (noc) {
        case 0:
            division(ren, aVary, bVary, cVary, a, b, c);
            triRender(ren, unif, tex, a, b, c);
            break;
        case 1:
            if (clipV[A]) {
                division(ren, &intersections[AC], bVary, cVary, a, b, c);
                triRender(ren, unif, tex, a, b, c);
                division(ren, &intersections[AB], &intersections[AC], bVary, a, b, c);
                triRender(ren, unif, tex, a, b, c);
            } else if (clipV[B]) {
                division(ren, &intersections[AB], aVary, cVary, a, b, c);
                triRender(ren, unif, tex, a, b, c);
                division(ren, &intersections[AB], &intersections[BC], cVary, a, b, c);
                triRender(ren, unif, tex, a, b, c);
            } else if (clipV[C]) {
                division(ren, &intersections[AC], bVary, aVary, a, b, c);
                triRender(ren, unif, tex, a, b, c);
                division(ren, &intersections[BC], &intersections[AC], bVary, a, b, c);
                triRender(ren, unif, tex, a, b, c);
            }
            break;
        case 2:
            if (!clipV[A]) {
                division(ren, &intersections[AB], &intersections[AC], aVary, a, b, c);
                triRender(ren, unif, tex, a, b, c);
            } else if (!clipV[B]) {
                division(ren, &intersections[AB], &intersections[BC], bVary, a, b, c);
                triRender(ren, unif, tex, a, b, c);
            } else if (!clipV[C]) {
                division(ren, &intersections[BC], &intersections[AC], cVary, a, b, c);
                triRender(ren, unif, tex, a, b, c);
            }
            break;
        case 3:
            break;
    }
}