/* read_write.cc	*/

/* several different file formats, depending on purpose

	filename.tri: list of triangles and normals, just for GLUT rendering and .eps output
	(no group to manipulate!)
	
	format: 
		First line is the number of triangles N
		For i = 0 to N-1:
			line 5i+2 to 5i+4 are xyz coordinates of vertices of triangle i
			line 5i+5 is xyz coordinates on normal to triangle i
			line 5i+6 is rgb colors of triangle i
		Last line is rgb background color
			
	filename.grp: list of semigroup generators for Kleinian group
	
	format:
		First line is number of semigroup generators N
		Second line is char c
		If c='n':	(for "numerical")
			For i = 0 to N-1:
				line 4i+3 to 4i+6 are matrix entries of G(i):=semigroup generator i
		If c='k':	(for "KAK format")
			For i = 0 to N-1:
				integer m (number of elementary generators of G(i))
				for j = 0 to m-1
					int a_j int b_j double l_j
				so that G(i) = build_mat(a_0,b_0,l_0)*. . . *build_mat(a_{m-1},b_{m-1},l_{m-1})
		Next line is number of triangle orbits M
			For i = 0 to M-1:
				Coordinates of vertices of triangle i
		Next line is 'c' for triangle colors
			For i = 0 to M-1:
				rgb colors of triangle i
		Next line is 'b' for background color
			rgb background color
*/

void kleinian_group::read_group_from_file(ifstream &input_file){		// read data from file
	GENERATORS.clear();	// initialize data
	mat M;
	vec V;
	int n,m,i,j,k;
	int a,b;
	dbl d;
	char c;
	triangle T;
	input_file >> n;	// number of generators
	cout << n << " generators\n";
	cout << "reading generators\n";
	input_file >> c;	// format 'n' for numerical or 'k' for KAK
	if(c=='n'){
		cout << "generators in numerical mode \n";
		for(i=0;i<n;i++){	// for each generator
			M.clear();
			for(j=0;j<4;j++){
				V.clear();
				for(k=0;k<4;k++){
					input_file >> d;
					V.push_back(d);
				};
				M.push_back(V);
			};
			GENERATORS.push_back(M);
		};
	} else if(c=='k'){
		cout << "generators in algebraic mode \n";
		for(i=0;i<n;i++){	// for each generator
			input_file >> m;	// number of elementary generators
			M=build_mat(0,1,0.0);	// identity matrix
			for(j=0;j<m;j++){
				input_file >> a >> b >> d;
				M=M*build_mat(a,b,d);
			};
			GENERATORS.push_back(M);
		};
	};
	TRIANGLES.clear();
	input_file >> m;	// number of triangle orbits
	cout << m << " triangles\n";
	for(i=0;i<m;i++){
		for(j=0;j<3;j++){
			V.clear();
			for(k=0;k<4;k++){
				input_file >> d;
				V.push_back(d);
			};
			T.v[j]=V;
		};
		TRIANGLES.push_back(T);
	};
	
	if(input_file.eof()==false){
		input_file >> c;	// color?
		if(c=='c'){
			COLORS.clear();
			for(i=0;i<m;i++){
				V.clear();
				for(k=0;k<3;k++){
					input_file >> d;
					V.push_back(d);
				};
				V.push_back(0.0);
				COLORS.push_back(V);
			};
		};
	};

	BACKGROUND_COLOR=build_vec(0.9, 0.9, 0.99, 1.0);	// default
	
	if(input_file.eof()==false){
		input_file >> c;	// background color?
		if(c=='b'){
			BACKGROUND_COLOR.clear();
			for(i=0;i<3;i++){
				input_file >> d;
				BACKGROUND_COLOR.push_back(d);
			};
			BACKGROUND_COLOR.push_back(0.0);
		};
	};
	
	AUTOMATON.clear();	// worst possible automaton
	vector< pair<int,int> > W;

	W.clear();	
	for(i=0;i<n;i++){
		W.push_back(make_pair(i,0));
	};
	AUTOMATON.push_back(W);
	
	draw_triangles_generated=false;
	do_prune=true;
	CAMERA=id_mat();		// camera skew angle; should be user specified
};

void kleinian_group::read_triangles_from_file(ifstream &input_file){
	DRAW_TRIANGLES.clear();
	DRAW_NORMALS.clear();
	DRAW_COLORS.clear();
	vec V;
	triangle T;
	int n,i,j,k;
	dbl d;
	input_file >> n;	// number of triangles
	for(i=0;i<n;i++){
		for(j=0;j<3;j++){	// read triangle j
			V.clear();
			for(k=0;k<3;k++){	// read triangle j vertex k
				input_file >> d;
				V.push_back(d);
			};
			V.push_back(0.0);	// dummy 4th variable just in case
			T.v[j]=V;
		};
		DRAW_TRIANGLES.push_back(T);
		V.clear();
		for(k=0;k<3;k++){	// read triangle j normal
			input_file >> d;
			V.push_back(d);
		};
		V.push_back(0.0);
		DRAW_NORMALS.push_back(V);
		V.clear();
		for(k=0;k<3;k++){	// read triangle j color
			input_file >> d;
			V.push_back(d);
		};
		V.push_back(0.0);
		DRAW_COLORS.push_back(V);
		V.clear();
	};
	draw_triangles_generated=true;
	BACKGROUND_COLOR.clear();
	for(k=0;k<3;k++){	// read background color
		input_file >> d;
		BACKGROUND_COLOR.push_back(d);
	};
	BACKGROUND_COLOR.push_back(0.0);
};


void kleinian_group::write_triangles_to_file(){		// write data to file
	// write triangle and normal data to file
	int i,j;
	ofstream output_file;
	
	output_file.open("output_triangles.tri");
	
	output_file << (int) DRAW_TRIANGLES.size() << "\n";	// how many triangles?
	
	for(i=0;i<(int) DRAW_TRIANGLES.size();i++){
		for(j=0;j<3;j++){
			output_file << DRAW_TRIANGLES[i].v[j][0] << " " << DRAW_TRIANGLES[i].v[j][1] << " "
				<< DRAW_TRIANGLES[i].v[j][2] << "\n";
		};
		output_file << DRAW_NORMALS[i][0] << " " <<  DRAW_NORMALS[i][1] << " "
			 << DRAW_NORMALS[i][2] << "\n";
		output_file << DRAW_COLORS[i][0] << " " << DRAW_COLORS[i][1] << " "
			<< DRAW_COLORS[i][2] << "\n";
	};
	output_file << BACKGROUND_COLOR[0] << " " << BACKGROUND_COLOR[1] << " " << BACKGROUND_COLOR[2] << "\n";
	output_file.close();	
};

void kleinian_group::write_group_to_file(){		// write group to file
	// always in "numerical" format
	int i,j,k;
	ofstream output_file;
	
	output_file.open("output_group.grp");
	
	output_file << (int) GENERATORS.size() << "\n";	// how many generators?
	
	output_file << 'n' << "\n";	// numerical format
	
	for(i=0;i<(int) GENERATORS.size();i++){
		for(j=0;j<4;j++){
			for(k=0;k<4;k++){
				output_file << GENERATORS[i][j][k] << " ";
			};
			output_file << "\n";
		};
	};
	
	output_file << (int) TRIANGLES.size() << "\n";	// how many triangle orbits?
	
	for(i=0;i<(int) TRIANGLES.size();i++){
		for(j=0;j<3;j++){
			for(k=0;k<4;k++){
				output_file << TRIANGLES[i].v[j][k] << " ";
			};
			output_file << "\n";
		};
	};
	
	output_file << 'c' << "\n";	// triangle colors
	for(i=0;i<(int) COLORS.size();i++){
		for(j=0;j<3;j++){
			output_file << COLORS[i][j] << " ";
		};
		output_file << "\n";
	};
	
	output_file << 'b' << "\n";	// background colors
	for(i=0;i<3;i++){
		output_file << BACKGROUND_COLOR[i] << " ";
	};
	output_file << "\n";
	
	output_file.close();
};