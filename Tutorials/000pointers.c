


/* On macOS, compile with...
    clang 000pointers.c
*/

#include <stdio.h>

/* A pointer is a memory location. More precisely, it is an integer that serves 
as the address of a memory location.

Here's a real-world analogy. Suppose everyone in the world lived on one very 
long street. Instead of having street addresses like '120 Winona St', they 
would have street addresses like '200343000268'. That is, the street numbers 
would be many digits long (instead of '120'), and there would be no reason to 
list the street name ('Winona St'). Similarly, a location inside your 
computer's memory is identified by a numeric address, and a pointer in C is 
simply this number.

Pointers are infamous for being confusing, but the basic ideas of pointers are 
actually quite simple:
    A. If you have a variable, you can get a pointer to that variable using &.
    B. If you have a pointer, you can access the value it points to using *.
    C. You also use * to declare a variable that has pointer type.
The code below illustrates these concepts. */

void basics(void) {
	/* x is a double. myPtr is a pointer-to-double. */
	double x = 3.14;
	double *myPtr;
	/* The printf code for pointers is '%p'. It prints the pointer as an 
	integer in hexadecimal. The output here is garbage, because we have not 
	initialized myPtr. */
	printf("myPtr = %p.\n", myPtr);
	/* Use & to get the address of x, and store that address in myPtr. */
	myPtr = &x;
	/* Now myPtr contains a meaningful value. */
	printf("myPtr = %p.\n", myPtr);
	/* Use * to get the contents pointed to by myPtr. */
	printf("The value, to which myPtr points, is %f.\n", *myPtr);
	printf("Because myPtr points to x, the value is the same as %f.\n", x);
	/* Alter the contents via myPtr. Notice that x changes. */
	*myPtr += 3.0;
	printf("*myPtr = %f, and x = %f.\n", *myPtr, x);
	/* Alter the contents via x. Notice that *myPtr changes. */
	x += 3.0;
	printf("*myPtr = %f, and x = %f.\n", *myPtr, x);
	/* But this arithmetic has not changed myPtr itself. It points to the same 
	memory location as it always has. It's just the contents stored at that 
	location that have changed. */
	printf("myPtr = %p.\n", myPtr);
	/* Change myPtr to NULL, which is another way of saying 0. The memory 
	location with address 0 is reserved. It is never an actual memory location 
	for a variable in your program. */
	myPtr = NULL;
	printf("myPtr = %p.\n", myPtr);
	/* If you uncomment the next line, the compiler will not complain, but the 
	program will generate an fatal error at runtime. Trying to use a bogus 
	address is strictly forbidden, and 0 is definitely a bogus address. */
	/*printf("*myPtr = %f.\n", *myPtr);*/
	/* x is still there. It doesn't care that myPtr has been nullified. */
	printf("x = %f.\n", x);
}

void sizes(void) {
	/* Remember that a double * is not a double, but rather an address. And an 
	int * is not an int, but rather an address. So you should not be surprised 
	to learn that double * and int * have the same size. */
	printf("sizeof(double) = %d, sizeof(double *) = %d.\n", 
		(int)sizeof(double), (int)sizeof(double *));
	printf("sizeof(int) = %d, sizeof(int *) = %d.\n", 
		(int)sizeof(int), (int)sizeof(int *));
	/* On a '64-bit computer', all memory addresses are 8 bytes. On a 
	'32-bit computer', they are 4 bytes. Which one is your computer? */
	printf("sizeof(char) = %d, sizeof(char *) = %d.\n", 
		(int)sizeof(char), (int)sizeof(char *));
}

void multiple(void) {
	/* Can you have pointers to pointers? Yep. */
	int y = 18, *yPtr, **yPtrPtr;
	yPtr = &y;
	yPtrPtr = &yPtr;
	printf("%d = %d = %d.\n", y, *yPtr, **yPtrPtr);
	printf("%p = %p = %p.\n", &y, yPtr, *yPtrPtr);
	printf("%p = %p.\n", &yPtr, yPtrPtr);
	/* If you uncomment the next line, it will not work, because && is the 
	logical AND operator, which requires two operands. (The && operator has 
	nothing to do with the & operator or pointers.) */
	/*printf("&&y = %p.\n", &&y);*/
	/* If you uncomment the next line, it will not work. The value of the 
	subexpression &y does not have a well-defined memory location. (Depending 
	on how the code is compiled, it might not even reside in RAM. It might just 
	reside in a CPU register.) So you're not allowed to ask what that location 
	is. Try the next line, and see how the compiler complains. */
	/*printf("&(&y) = %p.\n", &(&y));*/
}

/* Parameter c is a pointer-to-int, not just an int. */
void special(int *c) {
	/* c is the address of a variable that exists outside this function. So *c 
	is that variable. So altering *c really does alter that variable. */
	*c = *c / 10;
}

/* Parameter a is an int, but parameters b and c are pointers-to-int. */
void update(int a, int *b, int *c) {
	/* Remember that a is an independent copy of an integer that was passed to 
	update. So adding 2 to a affects the value of a inside update, but does not 
	affect the value of that integer outside update. */
	a += 2;
	/* Adding 2 to the integer *b has an effect outside this function, because 
	b is the memory location for a variable outside the function, and *b is 
	that variable outside the function. */
	*b += 2;
	/* special expects its argument to be a pointer-to-int. That's what c is, 
	so we just pass c --- not *c (which is int) or &c (which is int **). */
	special(c);
	printf("a = %d, *b = %d, *c = %d.\n", a, *b, *c);
}

void functions(void) {
	int n = 10, m = -3, p = 123;
	printf("n = %d, m = %d, p = %d.\n", n, m, p);
	/* We pass the address of m and p to update, because update expects its 
	second and third arguments to be pointers. */
	update(n, &m, &p);
	/* Notice that update alters m and p, but not n. */
	printf("n = %d, m = %d, p = %d.\n", n, m, p);
}

/* Functions can return pointers. Occasionally that is useful. The following 
example is legal, but not useful, and actually quite dangerous. (If you 
uncomment it, your compiler may even emit a warning.) Why? */
/*double * dangerous(void) {
	double x;
	return &x;
}*/

int main(void) {
	basics();
	sizes();
	multiple();
	functions();
	/* Uncomment the next line, and the definition of dangerous above, if you 
	dare. (Your computer will not be hurt. This is just a terrible way to write 
	a program.) */
	/*printf("dangerous returns %p.\n", dangerous());*/
}


