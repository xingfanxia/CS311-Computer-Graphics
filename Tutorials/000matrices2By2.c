


/* On macOS, compile with...
    clang 000matrices2By2.c
*/

#include <stdio.h>

/* You can think of a matrix as an array of arrays. Here's a function that 
takes a 2x2 matrix as input, and prints it. The syntax m[i][j] refers to the 
(i, j)th entry of the matrix m --- that is, the entry in row i and column j, 
both indexed from 0. */
void mat22Print(double m[2][2]) {
	int i;
	for (i = 0; i < 2; i += 1)
		printf("%f    %f\n", m[i][0], m[i][1]);
}

/* Because matrices are arrays, they have the same call-by-value-but-the-value-
is-actually-a-memory-location behavior as other C arrays. In particular, 
modifying a matrix within a function causes the matrix to be modified in the 
environment where the function was called, too. Here's a function that 
multiplies its input by 2.0, in place. */
void twice(double m[2][2]) {
	int i, j;
	for (i = 0; i < 2; i += 1)
		for (j = 0; j < 2; j += 1)
			m[i][j] *= 2.0;
}

/* Remember from 000arrays.c that you don't have to declare the length of the 
array in the interface to a function. For example, you can have a parameter 
'double a[]' instead of 'double a[2]'. Can you do the same with matrices, as in 
'double m[][]'? No, because C requires more information about how the memory 
for the matrix is laid out. Namely, it needs to know how many columns there 
are (but not how many rows there are). This makes it a little difficult to 
write functions that operate on general matrices. We will learn a workaround 
for that problem later in the course. Meanwhile, I recommend that you 
explicitly declare both the number of rows and the number of columns, unless 
you have a strong technical reason not to. */
void thrice(double m[][2]) {
	int i, j;
	for (i = 0; i < 2; i += 1)
		for (j = 0; j < 2; j += 1)
			m[i][j] *= 3.0;
}

/* This function exists only to help me make a point in the main function. */
void vec2Print(double v[2]) {
	printf("%f\n%f\n", v[0], v[1]);
}

int main(void) {
	/* You can initialize an array using syntax like this. */
	double myMatrix[2][2] = {
		{14.2, -3.0},
		{2.2, 1.6}};
	printf("myMatrix:\n");
	mat22Print(myMatrix);
	/* Notice that this function call modifies its argument. */
	twice(myMatrix);
	printf("myMatrix:\n");
	mat22Print(myMatrix);
	thrice(myMatrix);
	printf("myMatrix:\n");
	mat22Print(myMatrix);
	/* If m[0][0] and m[0][1] are the entries of the first row, does that mean 
	that m[0] is the first row, and it's an array, and we're just taking the 
	0th and 1th entries of that array? Yep. */
	printf("myMatrix[0]:\n");
	vec2Print(myMatrix[0]);
	printf("myMatrix[1]:\n");
	vec2Print(myMatrix[1]);
	/* By the way, does sizeof handle arrays and matrices well? Yep. */
	printf("sizeof(myMatrix) = %d, sizeof(myMatrix[0][0]) = %d.\n",
		(int)sizeof(myMatrix), (int)sizeof(myMatrix[0][0]));
	return 0;
}


