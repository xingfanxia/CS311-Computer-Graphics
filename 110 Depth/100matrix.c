//Xingfan Xia, January 21th
/*** 2 x 2 Matrices ***/

/* Pretty-prints the given matrix, with one line of text per row of matrix. */
void mat22Print(double m[2][2]) {
    for (int i = 0; i <2; i++){
        printf("%f  %f\n",m[i][0],m[i][1]);
    } 	
}

void mat33Print(double m[3][3]) {
    for (int i = 0; i < 3; i++) {
        printf("%f %f %f\n", m[i][0], m[i][1], m[i][2]);
    }
}

/* Returns the determinant of the matrix m. If the determinant is 0.0, then the 
matrix is not invertible, and mInv is untouched. If the determinant is not 0.0, 
then the matrix is invertible, and its inverse is placed into mInv. */
double mat22Invert(double m[2][2], double mInv[2][2]) {
    double det = m[0][0]*m[1][1] - m[0][1]*m[1][0];
    if (det != 0){
        mInv[0][0] = m[1][1]/det;
        mInv[0][1] = m[0][1]/((-1)*det);
        mInv[1][0] = m[1][0]/((-1)*det);
        mInv[1][1] = m[0][0]/det;
    }
    return det;	
}

/* Multiplies a 2x2 matrix m by a 2-column v, storing the result in mTimesV. 
The output should not */
void mat221Multiply(double m[2][2], double v[2], double mTimesV[2]) {
    mTimesV[0] = m[0][0]* v[0] + m[0][1] *v[1];
    mTimesV[1] = m[1][0]* v[0] + m[1][1] *v[1];
}

/* Fills the matrix m from its two columns. */
void mat22Columns(double col0[2], double col1[2], double m[2][2]) {
    m[0][0] = col0[0];
    m[0][1] = col1[0];
    m[1][0] = col0[1];
    m[1][1]	= col1[1];
}

/* Multiplies the 3x3 matrix m by the 3x3 matrix n. */
void mat333Multiply(double m[3][3], double n[3][3], double mTimesN[3][3]) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            double sum = 0;
            for (int k = 0; k < 3; k++) {
                sum += m[i][k] * n[k][j];
            }
            mTimesN[i][j] = sum;
        }
    }
}

void mat331Multiply(double m[3][3], double v[3], double mTimesV[3]) {
    for (int i = 0; i < 3; i++) {
        double sum = 0;
        for (int j = 0; j < 3; j++) {
            sum += m[i][j]*v[j];
        }
        mTimesV[i] = sum;
    }
}
/* Builds a 3x3 matrix representing 2D rotation and translation in homogeneous 
coordinates. More precisely, the transformation first rotates through the angle 
theta (in radians, counterclockwise), and then translates by the vector (x, y). 
*/
void mat33Isometry(double theta, double x, double y, double isom[3][3]) {
    // double rad = M_PI/180.0 * theta;
    double rotMat[3][3] = {
        cos(theta), -sin(theta), 0,
        sin(theta), cos(theta), 0,
        0, 0, 1,
    };

    double transMat[3][3] = {
        1, 0, x,
        0, 1, y,
        0, 0, 1,
    };
    mat333Multiply(transMat, rotMat, isom);
}

void mat33Add(double m[3][3], double v[3][3], double mAddV[3][3]){
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			mAddV[i][j] = m[i][j] + v[i][j];
		}
	}
}

void matMultScalar(double k, double m[3][3], double mMultK[3][3]) {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			mMultK[i][j] = m[i][j]*k;
		}
	}
}

void mat33Transpose(double m[3][3], double mTrans[3][3]) {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			mTrans[j][i] = m[i][j];
		}
	}
}

void mat44Transpose(double m[4][4], double mTrans[4][4]) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			mTrans[j][i] = m[i][j];
		}
	}
}
/* Given a length-1 3D vector axis and an angle theta (in radians), builds the 
rotation matrix for the rotation about that axis through that angle. Based on 
Rodrigues' rotation formula R = I + (sin theta) U + (1 - cos theta) U^2. */
void mat33AngleAxisRotation(double theta, double axis[3], double rot[3][3]) {
	double axis33[3][3] = {
		0, -axis[2], axis[1],
		axis[2], 0, -axis[0],
		-axis[1], axis[0], 0,
	};
	double axis33Square[3][3];
	double identity[3][3] = {
	  1,0,0,
	  0,1,0,
	  1,0,1,
	};
	double rAddPart1[3][3];
	double rAddPart2[3][3];
	mat333Multiply(axis33, axis33, axis33Square);
	matMultScalar(sin(theta), axis33, rAddPart1);
	matMultScalar(1-cos(theta), axis33Square, rAddPart2);
	mat33Add(rAddPart1, rAddPart2, rot);
	mat33Add(rot, identity, rot);
}

/* Given two length-1 3D vectors u, v that are perpendicular to each other. 
Given two length-1 3D vectors a, b that are perpendicular to each other. Builds 
the rotation matrix that rotates u to a and v to b. */
void mat33BasisRotation(double u[3], double v[3], double a[3], double b[3], 
        double rot[3][3]) {
	double w[3];
	vec3Cross(u, v, w);
	double R[3][3] = {
		u[0], v[0], w[0],
		u[1], v[1], w[1],
		u[2], v[2], w[2],
	};

	double ab[3];
	vec3Cross(a, b, ab);
	double S[3][3] = {
		a[0], b[0], ab[0],
		a[1], b[1], ab[1],
		a[2], b[2], ab[2],
	};
	double Rtrans[3][3];
	mat33Transpose(R, Rtrans);
	mat333Multiply(S, Rtrans, rot);
}

/* Multiplies m by n, placing the answer in mTimesN. */
void mat444Multiply(double m[4][4], double n[4][4], double mTimesN[4][4]) {
	for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            double sum = 0;
            for (int k = 0; k < 4; k++) {
                sum += m[i][k] * n[k][j];
            }
            mTimesN[i][j] = sum;
        }
    }
}

/* Multiplies m by v, placing the answer in mTimesV. */
void mat441Multiply(double m[4][4], double v[4], double mTimesV[4]) {
	for (int i = 0; i < 4; i++) {
        double sum = 0;
        for (int j = 0; j < 4; j++) {
            sum += m[i][j]*v[j];
        }
        mTimesV[i] = sum;
    }
}

/* Given a rotation and a translation, forms the 4x4 homogeneous matrix 
representing the rotation followed in time by the translation. */
void mat44Isometry(double rot[3][3], double trans[3], double isom[4][4]) {
	double trans44[4][4] = {
		1, 0, 0, trans[0],
		0, 1, 0, trans[1],
		0, 0, 1, trans[2],
		0, 0, 0, 1
	};

	double rot44[4][4] = {
		rot[0][0], rot[0][1], rot[0][2], 0,
		rot[1][0], rot[1][1], rot[1][2], 0,
		rot[2][0], rot[2][1], rot[2][2], 0,
		0, 0, 0, 1
	};

	mat444Multiply(trans44, rot44, isom);
}

/* Given a rotation and translation, forms the 4x4 homogeneous matrix 
representing the inverse translation followed in time by the inverse rotation. 
That is, the isom produced by this function is the inverse to the isom 
produced by mat44Isometry on the same inputs. */
void mat44InverseIsometry(double rot[3][3], double trans[3], 
        double isom[4][4]) {
	double trans44Inverse[4][4] = {
		1, 0, 0, -trans[0],
		0, 1, 0, -trans[1],
		0, 0, 1, -trans[2],
		0, 0, 0, 1
	};

	double rot44[4][4] = {
		rot[0][0], rot[0][1], rot[0][2], 0,
		rot[1][0], rot[1][1], rot[1][2], 0,
		rot[2][0], rot[2][1], rot[2][2], 0,
		0, 0, 0, 1
	};

	double rot44Transpose[4][4];
	mat44Transpose(rot44, rot44Transpose);
	mat444Multiply(rot44Transpose, trans44Inverse, isom);
}





