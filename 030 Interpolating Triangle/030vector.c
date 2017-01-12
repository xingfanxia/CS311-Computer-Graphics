/*** In general dimensions ***/

/* Copies the dim-dimensional vector v to the dim-dimensional vector copy. */
void vecCopy(int dim, double v[], double copy[]) {
    for (int i = 0; i < dim; i++){
        copy[i] = v[i];
    }	
}

/* Adds the dim-dimensional vectors v and w. */
void vecAdd(int dim, double v[], double w[], double vPlusW[]) {
    for (int i = 0; i < dim; i++){
    vPlusW[i] = v[i] + w[i];
    }	
}

/* Subtracts the dim-dimensional vectors v and w. */
void vecSubtract(int dim, double v[], double w[], double vMinusW[]) {
	for (int i = 0; i < dim; i++){
    vMinusW[i] = v[i] - w[i];
    }	
}

/* Scales the dim-dimensional vector w by the number c. */
void vecScale(int dim, double c, double w[], double cTimesW[]) {
	for (int i = 0; i < dim; i++){
    cTimesW[i] = c *w[i];
    }	
}

// void vectorPp(double v[3]) {
// 	for (int i = 0; i < 3; i++) {
// 		printf("%f\n", v[i]);
// 	}
// }
