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

vec vec_to_klein_vec(vec V){
	vec U;
	dbl t;
	t=V[3];
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

vec hyperbolic_midpoint(vec U, vec V){	// "midpoint" in hyperboloid model of U and V
	vec W;
	W=0.5*(vec_to_klein_vec(U)+vec_to_klein_vec(V));	// klein midpoint
	W=(1.0/Hnorm(W))*W;
	return(W);
};

vector< triangle > triangle_to_poincare_triangle_list(triangle T, dbl m){
	vector< triangle> Ulist, X, Y;
	triangle U,V;
	vec A,B,C,M;
	Ulist.clear();
	
	A=vec_to_poincare_vec(T.v[0]);
	B=vec_to_poincare_vec(T.v[1]);
	C=vec_to_poincare_vec(T.v[2]);
	dbl ab,ac,bc;
	ab=Enorm(B-A);
	ac=Enorm(C-A);
	bc=Enorm(C-B);
	if(ab<0.0001 || ac<0.0001 || bc<0.0001){
		Ulist.clear();
		return(Ulist);	// empty list
	};
	if(ab>m || ac>m || bc>m){		// some edge bigger than mesh size
		if(ab>=ac && ab>=bc){
			// subdivide edge AB and return two concatenated triangle lists
			U=T;
			V=T;
			M=hyperbolic_midpoint(T.v[0],T.v[1]);
			U.v[1]=M;
			V.v[0]=M;
			X=triangle_to_poincare_triangle_list(U,m);
			Y=triangle_to_poincare_triangle_list(V,m);
			Ulist=X;
			Ulist.insert( Ulist.end(), Y.begin(), Y.end());		
		} else if(ac>=bc){
			// subdivide edge AC and return two concatenated triangle lists
			U=T;
			V=T;
			M=hyperbolic_midpoint(T.v[0],T.v[2]);
			U.v[2]=M;
			V.v[0]=M;
			X=triangle_to_poincare_triangle_list(U,m);
			Y=triangle_to_poincare_triangle_list(V,m);
			Ulist=X;
			Ulist.insert( Ulist.end(), Y.begin(), Y.end());			
		} else {
			// subdivide edge BC and return two concatenated triangle lists
			U=T;
			V=T;
			M=hyperbolic_midpoint(T.v[1],T.v[2]);
			U.v[2]=M;
			V.v[1]=M;
			X=triangle_to_poincare_triangle_list(U,m);
			Y=triangle_to_poincare_triangle_list(V,m);
			Ulist=X;
			Ulist.insert( Ulist.end(), Y.begin(), Y.end());		
		};
	} else {
		// no subdivision
		U=triangle_to_poincare_triangle(T);
		Ulist.push_back(U);	
	};

	return(Ulist);
};

vec normal(triangle T){		// computes unit normal to plane spanned by Euclidean triangle
	vec U;
	dbl t;
	U=cross(T.v[1]-T.v[0],T.v[2]-T.v[0]);	// first compute cross product
	t=sqrt(U[0]*U[0]+U[1]*U[1]+U[2]*U[2]);
	U=(1.0/t)*U;							// then scale
	return(U);
};

void kleinian_group::fancy_generate_triangles(){
	int i,j,k;
	triangle T;
	vec N;
	vector<triangle > Tlist;
	vec C;
	
	DRAW_TRIANGLES.clear();
	DRAW_NORMALS.clear();
	
	cout << "subdividing triangles\n";
	
	for(i=0;i<(int) ELEMENTS.size();i++){
		for(j=0;j<(int) TRIANGLES.size();j++){
			T=CAMERA*ELEMENTS[i].M*TRIANGLES[j];	// hyperbolic triangle
			Tlist=triangle_to_poincare_triangle_list(T,mesh);		// Euclidean triangle list
			if((int) COLORS.size() > 0){
				C=COLORS[j];
			} else {
				C=build_vec(1.0,1.0,1.0,0.0);	// default color
			};
			for(k=0;k<(int) Tlist.size();k++){
				N=normal(Tlist[k]);							// Euclidean normal
				DRAW_TRIANGLES.push_back(Tlist[k]);
				DRAW_NORMALS.push_back(N);
				DRAW_COLORS.push_back(C);
			};
		};
	};
	cout << (int) DRAW_TRIANGLES.size() << " triangles\n";
};

void kleinian_group::generate_triangles(){
	int i,j;
	triangle T;
	vec N;
	vec C;
	
	DRAW_TRIANGLES.clear();
	DRAW_NORMALS.clear();
	DRAW_COLORS.clear();
	
	for(i=0;i<(int) ELEMENTS.size();i++){
		for(j=0;j<(int) TRIANGLES.size();j++){
			T=CAMERA*ELEMENTS[i].M*TRIANGLES[j];	// hyperbolic triangle
			T=triangle_to_poincare_triangle(T);		// Euclidean triangle
			N=normal(T);							// Euclidean normal
			if((int) COLORS.size() > 0){
				C=COLORS[j];
			} else {
				C=build_vec(1.0,1.0,1.0,0.0);	// default color
			};
			DRAW_TRIANGLES.push_back(T);
			DRAW_NORMALS.push_back(N);
			DRAW_COLORS.push_back(C);
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