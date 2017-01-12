


/* On macOS, compile with...
    clang 000functions.c
*/

#include <stdio.h>
#include <math.h>

/* We have already seen one example of a function: the main function, which is 
the core of any C program. As in any programming language, we can write other 
functions to help main do its work. */

/* As input the following function takes one double, called 'x'. As output it 
returns a double, that is approximately cos(x). (The approximation is good for
x near 0 but terrible for x far away from 0. If you've never seen this math, 
don't worry about the details.) */
double cosApprox(double x) {
	return 1.0 - x * x / 2.0 + x * x * x * x / 24.0;
}

/* This function inputs an int and outputs an int. Notice the recursion. */
int factorial(int n) {
	if (n == 0)
		return 1;
	else
		return n * factorial(n - 1);
}

/* A function that doesn't return anything has return type void. */
void printProduct(double x, double y) {
	printf("%f * %f = %f.\n", x, y, x * y);
}

/* Here's a function for computing integer powers of doubles. Notice the 
recursion. Also notice that we alter the exponent parameter. But the parameter 
is not altered in the calling environment, because the function is operating on 
an independent copy of the parameter in memory. C is 'call-by-value'. */
double power(double base, int exponent) {
	if (exponent < 0)
		return power(1.0 / base, -exponent);
	else {
		double answer;
		answer = 1.0;
		while (exponent > 0) {
			answer = answer * base;
			exponent = exponent - 1;
		}
		return answer;	
	}
}

int main(void) {
	/* You invoke functions in the way you'd expect from other languages. */
	double x, y;
	x = 0.1;
	printf("cos(%f) = %f ~ %f.\n", x, cos(x), cosApprox(x));
	x = 2.1;
	printf("cos(%f) = %f ~ %f.\n", x, cos(x), cosApprox(x));
	x = 4.1;
	printf("cos(%f) = %f ~ %f.\n", x, cos(x), cosApprox(x));
	/* The following code is working perfectly, but what's wrong with it? */
	int i;
	for (i = 11; i <= 14; i = i + 1)
		printf("%d! = %d.\n", i, factorial(i));
	y = 3.4;
	/* Here's our example of a void function. */
	printProduct(x, y);
	/* Notice that the value of i doesn't change, although power changes the 
	value of exponent. That's call-by-value. */
	i = 3;
	y = power(x, i);
	printf("i = %d.\n", i);
	printf("power(%f, %d) = %f.\n", x, i, y);
	printf("i = %d.\n", i);
	return 0;
}


