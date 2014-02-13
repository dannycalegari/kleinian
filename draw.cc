/* draw.cc draw state */

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


void draw_klein_line(vec V, vec W){
	point p,q;
	p=vec_to_klein_point(V);
	q=vec_to_klein_point(W);
	draw_line(p,q,0);
};

void draw_poincare_line(vec V, vec W){
	point p,q,r;
	vec R;
	p=vec_to_poincare_point(V);
	q=vec_to_poincare_point(W);
	
	R=0.5*(V+W);	// actually, average
	r=vec_to_poincare_point(R);
	draw_line(p,r,0);
	draw_line(r,q,0);
/*	
	if(fabs(p.x-q.x)+fabs(p.y-q.y)<100){
		draw_line(p,q,0);
	} else {
		R=M+N;
		draw_poincare_line(M,R);
		draw_poincare_line(R,N);
	}; */
};

void draw_poincare_triangle(triangle T){
	triangle U;
	U.v[0]=(1.0/(T.v[0][3]+1.0))*T.v[0];
	U.v[1]=(1.0/(T.v[1][3]+1.0))*T.v[1];
	U.v[2]=(1.0/(T.v[2][3]+1.0))*T.v[2];
	
	point p,q,r;
	
	p.x=500 + (int) (400.0*U.v[0][0]);
	p.y=430 + (int) (400.0*U.v[0][1]);	
	q.x=500 + (int) (400.0*U.v[1][0]);
	q.y=430 + (int) (400.0*U.v[1][1]);
	r.x=500 + (int) (400.0*U.v[2][0]);
	r.y=430 + (int) (400.0*U.v[2][1]);
	
	long col;
	dbl a,b,c,m,n;
	int i;
		// coordinates of cross product
	a=((U.v[1][1]-U.v[0][1])*(U.v[2][2]-U.v[0][2]))-((U.v[2][1]-U.v[0][1])*(U.v[1][2]-U.v[0][2]));
	b=((U.v[2][0]-U.v[0][0])*(U.v[1][2]-U.v[0][2]))-((U.v[1][0]-U.v[0][0])*(U.v[2][2]-U.v[0][2]));
	c=((U.v[1][0]-U.v[0][0])*(U.v[2][1]-U.v[0][1]))-((U.v[2][0]-U.v[0][0])*(U.v[1][1]-U.v[0][1]));
	m=sqrt((a*a)+(b*b)+(c*c));
	n=fabs(c/m);
	i= (int) (n*255.0);
	col=(256*256*i)+(256*i)+i;
	if(T.v[0][3]+T.v[1][3]+T.v[2][3]>1000){
		col=256*256*255;
	};
	draw_triangle(p,q,r,col);
};

void draw_poincare_triangle_eps(triangle T, ofstream &output_file){
	triangle U;
	U.v[0]=(1.0/(T.v[0][3]+1.0))*T.v[0];
	U.v[1]=(1.0/(T.v[1][3]+1.0))*T.v[1];
	U.v[2]=(1.0/(T.v[2][3]+1.0))*T.v[2];
	
	
	long col;
	dbl a,b,c,m,n;
	int i;
		// coordinates of cross product
	a=((U.v[1][1]-U.v[0][1])*(U.v[2][2]-U.v[0][2]))-((U.v[2][1]-U.v[0][1])*(U.v[1][2]-U.v[0][2]));
	b=((U.v[2][0]-U.v[0][0])*(U.v[1][2]-U.v[0][2]))-((U.v[1][0]-U.v[0][0])*(U.v[2][2]-U.v[0][2]));
	c=((U.v[1][0]-U.v[0][0])*(U.v[2][1]-U.v[0][1]))-((U.v[2][0]-U.v[0][0])*(U.v[1][1]-U.v[0][1]));
	m=sqrt((a*a)+(b*b)+(c*c));
	n=fabs(c/m);
	
	i= (int) (n*255.0);
	col=(256*256*i)+(256*i)+i;
	if(T.v[0][3]+T.v[1][3]+T.v[2][3]>1000){
		output_file << "1 0 0 setrgbcolor \n";
	} else {
		output_file << n << " " << n << " " << n << " setrgbcolor\n";
	};
	output_file << U.v[0][0] << " " << U.v[0][1] << " moveto\n";
	output_file << U.v[1][0] << " " << U.v[1][1] << " lineto\n";
	output_file << U.v[2][0] << " " << U.v[2][1] << " lineto\n";
	output_file << "closepath \n";
	output_file << "fill \n";
	output_file << "stroke \n";
};

void kleinian_group::draw_state(){
	erase_field();
		
	point p;
	p.x=500;
	p.y=430;
	vector<triangle > draw_triangles;
	
	// draw Riemann sphere
	draw_circle(p,400,0);
	
	// draw back side limit set
	
	// draw interior edges and polygons
	
	int i,j;
	
	draw_triangles.clear();
	
	// generate list of triangles
	
	for(i=0;i<(int) VERTICES.size();i++){
		for(j=0;j<(int) TRIANGLES.size();j++){
			draw_triangles.push_back(CAMERA*VERTICES[i].M*TRIANGLES[j]);
		};
	};
	draw_triangles = sort(draw_triangles);
	
	for(i=0;i<(int) draw_triangles.size();i++){
		draw_poincare_triangle(draw_triangles[i]);
	};
	// draw front side limit set

	XFlush(display);
};

void kleinian_group::draw_glut(){
	dbl s;
	int i,j;
	vec U;
	vec A,B,C;
	
	if(draw_triangles_generated==false){
		draw_triangles.clear();

	
	// generate list of triangles
	
		for(i=0;i<(int) VERTICES.size();i++){
			for(j=0;j<(int) TRIANGLES.size();j++){
				draw_triangles.push_back(CAMERA*VERTICES[i].M*TRIANGLES[j]);
			};
		};
		draw_triangles_generated=true;
	};

    glColor3f(0.0f,1.0f,0.0f); 
	glBegin(GL_TRIANGLES);

	for(i=0;i<(int) draw_triangles.size();i++){
		s=draw_triangles[i].v[0][3]+1.0;
		A=(1.0/s)*draw_triangles[i].v[0];
		s=draw_triangles[i].v[1][3]+1.0;
		B=(1.0/s)*draw_triangles[i].v[1];
		s=draw_triangles[i].v[2][3]+1.0;
		C=(1.0/s)*draw_triangles[i].v[2];
		U=cross(B-A,C-A);
		glNormal3f(U[0], U[1], U[2]);			
		glVertex3f(A[0], A[1], A[2]);
		glNormal3f(U[0], U[1], U[2]);			
		glVertex3f(B[0], B[1], B[2]);
		glNormal3f(U[0], U[1], U[2]);			
		glVertex3f(C[0], C[1], C[2]);
	};
	glEnd();
	glutSwapBuffers();
};

void kleinian_group::draw_eps(){
	vector<triangle > draw_triangles;
	ofstream output_file;
	int i,j;
	
	draw_triangles.clear();
	
	// generate list of triangles
	
	for(i=0;i<(int) VERTICES.size();i++){
		for(j=0;j<(int) TRIANGLES.size();j++){
			draw_triangles.push_back(CAMERA*VERTICES[i].M*TRIANGLES[j]);
		};
	};
	draw_triangles = sort(draw_triangles);
	
	output_file.open("output.eps");
	
	output_file << "%!PS-Adobe-2.0 EPSF-2.0 \n";
	output_file << "%%BoundingBox: 0 0 300 300 \n";
	output_file << "gsave \n";
	output_file << "100 100 scale \n";
	output_file << "0 setlinewidth \n";
	output_file << "1.5 1.5 translate \n";
	output_file << "newpath \n";
	output_file << "0 0 1 0 360 arc \n";
	output_file << "stroke \n";

	output_file << "newpath \n";

	for(i=0;i<(int) draw_triangles.size();i++){
		draw_poincare_triangle_eps(draw_triangles[i], output_file);
	};
	output_file << "stroke \n";
	output_file << "grestore \n";
	output_file << "%eof \n";
	output_file.close();
};
