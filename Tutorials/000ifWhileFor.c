


/* On macOS, compile with...
    clang 000ifWhileFor.c
*/

#include <stdio.h>
#include <math.h>

int main(void) {
	/* if statements work much as in other languages, so I'll skip the simplest 
	examples. When a branch consists of multiple statements, as in the first 
	else-branch below, then those statements must be enclosed by curly 
	brackets. Otherwise, they are optional. */
	double numer, denom, ratio;
	numer = 17.3;
	denom = -4.1;
	if (denom == 0.0)
		printf("Error: Division by zero.\n");
	else {
		ratio = numer / denom;
		if (ratio > 0.0)
			printf("The ratio is positive.\n");
		else if (ratio == 0.0)
			printf("The ratio is zero.\n");
		else
			printf("The ratio is negative.\n");
	}
	/* As in other languages, a while loop is essentially a repeated if 
	statement. Usually the body consists of multiple statements and hence must 
	be enclosed by curly brackets. */
	while (numer > 1.5) {
		printf("numer is %f.\n", numer);
		numer = sqrt(numer);
	}
	/* As in other languages, a for loop is a special kind of while loop. The 
	first line of the loop consists of three statements separated (not 
	terminated) by semicolons. */
	int i;
	for (i = 0; i < 7; i = i + 2) {
		printf("Here is i: %d.\n", i);
		printf("Here is denom^i: %f.\n", pow(denom, i));
	}
	/* In the first line of the for loop, the first statement is an 
	initializer. The second statement is a condition; once this condition is 
	violated, the loop ends. The third statement is an arbitrary statement to 
	be executed at the end of the loop body. Usually it is some kind of 
	incrementer or decrementer. In other words, the for loop above is 
	functionally identical to the following code. */
	i = 0;
	while (i < 7) {
		printf("Here is i: %d.\n", i);
		printf("Here is denom^i: %f.\n", pow(denom, i));
		i = i + 2;
	}
	return 0;
}


