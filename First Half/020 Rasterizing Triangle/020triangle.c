//Xingfan Xia, January 11th, With the help from Yanhan Lyu for teaching
//me the classes I have missed for starting this class in the middle of second week
#include <stdio.h>
#include <math.h>
#include "000pixel.h"

/* triRender rasterizes a triangle whose vertices are given in 
a counter-clockwise order and whose color is set by the given 
r,g and b.*/
void triRender(double a0, double a1, double b0, double b1, double c0, 
	double c1, double r, double g, double b) {
	//Init vector A, B, C
	double vectorA[2];
	double vectorB[2];
	double vectorC[2];

	/* Basically transforming the vertices A, B, C into a order where A is at the
	most left position and then B, C are positioned as according to the counter-
	clockwise order*/
	
	/* case1: If V_A is the most left*/
	if (a0< b0 && a0 <c0){
		vectorA[0] = a0;
		vectorA[1] = a1;
		vectorB[0] = b0;
		vectorB[1] = b1;
		vectorC[0] = c0;
		vectorC[1] = c1;
	/* case2: If V_B is the most left*/
    } else if (b0 < a0 && b0 < c0){
		vectorA[0] = b0;
		vectorA[1] = b1;
		vectorB[0] = c0;
		vectorB[1] = c1;
		vectorC[0] = a0;
		vectorC[1] = a1;
	/* case3: If V_C is the most left*/
	} else if (c0<a0 && c0<b0){
		vectorA[0] = c0;
		vectorA[1] = c1;
		vectorB[0] = a0;
		vectorB[1] = a1;
		vectorC[0] = b0;
		vectorC[1] = b1;
	} else {
		printf("The vertices have to make a triangle");
	}
	/*Reassign values back to coordinates*/ 
	a0 = vectorA[0];
	a1 = vectorA[1];
	b0 = vectorB[0];
	b1 = vectorB[1];
	c0 = vectorC[0];
	c1 = vectorC[1];

	//c0<b0, so it is an Acute Triangle/Right Triangle, Angle(abc)<=90
	if (c0 <= b0){ 

		//A and C on the same y-axis
		if (a0 == c0){ 
			/* Some triangle
			C

						B
			A            
			*/
			int x0;
			/* The left bound is (int)ceil(a0) and the right bound is (int)floor(b0)*/
			for (x0=(int)ceil(a0); x0<=(int)floor(b0); x0++){
				int x1_low;
				int x1_high;
				/*The lower bound is ac and the higher bound is bc*/
				x1_low = a1+(b1-a1)/(b0-a0)*(x0-a0);
				x1_high = c1+(b1-c1)/(b0-c0)*(x0-c0);
				int x1;
				for (x1=(int)ceil(x1_low); x1<=(int)floor(x1_high); x1++){
					pixSetRGB(x0,x1,r,g,b);
				}
			}
        /*case2: B and C on the same y-axis*/
		} else if (c0 == b0){
			/* Some triangle
			             C

		
						 B
			A            
			*/			
			int x0;
			for (x0=(int)ceil(a0); x0<=(int)floor(b0); x0++){
				int x1_low;
				int x1_high;
				x1_low = a1+(b1-a1)/(b0-a0)*(x0-a0);
				x1_high = a1+(c1-a1)/(c0-a0)*(x0-a0);
				int x1;
				for (x1=(int)ceil(x1_low); x1<=(int)floor(x1_high); x1++){
					pixSetRGB(x0,x1,r,g,b);
				}
			}
		/*case3: B in the middle of A and B in terms of x-coordinates*/
		} else {
			/* Some triangle
			      C

		
						 B
			A            
			*/	
			int x0;
			for (x0=(int)ceil(a0); x0<=(int)floor(c0); x0++){
				//Left sub-Triangle of Triangle ABC
				int x1_low;
				int x1_high;
				x1_low = a1+(b1-a1)/(b0-a0)*(x0-a0);
				x1_high = a1+(c1-a1)/(c0-a0)*(x0-a0);
				int x1;
				for (x1=(int)ceil(x1_low); x1<=(int)floor(x1_high); x1++){
					pixSetRGB(x0,x1,r,g,b);
				}
			}
			for (x0=(int)floor(c0)+1; x0<=(int)floor(b0); x0++){
				//Right sub-Triangle of Triangle ABC
				int x1_low;
				int x1_high;
				x1_low = a1+(b1-a1)/(b0-a0)*(x0-a0);
				x1_high = c1+(b1-c1)/(b0-c0)*(x0-c0);
				int x1;
				for (x1=(int)ceil(x1_low); x1<=(int)floor(x1_high); x1++){	
					pixSetRGB(x0,x1,r,g,b);
				}
			}
		}
	/* c0 > b0, Obtuse triangles. 90<Angle(abc)*/
	} else {
		int x0;
		for (x0=(int)ceil(a0); x0<=(int)floor(b0); x0++){
			//Left sub-Triangle of Triangle ABC
			int x1_low;
			int x1_high;
			x1_low = a1+(b1-a1)/(b0-a0)*(x0-a0);
			x1_high = a1+(c1-a1)/(c0-a0)*(x0-a0);
			int x1;
			for (x1=(int)ceil(x1_low); x1<=(int)floor(x1_high); x1++){
				pixSetRGB(x0,x1,r,g,b);
			}
		}
		for (x0=(int)floor(b0)+1; x0<=(int)floor(c0); x0++){
			//Right sub-Triangle of Triangle ABC
			int x1_low;
			int x1_high;
			x1_low  = b1+(c1-b1)/(c0-b0)*(x0-b0);
			x1_high = a1+(c1-a1)/(c0-a0)*(x0-a0);
			int x1;
			for (x1=(int)ceil(x1_low); x1<=(int)floor(x1_high); x1++){	
				pixSetRGB(x0,x1,r,g,b);
			}

		}
		
	}	
}

