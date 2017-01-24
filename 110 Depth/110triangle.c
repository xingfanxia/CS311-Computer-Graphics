//Xingfan Xia, January 20th


//To Generate tex coord S and T at vector x by applying formulae learnt in class
int getChi(renRenderer *ren, double unif[], double x[], double a[], double b[], double c[], double attr[]) {

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
	if (det > 0) {
		double vec[2] = {x[0]-a[0], x[1]-a[1]};
		double pq[2] = {0.0, 0.0};
		mat221Multiply(Inv,vec,pq);
		//a temp to hold intermediate results
		double temp[ren->varyDim];
		//change to vector
		vecSubtract(ren->varyDim, b, a, temp);
		vecScale(ren->varyDim, pq[0], temp, attr);
		vecSubtract(ren->varyDim, c, a, temp);
		vecScale(ren->varyDim, pq[1], temp, temp);
		vecAdd(ren->varyDim, temp, attr, attr);
		vecAdd(ren->varyDim, attr, a, attr);
		return 0;	
	} else {
		return 1;
	}

}

/* triRenderAleft rasterizes a triangle whose vertices are given in 
a counter-clockwise order and with each pixel's rgb interpolated by
texture coord and unif background.*/
void triRenderALeft(renRenderer *ren, double unif[], texTexture *tex[], 
		double a[], double b[], double c[]) {

	
	double STvalue[ren->varyDim];
	double sampleRGBZ[4];
	double x[2];
	double x1_low, x1_high;

	//c[0]<b[0], so it is an Acute Triangle/Right Triangle, Angle(abc)<=90
	if (b[0] <= c[0]){ 

		// all in same y-axis
		if (b[0] == c[0] && a[0] == c[0]) {
			for (x[1] = ceil(fmin(fmin(a[1],b[1]),fmin(a[1],c[1]))); x[1] <= floor(fmax(fmax(a[1],b[1]),fmax(a[1],c[1]))); x[1] ++) {
                /* just draw a vertical line*/
                pixSetRGB(ceil(a[0]), x[1], sampleRGBZ[0], sampleRGBZ[1], sampleRGBZ[2]);
            }
            //b and c on same y-axis
		} else if (b[0] == c[0] && a[0] != c[0]) {
			for (x[0]=(int)ceil(a[0]); x[0]<=(int)floor(b[0]); x[0]++){
			
				/*The lower bound is ac and the higher bound is bc*/
				x1_low = a[1]+(b[1]-a[1])/(b[0]-a[0])*(x[0]-a[0]);
				x1_high = a[1]+(c[1]-a[1])/(c[0]-a[0])*(x[0]-a[0]);
				for (x[1]=(int)ceil(x1_low); x[1]<=(int)floor(x1_high); x[1]++){
					//get s and t
					if (getChi(ren, unif, x, a, b, c, STvalue) != 0) {
						return;
					}
					
					//get RGB infos
					ren->colorPixel(ren, unif, tex, STvalue, sampleRGBZ);
					
					//Compare Z value to what stored in depth buffer
					if (sampleRGBZ[3] > depthGetZ(ren->depth, x[0], x[1])) {
						depthSetZ(ren->depth, x[0], x[1], sampleRGBZ[3]);
						pixSetRGB(x[0], x[1], sampleRGBZ[0], sampleRGBZ[1], sampleRGBZ[2]);
					}
				}
			}
		} else {
			//other normal cases of Acute triangles
			for (x[0]=(int)ceil(b[0]); x[0]<=(int)floor(c[0]); x[0]++){
					//Left sub-Triangle of Triangle ABC
				
					x1_low = c[1]+(b[1]-c[1])/(b[0]-c[0])*(x[0]-c[0]);
					x1_high = a[1]+(c[1]-a[1])/(c[0]-a[0])*(x[0]-a[0]);
					for (x[1]=(int)ceil(x1_low); x[1]<=(int)floor(x1_high); x[1]++){
						//get s and t
						if (getChi(ren, unif, x, a, b, c, STvalue) != 0) {
							return;
						}
						
						//get RGB infos
						ren->colorPixel(ren, unif, tex, STvalue, sampleRGBZ);
						
						//Compare Z value to what stored in depth buffer
						if (sampleRGBZ[3] > depthGetZ(ren->depth, x[0], x[1])) {
							depthSetZ(ren->depth, x[0], x[1], sampleRGBZ[3]);
							pixSetRGB(x[0], x[1], sampleRGBZ[0], sampleRGBZ[1], sampleRGBZ[2]);
						}
					}
				}
			for (x[0]=(int)floor(a[0])+1; x[0]<=(int)floor(b[0]); x[0]++){
				//Right sub-Triangle of Triangle ABC
			
				x1_low = a[1]+(b[1]-a[1])/(b[0]-a[0])*(x[0]-a[0]);
				x1_high = a[1]+(c[1]-a[1])/(c[0]-a[0])*(x[0]-a[0]);
				for (x[1]=(int)ceil(x1_low); x[1]<=(int)floor(x1_high); x[1]++){	
					//get s and t
					if (getChi(ren, unif, x, a, b, c, STvalue) != 0) {
						return;
					}
					
					//get RGB infos
					ren->colorPixel(ren, unif, tex, STvalue, sampleRGBZ);
					
					//Compare Z value to what stored in depth buffer
					if (sampleRGBZ[3] > depthGetZ(ren->depth, x[0], x[1])) {
						depthSetZ(ren->depth, x[0], x[1], sampleRGBZ[3]);
						pixSetRGB(x[0], x[1], sampleRGBZ[0], sampleRGBZ[1], sampleRGBZ[2]);
					}
				}
			}
		} 
	} else {
		//other normal cases of Obtuse triangles
		//A and C on the same y-axis
		if (a[0] == c[0]){ 
			/* Some triangle
			C

						B
			A            
			*/
			/* The left bound is (int)ceil(a[0]) and the right bound is (int)floor(b[0])*/
			for (x[0] = ceil(c[0]); x[0] <= floor(b[0]); x[0] ++) {
                x1_low = a[1] + (b[1]-a[1])/(b[0]-a[0]) * (x[0] - a[0]);
                x1_high = c[1] + (b[1]-c[1])/(b[0]-c[0]) * (x[0] - c[0]);
                for (x[1] = ceil(x1_low); x[1] <= floor(x1_high); x[1] ++) {
					//get s and t
					if (getChi(ren, unif, x, a, b, c, STvalue) != 0) {
						return;
					}
					
					//get RGB infos
					ren->colorPixel(ren, unif, tex, STvalue, sampleRGBZ);
					
					//Compare Z value to what stored in depth buffer
					if (sampleRGBZ[3] > depthGetZ(ren->depth, x[0], x[1])) {
						depthSetZ(ren->depth, x[0], x[1], sampleRGBZ[3]);
						pixSetRGB(x[0], x[1], sampleRGBZ[0], sampleRGBZ[1], sampleRGBZ[2]);
					}
				}
			}
		} else {
			/* Some triangle
				  C

		
						 B
			A            
			*/	
			for (x[0]=(int)ceil(a[0]); x[0]<=(int)floor(c[0]); x[0]++){
				//Left sub-Triangle of Triangle ABC
			
				x1_low = a[1]+(b[1]-a[1])/(b[0]-a[0])*(x[0]-a[0]);
				x1_high = a[1]+(c[1]-a[1])/(c[0]-a[0])*(x[0]-a[0]);
				for (x[1]=(int)ceil(x1_low); x[1]<=(int)floor(x1_high); x[1]++){
					//get s and t
					if (getChi(ren, unif, x, a, b, c, STvalue) != 0) {
						return;
					}
					
					//get RGB infos
					ren->colorPixel(ren, unif, tex, STvalue, sampleRGBZ);
					
					//Compare Z value to what stored in depth buffer
					if (sampleRGBZ[3] > depthGetZ(ren->depth, x[0], x[1])) {
						depthSetZ(ren->depth, x[0], x[1], sampleRGBZ[3]);
						pixSetRGB(x[0], x[1], sampleRGBZ[0], sampleRGBZ[1], sampleRGBZ[2]);
					}
				}
			}
			for (x[0]=(int)floor(c[0])+1; x[0]<=(int)floor(b[0]); x[0]++){
				//Right sub-Triangle of Triangle ABC
			
				x1_low = a[1]+(b[1]-a[1])/(b[0]-a[0])*(x[0]-a[0]);
				x1_high = c[1]+(b[1]-c[1])/(b[0]-c[0])*(x[0]-c[0]);
				for (x[1]=(int)ceil(x1_low); x[1]<=(int)floor(x1_high); x[1]++){	
					//get s and t
					if (getChi(ren, unif, x, a, b, c, STvalue) != 0) {
						return;
					}
					
					//get RGB infos
					ren->colorPixel(ren, unif, tex, STvalue, sampleRGBZ);
					
					//Compare Z value to what stored in depth buffer
					if (sampleRGBZ[3] > depthGetZ(ren->depth, x[0], x[1])) {
						depthSetZ(ren->depth, x[0], x[1], sampleRGBZ[3]);
						pixSetRGB(x[0], x[1], sampleRGBZ[0], sampleRGBZ[1], sampleRGBZ[2]);
					}
				}
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
	} else if (a[0] == c[0] && b[0] != c[0]) {
		triRenderALeft(ren, unif, tex, a, b, c);
	} else if (a[0] == b[0] && a[0] != c[0]) {
		triRenderALeft(ren, unif, tex, b, c, a);
	} else if (b[0] == c[0] && a[0] != b[0]) {
		triRenderALeft(ren, unif, tex, c, a, b);
	} else {
		triRenderALeft(ren, unif, tex, a, b, c);
	}
}





