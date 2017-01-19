//Xingfan Xia, January 12th
//me the classes I have missed for starting this class in the middle of second week


//To Generate tex coord S and T at vector x by applying formulae learnt in class
void getSTcoordinates(renRenderer *ren, double unif[], double x[], double a[], double b[], double c[], double attr[]) {

	double m[2][2] = {
		{b[0]-a[0], c[0]-a[0]},
		{b[1]-a[1], c[1]-a[1]}
	};
	double Inv[2][2] = {
		{0.0,0.0},
		{0.0,0.0}
	};	
	//apply interpolation formula learnt in class
	double det = mat22Invert(m,Inv);
	double vec[2] = {x[0]-a[0], x[1]-a[1]};
	double pq[2] = {0.0, 0.0};
	mat221Multiply(Inv,vec,pq);
	if (det != 0) {
		//a temp to hold intermediate results
		double temp[ren->attrDim];
		//change to vector
		vecSubtract(ren->attrDim, b, a, temp);
		vecScale(ren->attrDim, pq[0], temp, attr);
		vecSubtract(ren->attrDim, c, a, temp);
		vecScale(ren->attrDim, pq[1], temp, temp);
		vecAdd(ren->attrDim, temp, attr, attr);
		vecAdd(ren->attrDim, attr, a, attr);		
	} else {
		printf("The matrix doesn't have an Inverse, something is wrong here\n");
	}

}

/* triRenderAleft rasterizes a triangle whose vertices are given in 
a counter-clockwise order and with each pixel's rgb interpolated by
texture coord and unif background.*/
void triRenderALeft(renRenderer *ren, double unif[], texTexture *tex[], 
		double a[], double b[], double c[]) {
	
	double STvalue[ren->attrDim];
	double sampleRGB[3];
	double x[2];

	if (a[0] == b[0] == c[0]) {
		printf("Three Vertices on the same axis! Can't draw! \n");
		exit(0);
	}
	//c[0]<b[0], so it is an Acute Triangle/Right Triangle, Angle(abc)<=90
	if (c[0] <= b[0]){ 

		//A and C on the same y-axis
		if (a[0] == c[0]){ 
			/* Some triangle
			C

						B
			A            
			*/
			/* The left bound is (int)ceil(a[0]) and the right bound is (int)floor(b[0])*/
			for (x[0]=(int)ceil(a[0]); x[0]<=(int)floor(b[0]); x[0]++){
				int x1_low;
				int x1_high;
				/*The lower bound is ac and the higher bound is bc*/
				x1_low = a[1]+(b[1]-a[1])/(b[0]-a[0])*(x[0]-a[0]);
				x1_high = c[1]+(b[1]-c[1])/(b[0]-c[0])*(x[0]-c[0]);
				for (x[1]=(int)ceil(x1_low); x[1]<=(int)floor(x1_high); x[1]++){
					//get s and t
					getSTcoordinates(ren, unif, x, a, b, c, STvalue);
					
					//get RGB infos
					colorPixel(ren, unif, tex, STvalue, sampleRGB);
					
					//Set color
					pixSetRGB(x[0], x[1], sampleRGB[0], sampleRGB[1], sampleRGB[2]);
				}
			}
		/*case2: B and C on the same y-axis*/
		} else if (c[0] == b[0]){
			/* Some triangle
						 C

		
						 B
			A            
			*/			
			for (x[0]=(int)ceil(a[0]); x[0]<=(int)floor(b[0]); x[0]++){
				int x1_low;
				int x1_high;
				x1_low = a[1]+(b[1]-a[1])/(b[0]-a[0])*(x[0]-a[0]);
				x1_high = a[1]+(c[1]-a[1])/(c[0]-a[0])*(x[0]-a[0]);
				for (x[1]=(int)ceil(x1_low); x[1]<=(int)floor(x1_high); x[1]++){
					//get s and t
					getSTcoordinates(ren, unif, x, a, b, c, STvalue);
					
					//get RGB infos
					colorPixel(ren, unif, tex, STvalue, sampleRGB);
					
					//Set color
					pixSetRGB(x[0], x[1], sampleRGB[0], sampleRGB[1], sampleRGB[2]);
				}
			}
		/*case3: B in the middle of A and B in terms of x-coordinates*/
		} else {
			/* Some triangle
				  C

		
						 B
			A            
			*/	
			for (x[0]=(int)ceil(a[0]); x[0]<=(int)floor(c[0]); x[0]++){
				//Left sub-Triangle of Triangle ABC
				int x1_low;
				int x1_high;
				x1_low = a[1]+(b[1]-a[1])/(b[0]-a[0])*(x[0]-a[0]);
				x1_high = a[1]+(c[1]-a[1])/(c[0]-a[0])*(x[0]-a[0]);
				for (x[1]=(int)ceil(x1_low); x[1]<=(int)floor(x1_high); x[1]++){
					//get s and t
					getSTcoordinates(ren, unif, x, a, b, c, STvalue);
					
					//get RGB infos
					colorPixel(ren, unif, tex, STvalue, sampleRGB);
					
					//Set color
					pixSetRGB(x[0], x[1], sampleRGB[0], sampleRGB[1], sampleRGB[2]);
				}
			}
			for (x[0]=(int)floor(c[0])+1; x[0]<=(int)floor(b[0]); x[0]++){
				//Right sub-Triangle of Triangle ABC
				int x1_low;
				int x1_high;
				x1_low = a[1]+(b[1]-a[1])/(b[0]-a[0])*(x[0]-a[0]);
				x1_high = c[1]+(b[1]-c[1])/(b[0]-c[0])*(x[0]-c[0]);
				for (x[1]=(int)ceil(x1_low); x[1]<=(int)floor(x1_high); x[1]++){	
					//get s and t
					getSTcoordinates(ren, unif, x, a, b, c, STvalue);
					
					//get RGB infos
					colorPixel(ren, unif, tex, STvalue, sampleRGB);
					
					//Set color
					pixSetRGB(x[0], x[1], sampleRGB[0], sampleRGB[1], sampleRGB[2]);
				}
			}
		}
	/* c[0] > b[0], Obtuse triangles. 90<Angle(abc)*/
	} else {
		for (x[0]=(int)ceil(a[0]); x[0]<=(int)floor(b[0]); x[0]++){
			//Left sub-Triangle of Triangle ABC
			int x1_low;
			int x1_high;
			x1_low = a[1]+(b[1]-a[1])/(b[0]-a[0])*(x[0]-a[0]);
			x1_high = a[1]+(c[1]-a[1])/(c[0]-a[0])*(x[0]-a[0]);
			for (x[1]=(int)ceil(x1_low); x[1]<=(int)floor(x1_high); x[1]++){
				//get s and t
				getSTcoordinates(ren, unif, x, a, b, c, STvalue);
				
				//get RGB infos
				colorPixel(ren, unif, tex, STvalue, sampleRGB);
				
				//Set color
				pixSetRGB(x[0], x[1], sampleRGB[0], sampleRGB[1], sampleRGB[2]);
			}
		}
		for (x[0]=(int)floor(b[0])+1; x[0]<=(int)floor(c[0]); x[0]++){
			//Right sub-Triangle of Triangle ABC
			int x1_low;
			int x1_high;
			x1_low  = b[1]+(c[1]-b[1])/(c[0]-b[0])*(x[0]-b[0]);
			x1_high = a[1]+(c[1]-a[1])/(c[0]-a[0])*(x[0]-a[0]);
			for (x[1]=(int)ceil(x1_low); x[1]<=(int)floor(x1_high); x[1]++){	
				//get s and t
				getSTcoordinates(ren, unif, x, a, b, c, STvalue);
				
				//get RGB infos
				colorPixel(ren, unif, tex, STvalue, sampleRGB);
				
				//Set color
				pixSetRGB(x[0], x[1], sampleRGB[0], sampleRGB[1], sampleRGB[2]);
			}

		}
		
	}
}	

/*triRender is a dummy functio that calls the triRenderAleft which
is the actual method that does the drawing and pass the vertices in
correct order*/
void triRender(renRenderer *ren, double unif[], texTexture *tex[], double a[], 
        double b[], double c[]) { 
	if (a[0] < b[0] && a[0] < c[0]) {
		triRenderALeft(ren, unif, tex, a, b, c);
	} else if (b[0] < a[0] && b[0] < c[0]) {
		triRenderALeft(ren, unif, tex, b, c, a);
	} else if (c[0] < a[0] && c[0] < b[0]) {
		triRenderALeft(ren, unif, tex, c, a, b);
	}
}