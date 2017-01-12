


/* On macOS, compile with...
    clang 000structs.c
*/

#include <stdio.h>

/* Like arrays, structs are composite data types, made by gluing other data 
types together. But whereas an array's elements must all be of the same type, a 
struct's elements may be of varying types. Also an array refers to its elements 
using integer indices, while a struct refers to its elements using identifiers 
(textual names). */

/* This first piece of code declares a new data type called 'Employee'. */
typedef struct Employee Employee;

/* This second piece of code declares the struct that underlies the type. */
struct Employee {
	char *firstName;		/* This is a string, such as "Juana". */
	char *lastName;			/* We don't use strings much in this course. */
	int birthYear;			/* birthYear could be 1998, for example. */
	double hairColor[3];	/* Yes, an array can be a member of a struct. */
};

/* If you've studied C elsewhere, you might know other, less-verbose ways to 
define a struct type. Later in this tutorial I explain two great advantages of 
this way over other ways. I always use this way, and it never lets me down. */

/* This function takes an Employee as input. Because C is call-by-value, emp is 
a copy of whatever Employee object was passed to the function. The function 
'ages' the Employee by making the Employee's hair whiter. Then it returns its 
aged copy of the Employee. */
Employee ageWithoutPointers(Employee emp) {
	/* First we use the . operator to access the Employee's hairColor element, 
	which is an array. Then we use [0] to access the red channel. */
	emp.hairColor[0] = (emp.hairColor[0] + 1.0) / 2.0;
	emp.hairColor[1] = (emp.hairColor[1] + 1.0) / 2.0;
	emp.hairColor[2] = (emp.hairColor[2] + 1.0) / 2.0;
	return emp;
}

/* The following function also ages an Employee by whitening the Employee's 
hair, but the design is different. Instead of operating on multiple copies of 
an Employee, it operates on a pointer to a single copy. Usually, pointers are 
smaller than the structs to which they point, and functions are written to 
operate on pointers, not the structs themselves. Therefore ageWithPointers is 
'better style' than ageWithoutPointers. */
void ageWithPointers(Employee *emp) {
	/* First we use * to access the Employee itself. Then we use . to access 
	its hairColor element. Then we use [0] to access the red channel. */
	(*emp).hairColor[0] = ((*emp).hairColor[0] + 1.0) / 2.0;
	/* Pointers to structs are so common that there is a shortcut, ->, for the 
	* operator followed by the . operator. */
	emp->hairColor[1] = (emp->hairColor[1] + 1.0) / 2.0;
	emp->hairColor[2] = (emp->hairColor[2] + 1.0) / 2.0;
}

/* Earlier I promised to explain two advantages of my method for defining 
struct types. The first advantage is that you can place the first piece 
('typedef ...') into a .h file and the second piece ('struct ...') into a .c 
file, so that your type is available to other programmers but the details are 
hidden from them. The second advantage is that it permits recursive data 
structures. For example, here we define a linked-list-of-doubles type. */
typedef struct DoubleNode DoubleNode;
struct DoubleNode {
	double payload;		/* The data, that the node exists to hold. */
	DoubleNode *next;	/* A pointer to the next node, or NULL if none. */
};

/* Here's a function that prints a linked list. C does not support object-
oriented design, but you can fake it by combining structs with functions like 
this. (In fact, several object-oriented languages are implemented in C, by 
pursuing this idea to its logical conclusion. Talk to me about that if you're 
interested.) */
void DoubleNode_print(DoubleNode *node) {
	DoubleNode *current = node;
	while (current->next != NULL) {
		printf("%f, ", current->payload);
		current = current->next;
	}
	printf("%f\n", current->payload);
}

int main(void) {
	/* You can initialize structs using curly brackets, much like arrays. */
	Employee first = {"Ada", "Lovelace", 1815, {0.1, 0.1, 0.0}};
	Employee second = {"Babatope", "Ogunmola", 1976, {0.0, 0.0, 0.0}};
	/* Or you can allocate them uninitialized. */
	Employee third;
	/* Access a member of a struct using the . operator. */
	printf("The first employee is %d years old.\n", 2017 - first.birthYear);
	printf("The second employee's green is %f.\n", second.hairColor[1]);
	/* ageWithoutPointers inputs an Employee and returns an Employee. */
	second = ageWithoutPointers(second);
	printf("After some aging, the green is %f.\n", second.hairColor[1]);
	/* ageWithPointers inputs a pointer-to-Employee. */
	ageWithPointers(&second);
	printf("After more aging, the green is %f.\n", second.hairColor[1]);
	/* And let's demo our linked list a little. */
	DoubleNode c = {300.0, NULL};
	DoubleNode b = {200.0, &c};
	DoubleNode a = {100.0, &b};
	DoubleNode_print(&a);
	return 0;
}


