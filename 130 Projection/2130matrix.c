/*Author: Yanhan Lyu*/

/*** 2 x 2 Matrices ***/

/* Pretty-prints the given matrix, with one line of text per row of matrix. */


/* Returns the determinant of the matrix m. If the determinant is 0.0, then the 
matrix is not invertible, and mInv is untouched. If the determinant is not 0.0, 
then the matrix is invertible, and its inverse is placed into mInv. */
double mat22Invert(double m[2][2], double mInv[2][2]) {
	double det = m[0][0]*m[1][1] - m[0][1]*m[1][0];
	if (det != 0){
		double scalar = 1/det;
		mInv[0][0] = scalar*m[1][1];
		mInv[0][1] = -scalar*m[0][1];
		mInv[1][0] = -scalar*m[1][0];
		mInv[1][1] = scalar*m[0][0];
	}
	return det;
}

/* Multiplies a 2x2 matrix m by a 2-column v, storing the result in mTimesV. 
The output should not */
void mat221Multiply(double m[2][2], double v[2], double mTimesV[2]) {
	mTimesV[0] = m[0][0]*v[0]+m[0][1]*v[1];
	mTimesV[1] = m[1][0]*v[0]+m[1][1]*v[1];
}

/* Fills the matrix m from its two columns. */
void mat22Columns(double col0[2], double col1[2], double m[2][2]) {
	m[0][0] = col0[0];
	m[1][0] = col0[1];
	m[0][1] = col1[0];
	m[1][1] = col1[1];
}

/* Multiplies the 3x3 matrix m by the 3x3 matrix n. */
void mat333Multiply(double m[3][3], double n[3][3], double mTimesN[3][3]){
	mTimesN[0][0] = m[0][0] * n[0][0] + m[0][1]*n[1][0] + m[0][2]*n[2][0];
	mTimesN[0][1] = m[0][0] * n[0][1] + m[0][1]*n[1][1] + m[0][2]*n[2][1];
	mTimesN[0][2] = m[0][0] * n[0][2] + m[0][1]*n[1][2] + m[0][2]*n[2][2];

	mTimesN[1][0] = m[1][0] * n[0][0] + m[1][1]*n[1][0] + m[1][2]*n[2][0];
	mTimesN[1][1] = m[1][0] * n[0][1] + m[1][1]*n[1][1] + m[1][2]*n[2][1];
	mTimesN[1][2] = m[1][0] * n[0][2] + m[1][1]*n[1][2] + m[1][2]*n[2][2];

	mTimesN[2][0] = m[2][0] * n[0][0] + m[2][1]*n[1][0] + m[2][2]*n[2][0];
	mTimesN[2][1] = m[2][0] * n[0][1] + m[2][1]*n[1][1] + m[2][2]*n[2][1];
	mTimesN[2][2] = m[2][0] * n[0][2] + m[2][1]*n[1][2] + m[2][2]*n[2][2];
}

/* Multiplies the 3x3 matrix m by the 3x1 matrix v. */
void mat331Multiply(double m[3][3], double v[3], double mTimesV[3]){
	mTimesV[0] = m[0][0] * v[0] + m[0][1]*v[1]+ m[0][2]*v[2];
	mTimesV[1] = m[1][0] * v[0] + m[1][1]*v[1] + m[1][2]*v[2];
	mTimesV[2] = m[2][0] * v[0] + m[2][1]*v[1] + m[2][2]*v[2];
}

/* Builds a 3x3 matrix representing 2D rotation and translation in homogeneous 
coordinates. More precisely, the transformation first rotates through the angle 
theta (in radians, counterclockwise), and then translates by the vector (x, y). 
*/
void mat33Isometry(double theta, double x, double y, double isom[3][3]){
	double r[3][3] = {
        {cos(theta), -sin(theta), 0},
        {sin(theta), cos(theta), 0},
        {0, 0, 1},
    };
    double t[3][3] = {
        {1, 0, x},
        {0, 1, y},
        {0, 0, 1},
    };
	mat333Multiply(t,r, isom);
}
/*3*3 matrix addition*/
void mat33Add(double u[3][3], double v[3][3], double uPlusV[3][3]){
	for (int i = 0; i<3; i+=1){
		for (int j=0; j<3; j+=1){
			uPlusV[i][j] = u[i][j] + v[i][j];
		} 	
	}
}
/*3*3 matrix scale*/
void matrixScale(double s, double m[3][3], double sTimesM[3][3]) {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			sTimesM[i][j] = m[i][j]*s;
		}
	}
}

/* Given a length-1 3D vector axis and an angle theta (in radians), builds the 
rotation matrix for the rotation about that axis through that angle. Based on 
Rodrigues' rotation formula R = I + (sin theta) U + (1 - cos theta) U^2. */
void mat33AngleAxisRotation(double theta, double axis[3], double rot[3][3]){
	double axisU[3][3] = {{0,-axis[2],axis[1]},
						{axis[2], 0, -axis[0]},
						{-axis[1], axis[0], 0}};
	double identity[3][3] = {{1,0,0},
							 {0,1,0}, 
							 {0,0,1}};
	double axisUSquare[3][3];
	mat333Multiply(axisU, axisU, axisUSquare);
	double sinthetaU[3][3];
	matrixScale(sin(theta), axisU, sinthetaU);
	double oneMinusCosthetaUSquare[3][3];
	matrixScale(1-cos(theta), axisUSquare, oneMinusCosthetaUSquare);
	double addition[3][3];
	mat33Add(sinthetaU, oneMinusCosthetaUSquare, addition);
	mat33Add(addition, identity, rot);
}




/*calculate the transpose of a 3*3 matrix u*/
void mat33Transpose(double u[3][3], double transposeU[3][3]){
	for (int i = 0; i<3; i+=1){
		for (int j=0; j<3; j+=1){
			transposeU[j][i] = u[i][j];
		} 	
	}
}

/* Given two length-1 3D vectors u, v that are perpendicular to each other. 
Given two length-1 3D vectors a, b that are perpendicular to each other. Builds 
the rotation matrix that rotates u to a and v to b. */
void mat33BasisRotation(double u[3], double v[3], double a[3], double b[3], 
        double rot[3][3]){
	double uVCross[3];
	vec3Cross(u, v, uVCross);
	double r[3][3] = {
		{u[0], v[0], uVCross[0]},
		{u[1], v[1], uVCross[1]},
		{u[2], v[2], uVCross[2]},
	};

	double aBCross[3];
	vec3Cross(a, b, aBCross);
	double s[3][3] = {
		{a[0], b[0], aBCross[0]},
		{a[1], b[1], aBCross[1]},
		{a[2], b[2], aBCross[2]},
	};
	double rT[3][3];
	mat33Transpose(r, rT);
	mat333Multiply(s, rT, rot);
}

/* Multiplies m by n, placing the answer in mTimesN. */
void mat444Multiply(double m[4][4], double n[4][4], double mTimesN[4][4]){
	for (int i = 0; i < 4; i+=1) {
		for (int j = 0; j < 4; j+=1) {
			mTimesN[i][j] = m[i][0]*n[0][j] + m[i][1]*n[1][j] + m[i][2]*n[2][j] + m[i][3]*n[3][j];
		}
	}
}

/* Multiplies m by v, placing the answer in mTimesV. */
void mat441Multiply(double m[4][4], double v[4], double mTimesV[4]){
	for (int i = 0; i < 4; i+=1) {
		mTimesV[i] = m[i][0]*v[0] + m[i][1]*v[1] + m[i][2]*v[2] + m[i][3]*v[3];
	}

}

/* Given a rotation and a translation, forms the 4x4 homogeneous matrix 
representing the rotation followed in time by the translation. */
void mat44Isometry(double rot[3][3], double trans[3], double isom[4][4]){
	double rotNew[4][4] = {
		{rot[0][0], rot[0][1], rot[0][2], 0},
		{rot[1][0], rot[1][1], rot[1][2], 0},
		{rot[2][0], rot[2][1], rot[2][2], 0},
		{0,          0,         0,        1}
	};
	double transNew[4][4] = {
		{1, 0, 0, trans[0]},
		{0, 1, 0, trans[1]},
		{0, 0, 1, trans[2]},
		{0, 0, 0, 1}
	};
	mat444Multiply(transNew, rotNew, isom);
}

/*calculate the transpose of a 4*4 matrix m*/
void mat44Transpose(double m[4][4], double t[4][4]){
	for (int i = 0; i<4; i+=1){
		for (int j=0; j<4; j+=1){
			t[j][i] = m[i][j];
		} 	
	}

}

/*pack the matrix into the array*/
void packArray(double v[4][4], double unif[]){
	unif[0] = v[0][0];
	unif[1] = v[0][1];
	unif[2] = v[0][2];
	unif[3] = v[0][3];

	unif[4] = v[1][0];
	unif[5] = v[1][1];
	unif[6] = v[1][2];
	unif[7] = v[1][3];

	unif[8] = v[2][0];
	unif[9] = v[2][1];
	unif[10] = v[2][2];
	unif[11] = v[2][3];

	unif[12] = v[3][0];
	unif[13] = v[3][1];
	unif[14] = v[3][2];
	unif[15] = v[3][3];
}

/* Given a rotation and translation, forms the 4x4 homogeneous matrix 
representing the inverse translation followed in time by the inverse rotation. 
That is, the isom produced by this function is the inverse to the isom 
produced by mat44Isometry on the same inputs. */
void mat44InverseIsometry(double rot[3][3], double trans[3], 
        double isom[4][4]){
	double rotTranspose[4][4];
	double transInverse[4][4] = {
		{1, 0, 0, -trans[0]},
		{0, 1, 0, -trans[1]},
		{0, 0, 1, -trans[2]},
		{0, 0, 0, 1}
	};
	double rotNew[4][4] = {
		{rot[0][0], rot[0][1], rot[0][2], 0},
		{rot[1][0], rot[1][1], rot[1][2], 0},
		{rot[2][0], rot[2][1], rot[2][2], 0},
		{0, 0, 0, 1}
	};
	mat44Transpose(rotNew, rotTranspose);
	mat444Multiply(rotTranspose, transInverse, isom);
}

/* Builds a 4x4 matrix representing orthographic projection with a boxy viewing 
volume [left, right] x [bottom, top] x [far, near]. That is, on the near plane 
the box is the rectangle R = [left, right] x [bottom, top], and on the far 
plane the box is the same rectangle R. Keep in mind that 0 > near > far. Maps 
the viewing volume to [-1, 1] x [-1, 1] x [-1, 1]. */
void mat44Orthographic(double left, double right, double bottom, double top, 
        double far, double near, double proj[4][4]) {
	proj[0][0] = 2/(right-left);
	proj[0][1] = 0;
	proj[0][2] = 0;
	proj[0][3] = (-right-left)/(right-left);
	proj[1][0] = 0;
	proj[1][1] = 2/(top-bottom);
	proj[1][2] = 0;
	proj[1][3] = (-top-bottom)/(top-bottom);
    proj[2][0] = 0;
    proj[2][1] = 0;
    proj[2][2] = 2/(near-far);
    proj[2][3] = (-near-far)/(near-far);
	proj[3][0] = 0;
	proj[3][1] = 0;
	proj[3][2] = 0;
	proj[3][3] = 1;
}

/* Builds a 4x4 matrix that maps a projected viewing volume 
[-1, 1] x [-1, 1] x [-1, 1] to screen [0, w - 1] x [0, h - 1] x [-1, 1]. */
void mat44Viewport(double width, double height, double view[4][4]) {
    view[0][0] = (width-1)/2;
	view[0][1] = 0;
	view[0][2] = 0;
	view[0][3] = (width-1)/2;
	view[1][0] = 0;
	view[1][1] = (height-1)/2;
	view[1][2] = 0;
	view[1][3] = (height-1)/2;
    view[2][0] = 0;
    view[2][1] = 0;
    view[2][2] = 1;
    view[2][3] = 0;
	view[3][0] = 0;
	view[3][1] = 0;
	view[3][2] = 0;
	view[3][3] = 1;
}

/* Builds a 4x4 matrix representing perspective projection. The viewing frustum 
is contained between the near and far planes, with 0 > near > far. On the near 
plane, the frustum is the rectangle R = [left, right] x [bottom, top]. On the 
far plane, the frustum is the rectangle (far / near) * R. Maps the viewing 
volume to [-1, 1] x [-1, 1] x [-1, 1]. */
void mat44Perspective(double left, double right, double bottom, double top, 
        double far, double near, double proj[4][4]) {
    proj[0][0] = (-2*near)/(right-left);
	proj[0][1] = 0;
	proj[0][2] = (right+left)/(right-left);
	proj[0][3] = 0;
	proj[1][0] = 0;
	proj[1][1] = (-2*near)/(top-bottom);
	proj[1][2] = (top+bottom)/(top-bottom);
	proj[1][3] = 0;
    proj[2][0] = 0;
    proj[2][1] = 0;
    proj[2][2] = (-near-far)/(near-far);
    proj[2][3] = (2*near*far)/(near-far);
	proj[3][0] = 0;
	proj[3][1] = 0;
	proj[3][2] = -1;
	proj[3][3] = 0;
}