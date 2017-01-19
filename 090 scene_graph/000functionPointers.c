


/* On macOS, compile with...
    clang 000functionPointers.c
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* Functions in C are not 'first class'. You cannot make a function at run 
time. However, you can pass functions as arguments to other functions. And you 
can return functions from functions. This feature gives helps us write programs 
that reconfigure their behavior at run time, if only a little. */

/* Here's a function that inputs an int and a double and returns nothing. I'm 
not interested in what the function does. I'm just interested in its input and 
output types. */
void printSum(int n, double x) {
	printf("printSum says %f.\n", n + x);
}

/* Here's another function of exactly the same input and output types. */
void printDifference(int n, double x) {
	printf("printDifference says %f.\n", n - x);
}

/* Here's a function that takes two inputs and returns nothing. The first 
input is an int. The second input is a function f, that takes an int and a 
double and returns nothing. */
void applyIntFunction(int n, void (*f)(int, double)) {
	printf("applyIntFunction is doing its thing now...\n");
	/* This is how you apply a function pointer to some arguments. Simple. */
	f(n, 13.0);
}

/* Here's a function that takes as input a double array, and returns as output 
a pointer-to-double. */
double *arrayElementOne(double array[]) {
	return &array[1];
}

/* And here's another function, of exactly the same types. */
double *arrayElementTwo(double array[]) {
	return &array[2];
}

int main(void) {
	/* Seed a random number generator with the current time. By the way, this 
	generator is not good enough for serious cryptographic or statistical 
	applications. */
	time_t t;
	srand((unsigned)time(&t));
	/* Print some pointless arithmetic, based on the random number stream. */
	int i, r;
	for (i = 0; i < 10; i += 1) {
		r = rand();
		printf("r is %d, and then ", r);
		if (r % 2 == 0)
			applyIntFunction(r, printSum);
		else
			applyIntFunction(r, printDifference);
	}
	/* This variable is a pointer-to-function, where the function takes as 
	input a double array and returns as output a pointer-to-double. We 
	initialize it to the address of the function arrayElementTwo. */
	double* (*func)(double[]) = arrayElementTwo;
	double arr[4] = {5.7, 2.1, 3.1, -1.0};
	printf("arr = {%f, %f, %f, %f}.\n", arr[0], arr[1], arr[2], arr[3]);
	*func(arr) = 4.0;
	printf("arr = {%f, %f, %f, %f}.\n", arr[0], arr[1], arr[2], arr[3]);
	func = arrayElementOne;
	*func(arr) = 4.0;
	printf("arr = {%f, %f, %f, %f}.\n", arr[0], arr[1], arr[2], arr[3]);
	/* What is going on here? */
	*func(func(arr)) = -2.4;
	printf("arr = {%f, %f, %f, %f}.\n", arr[0], arr[1], arr[2], arr[3]);
	return 0;
}

/* For many more examples of function pointers, see 000pixel.h. Each 
callback-setting function takes the callback as a pointer-to-function. */


