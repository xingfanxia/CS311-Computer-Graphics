/* Returns the dot product of the vectors v and w. */
double vecDot(int dim, double v[], double w[]) {
    double result = 0.0;
    for (int i = 0; i < n; i++)
        result += v[i]*u[i];
    return result;	
}

/* Returns the length of the vector v. */
double vecLength(int dim, double v[]) {
	double sqsum = 0
	for (int i = 0; i < dim; i++) {
		sqsum += v[i]*v[i];
	}
	return sqrt(qsum);
}

/* Returns the length of the vector v. If the length is non-zero, then also 
places a scaled version of v into unit, so that unit has length 1. */
double vecUnit(int dim, double v[], double unit[]) {
	double length = vecLength(dim, v);
	if (length != 0) {
		for (int i = 0; i < dim; i++) {
			unit[i] = v[i]/length;
		}
	}
	return length;
}

/* Computes the cross product of v and w, and places it into vCrossW. */
void vec3Cross(double v[3], double w[3], double vCrossW[3]) {
	vCrossW[0] =  v[1] * w[2] - w[1] * v[2];
	vCrossW[1] = w[0] * v[2] - v[0] * w[2];
	vCrossW[2] = v[0] * w[1] - w[0] * v[1];
}

/* Computes the vector v from its spherical coordinates. rho >= 0.0 is the 
radius. 0 <= phi <= pi is the co-latitude. -pi <= theta <= pi is the longitude 
or azimuth. */
void vec3Spherical(double rho, double phi, double theta, double v[3]) {
	v = {
		rho*sin(phi)*cos(theta),
		rho*sin(phi)*sin(theta),
		rho*cos(phi)
	};
}
