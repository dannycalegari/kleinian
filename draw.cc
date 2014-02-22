/* draw.cc draw state */

void draw_klein_line(vec V, vec W){		// draw to X windows
	point p,q;
	p=vec_to_klein_point(V);
	q=vec_to_klein_point(W);
	draw_line(p,q,0);
};

void draw_poincare_line(vec V, vec W){		// draw to X windows
	point p,q,r;
	vec R;
	p=vec_to_poincare_point(V);
	q=vec_to_poincare_point(W);
	
	R=0.5*(V+W);	// actually, average
	r=vec_to_poincare_point(R);
	draw_line(p,r,0);
	draw_line(r,q,0);
};

void draw_X_triangle(triangle T, vec N){	// draw to X windows; N is normal (precomputed)
	point p,q,r;
	dbl n;
	int i;
	long col;

	p.x=500 + (int) (400.0*T.v[0][0]);
	p.y=430 + (int) (400.0*T.v[0][1]);	
	q.x=500 + (int) (400.0*T.v[1][0]);
	q.y=430 + (int) (400.0*T.v[1][1]);
	r.x=500 + (int) (400.0*T.v[2][0]);
	r.y=430 + (int) (400.0*T.v[2][1]);
	
	n=fabs(N[2]);	// should already be normalized
	i= (int) (n*255.0);
	col=(256*256*i)+(256*i)+i;
	draw_triangle(p,q,r,col);
};

void draw_eps_triangle(triangle T, vec N, ofstream &output_file){	// draw to .eps file
	dbl n;
	n=fabs(N[2]);	// should already be normalized
	output_file << n << " " << n << " " << n << " setrgbcolor\n";
	output_file << T.v[0][0] << " " << T.v[0][1] << " moveto\n";
	output_file << T.v[1][0] << " " << T.v[1][1] << " lineto\n";
	output_file << T.v[2][0] << " " << T.v[2][1] << " lineto\n";
	output_file << "closepath \n";
	output_file << "fill \n";
};

void kleinian_group::draw_X(){		// draw to X windows
	erase_field();
		
	point p;
	p.x=500;
	p.y=430;
	vector<triangle > draw_triangles;
	
	// draw Riemann sphere
	draw_circle(p,400,0);
	
	// draw back side limit set
	
	// draw interior edges and polygons
	
	int i;

	if(draw_triangles_generated==false){
		generate_triangles();
		sort_triangles();
		draw_triangles_generated=true;
	};
	
	for(i=0;i<(int) DRAW_TRIANGLES.size();i++){
		draw_X_triangle(DRAW_TRIANGLES[i],DRAW_NORMALS[i]);
	};
	// draw front side limit set

	XFlush(display);
};

void kleinian_group::draw_glut(){	// draw to GLUT
	int i;
	
	if(draw_triangles_generated==false){
		if(generate_fancy){
			fancy_generate_triangles();
		} else {
			generate_triangles();	
		};
		draw_triangles_generated=true;
	};

//	dbl r,g,b;
	
	/* no shimmer effect */
//    glColor3f(1.0f,0.9f,0.6f); 
	
	glBegin(GL_TRIANGLES);
	srand(0);
	for(i=0;i<(int) DRAW_TRIANGLES.size();i++){
	
	/*	shimmer effect
		r= 0.9 + ((dbl) (rand()) / (dbl) (RAND_MAX))/10.0;
		g= 0.9 + ((dbl) (rand()) / (dbl) (RAND_MAX))/10.0;
		b= 0.9 + ((dbl) (rand()) / (dbl) (RAND_MAX))/10.0;
    	glColor3f(r,g,b);	*/
	
		glNormal3f(DRAW_NORMALS[i][0], DRAW_NORMALS[i][1], DRAW_NORMALS[i][2]);
		if((int) DRAW_COLORS.size()>0){
			glColor3f(DRAW_COLORS[i][0], DRAW_COLORS[i][1], DRAW_COLORS[i][2]);
		} else {
			glColor3f(0.9,0.85,0.7);
		};
		glVertex3f(DRAW_TRIANGLES[i].v[0][0], DRAW_TRIANGLES[i].v[0][1], DRAW_TRIANGLES[i].v[0][2]);
		glVertex3f(DRAW_TRIANGLES[i].v[1][0], DRAW_TRIANGLES[i].v[1][1], DRAW_TRIANGLES[i].v[1][2]);
		glVertex3f(DRAW_TRIANGLES[i].v[2][0], DRAW_TRIANGLES[i].v[2][1], DRAW_TRIANGLES[i].v[2][2]);
	};
	glEnd();
	glutSwapBuffers();
};

void kleinian_group::draw_eps(){	// draw to .eps
	vector<triangle > draw_triangles;
	ofstream output_file;
	int i;
	
	if(draw_triangles_generated==false){
		generate_triangles();
		sort_triangles();
		draw_triangles_generated=true;
	};
	
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

	for(i=0;i<(int) DRAW_TRIANGLES.size();i++){
		draw_eps_triangle(DRAW_TRIANGLES[i],DRAW_NORMALS[i], output_file);
	};

	output_file << "stroke \n";
	output_file << "grestore \n";
	output_file << "%eof \n";
	output_file.close();
};
