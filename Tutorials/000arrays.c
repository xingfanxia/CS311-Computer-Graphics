


/* On macOS, compile with...
    clang 000arrays.c
*/

#include <stdio.h>

/* The following function inputs a list of numbers and outputs their sum. The 
parameter a is not just a double, but an array of doubles. Arrays are much like 
lists in other languages, but in C they have two crucial limitations. First, 
every element of the array must be of the same type --- in this, case, double. 
Second, an array does not know its length. That's why we pass the length as a 
separate parameter. The array elements are indexed from 0, not from 1. */
double sum(int length, double a[]) {
	int i;
	/* By the way, you may initialize a variable when you declare it. */
	double total = 0.0;
	/* By the way, the '+=' operator behaves as in other languages. For 
	example, 'i += 1' is shorthand for 'i = i + 1'. */
	for (i = 0; i < length; i += 1)
		total += a[i];
	return total;
}

/* Recall from an earlier tutorial that C is call-by-value. That is, when a 
parameter is passed to a function, a copy of that parameter is passed. So you 
can alter the parameter inside the function, without altering its value outside 
the function. Right? The tricky part is that a C array is actually a memory 
location. So when you alter a C array in a function, you are altering the array 
outside the function too. Here's an example. */
void cumulativeSum(int length, double a[]) {
	int i;
	for (i = 1; i < length; i += 1)
		a[i] += a[i - 1];
}

/* This function determines which elements of the array a are divisible by the 
parameter divisor (with an integer quotient). This function doesn't alter the 
array a, but it does alter the array indices. It is important that indices has 
already been allocated to hold enough ints --- as many as length of them. */
int integerMultipleIndices(int length, int a[], int divisor, int indices[]) {
	int i, count = 0;
	for (i = 0; i < length; i += 1)
		if (a[i] % divisor == 0) {
			indices[count] = i;
			count += 1;
		}
	return count;
}

int main(void) {
	/* If you like, you may initialize an array as follows. */
	double myArray[4] = {1.5, 2.3, 3.7, 4.1};
	int i;
	printf("Here are the elements of myArray:\n");
	for (i = 0; i < 4; i += 1)
		printf("    myArray[%d] = %f.\n", i, myArray[i]);
	printf("The sum of myArray is %f.\n", sum(4, myArray));
	/* Observe how cumulativeSum alters its array argument. */
	printf("Now let's compute the cumulative sums of myArray:\n");
	cumulativeSum(4, myArray);
	for (i = 0; i < 4; i += 1)
		printf("    myArray[%d] = %f.\n", i, myArray[i]);
	/* Here are some arrays of ints. The first one is allocated and 
	initialized. The second one is allocated but not initialized. */
	int myInts[7] = {9, 13, 14, 21, -3, 0, -1};
	printf("Here is myInts, exactly as we initialized it.\n");
	for (i = 0; i < 7; i += 1)
		printf("    myInts[%d] = %d.\n", i, myInts[i]);
	int indices[7];
	printf("Here is indices, holding whatever it happens to hold.\n");
	for (i = 0; i < 7; i += 1)
		printf("    indices[%d] = %d.\n", i, indices[i]);
	int divisor = 3;
	int count = integerMultipleIndices(7, myInts, divisor, indices);
	printf("Here are the entries of myInts that are divisible by %d.\n", 
		divisor);
	for (i = 0; i < count; i += 1)
		/* Yes, you can index an array to get an index into another array. */
		printf("myInts[%d] = %d.\n", indices[i], myInts[indices[i]]);
	return 0;
}


