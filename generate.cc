/* generate.cc */

// conversion functions

point vec_to_klein_point(vec V){
	point p;
	p.x=500 + (int) (400.0*V[0]/V[3]);
	p.y=430 + (int) (400.0*V[1]/V[3]);
	return(p);
};

point vec_to_poincare_point(vec V){
	point p;
	p.x=500 + (int) (400.0*V[0]/(V[3]+1.0));
	p.y=430 + (int) (400.0*V[1]/(V[3]+1.0));
	return(p);
};

vec vec_to_poincare_vec(vec V){		// project vector from hyperboloid to unit ball in Poincare
	vec U;
	dbl t;
	t=V[3]+1.0;
	U=(1.0/t)*V;
	return(U);
};

triangle triangle_to_poincare_triangle(triangle T){
	triangle U;
	U.v[0]=vec_to_poincare_vec(T.v[0]);
	U.v[1]=vec_to_poincare_vec(T.v[1]);
	U.v[2]=vec_to_poincare_vec(T.v[2]);
	return(U);
};

vec normal(triangle T){		// computes unit normal to plane spanned by Euclidean triangle
	vec U;
	dbl t;
	U=cross(T.v[1]-T.v[0],T.v[2]-T.v[0]);	// first compute cross product
	t=sqrt(U[0]*U[0]+U[1]*U[1]+U[2]*U[2]);
	U=(1.0/t)*U;							// then scale
	return(U);
};

void kleinian_group::generate_triangles(){
	int i,j;
	triangle T;
	vec N;
	
	DRAW_TRIANGLES.clear();
	DRAW_NORMALS.clear();
	
	for(i=0;i<(int) ELEMENTS.size();i++){
		for(j=0;j<(int) TRIANGLES.size();j++){
			T=CAMERA*ELEMENTS[i].M*TRIANGLES[j];	// hyperbolic triangle
			T=triangle_to_poincare_triangle(T);		// Euclidean triangle
			N=normal(T);							// Euclidean normal
			DRAW_TRIANGLES.push_back(T);
			DRAW_NORMALS.push_back(N);
		};
	};
};

void kleinian_group::sort_triangles(){
	int i;
	vec N;
	
	DRAW_TRIANGLES = sort(DRAW_TRIANGLES);			// sort triangles

	/* recompute normals */
	DRAW_NORMALS.clear();	
	for(i=0;i<(int) DRAW_TRIANGLES.size();i++){
		N=normal(DRAW_TRIANGLES[i]);
		DRAW_NORMALS.push_back(N);
	};
};