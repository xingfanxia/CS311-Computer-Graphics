//Xingfan Xia, January 20th
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


// int main(void) {
//     double m33[3][3]= {
//         3, 2, 3,
//         3, 1, 5,
//         5, 1, 2,
//     };

//     double n33[3][3] = {
//         1, 5, 7,
//         7, 2, 4,
//         8, 6, 3,
//     };

//     double n31[3] = {2, 3, 9};

//     double result333[3][3];
//     double result331[3];
//     mat333Multiply(m33, n33, result333);
//     mat331Multiply(m33, n31, result331);

//     mat33Print(result333);
//     printf("%f %f %f\n", result331[0], result331[1], result331[2]);  

// }


