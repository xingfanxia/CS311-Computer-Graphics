


/* On macOS, compile with...
	clang 000variableArity.c
*/

#include <stdio.h>
/* This standard C library is specifically for writing functions that take a 
varying number of arguments. */
#include <stdarg.h>

/* This function takes as input an integer count followed by count doubles. It 
returns the sum of those doubles. */
double total(int count, ...) {
	double runningTotal = 0.0;
	/* This variable will point to each anonymous argument in turn. */
	va_list argumentPointer;
	/* Tell the argument pointer to start at the argument after count. */
	va_start(argumentPointer, count);
	/* You give va_arg an argument pointer and a type name. It interprets the 
	current argument to be of that type and returns that argument. As a side 
	effect, it increments the argument pointer so that it points to the next 
	argument. */
	int i;
	for (i = 0; i < count; i += 1)
		runningTotal += va_arg(argumentPointer, double);
	/* You must clean up the argument pointer at the end. */
	va_end(argumentPointer);
	return runningTotal;
}

int main(void) {
	/* Here's an example and a check. */
	printf("The sum is %f.\n", total(4, 3.1, 1.0, 0.6, 2.6));
	printf("The sum is %f.\n", 3.1 + 1.0 + 0.6 + 2.6);
	/* Check some extreme cases. */
	printf("The sum is %f.\n", total(1, 3.1));
	printf("The sum is %f.\n", total(0));
	/* What happens if you pass too many summands? The extras are ignored. */
	printf("The sum is %f.\n", total(2, 3.1, 1.0, 0.6));
	/* What happens if you pass too few summands? Extra summands are formed out 
	of whatever garbage happens to be in memory. */
	printf("The sum is %f.\n", total(4, 3.1, 1.0, 0.6));
	/* There is no type checking on the arguments, and therefore no automatic 
	type promotion, for example from int to double. */
	printf("The sum is %f.\n", total(4, 3.1, 1, 0.6, 2.6));
	return 0;
}

/* Can you write a function with no named arguments, as in 'double total(...)'? 
No. There must be at least one named argument. */

/* Is there any way for stdarg.h to detect how many arguments there are, so 
that we don't have to tell it through count? As far as I know, no. You must 
somehow signal how many arguments there are. For example, printf is another 
example of a variable-arity function. How does it signal? */

/* Is this stuff dangerous? Should we use it sparingly? Yes. */


