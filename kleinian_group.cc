/* kleinian_group.cc */

class kleinian_group{

	public:
		vector<mat > GENERATORS;		// semigroup generators
		automaton AUTOMATON;			// automaton defining a combing
		
		vector<vertex > VERTICES;	
		vector<edge > EDGES;			// orbit class of edges
		vector<triangle > TRIANGLES;	// orbit classes of triangles
		
		vector<triangle > DRAW_TRIANGLES;	// actual Euclidean triangles in R^3 to draw
		vector<vec > DRAW_NORMALS;			// normals to actual Euclidean triangles
		
		vector<triangle > draw_triangles;
		bool draw_triangles_generated;

		mat CAMERA;
		
		void initialize();
		void clever_prune_vertices(int dep);	// needs to be more clever
		void prune_vertices(int dep);	// removes redundant vertices of depth dep
		void generate_to_depth(int n);	// generate elements out to depth n using combing
		void list_verts_and_edges();	// output to cout list of vertices and edges to draw

		void generate_triangles();	
		void sort_triangles();		// sort by height - for .eps output
		
		void draw_X();		// draw to X windows
		void draw_glut();	// draw to GLUT
		void draw_eps();	// draw to eps
		
		void read_from_file(ifstream &);		// read data from file
		void write_to_file(ofstream &);		// write data to file
		void X_user_interface();			// top-level user interaction routine
};

void kleinian_group::initialize(){	// this is a hardcoded example; should make this interactive
	GENERATORS.clear();
	
	int i;
	mat a,b,A,B;
	
	a=build_mat(0.0,1.0,0.0,0.0, 0.0,0.0,1.0,0.0, 1.0,0.0,0.0,0.0, 0.0,0.0,0.0,1.0); // alpha
	b=build_mat(0.0,0.0,-1.0,0.0, 1.0,0.0,0.0,0.0, 0.0,-1.0,0.0,0.0, 0.0,0.0,0.0,1.0); // beta
	A=a*a;	// alpha^-1
	B=b*b;	// beta^-1
	
	// 11 nontrivial elements of A_4
	
	GENERATORS.push_back(a);	// gen 0
	GENERATORS.push_back(A);	// gen 1
	GENERATORS.push_back(b);	// gen 2
	GENERATORS.push_back(B);	// gen 3
	GENERATORS.push_back(a*b);	// gen 4
	GENERATORS.push_back(a*B);	// gen 5
	GENERATORS.push_back(b*a);	// gen 6
	GENERATORS.push_back(b*A);	// gen 7
	GENERATORS.push_back(A*b);	// gen 8
	GENERATORS.push_back(B*a);	// gen 9
	GENERATORS.push_back(a*b*a);	// gen 10
		
	dbl len;
	dbl ord;	
	mat x,y,z,X,Y,Z,m,M;
	
	ord=10.0;
	len=acosh((cos(PI/ord)/sqrt(3.0))/(sin(PI/ord)*sqrt(2.0)/sqrt(3.0)));
	
	x=build_mat(0,2,TWOPI/ord);
	y=build_mat(0,1,PI/4.0);
	z=build_mat(2,3,len);
	X=build_mat(2,0,TWOPI/ord);
	Y=build_mat(1,0,PI/4.0);
	Z=build_mat(2,3,-1.0*len);
	
	m=y*z*x*Y*Z;
	M=y*z*X*Y*Z;
	
	// rotation of order ord
	
	GENERATORS.push_back(m);
	GENERATORS.push_back(M);

	AUTOMATON.clear();
	vector< pair<int,int> > V;

	// state 0
	V.clear();	
	for(i=0;i<13;i++){
		V.push_back(make_pair(i,0));
	};
	AUTOMATON.push_back(V);
	
	
	EDGES.clear();
	edge e;
	e.vi=build_vec(0.0,0.0,0.0,1.0);
	e.vt=m*build_vec(0.0,0.0,0.0,1.0);
	EDGES.push_back(e);
	
	TRIANGLES.clear();
	triangle T;
	T.v[0]=z*build_vec(0.0,0.0,0.0,1.0);
	T.v[1]=build_vec(0.0,0.0,0.0,1.0);
	T.v[2]=m*build_vec(0.0,0.0,0.0,1.0);
	TRIANGLES.push_back(T);
	
	CAMERA=build_mat(2,3,-0.5)*build_mat(0,2,0.7);	// camera skew angle
	
	draw_triangles_generated=false;
};

void kleinian_group::prune_vertices(int dep){
	int i,j;
	for(i=0;i< (int) VERTICES.size();i++){
		for(j=0;j<i;j++){
			if(VERTICES[i].d==dep){	// don't prune the same vertex twice
				if(norm(VERTICES[i].M+(-1.0*VERTICES[j].M))<0.01){	// if i and j are too close
					VERTICES.erase(VERTICES.begin()+i);	// prune vertex i
					i--;
				};
			};
		};
	};
};

void kleinian_group::clever_prune_vertices(int dep){
	int i;
	cout << "sorting vertices \n";
	VERTICES=sort(VERTICES);
	cout << "pruning vertices \n";
	vector<vertex > V;
	V.clear();
	V.push_back(VERTICES[0]);
	for(i=1;i< (int) VERTICES.size();i++){
		if(norm(VERTICES[i].M+(-1.0*VERTICES[i-1].M))>=0.01){	// if i and i-1 are not too close
			V.push_back(VERTICES[i]);
//			VERTICES.erase(VERTICES.begin()+i);	// prune vertex i
//			i--;
		};
	};
	VERTICES=V;
};

void kleinian_group::generate_to_depth(int n){
	int i,j,dep;
	int s,t;
	vertex X;
	edge E;
	mat M;

	VERTICES.clear();	// initialize;
	
	for(dep=0;dep<=n;dep++){	
		if(dep==0){	// base case for induction
			X.M=id_mat();	// base matrix is identity
			X.s=0;			// base state is 0
			X.d=0;			// base depth is 0
			VERTICES.push_back(X);
		} else {	
			for(i=0;i< (int) VERTICES.size();i++){	// for each element in the list,
				if(VERTICES[i].d==dep-1){	// is this a terminal vertex?
					s=VERTICES[i].s;	// s is state
					for(j=0;j<(int) AUTOMATON[s].size();j++){
						M=GENERATORS[AUTOMATON[s][j].first];	// transition matrix
						t=AUTOMATON[s][j].second;		// end state
						X.M=VERTICES[i].M*M;		// new vertex location
						X.s=t;						// new state
						X.d=dep;					// new depth
						VERTICES.push_back(X);		// add vertex to collection
					};
				};
			};
		};
		cout << "depth " << dep << ";  " << (int) VERTICES.size() << " vertices\n"; 
		clever_prune_vertices(dep);	// we'll see
		cout << "after pruning,  " << (int) VERTICES.size() << " vertices\n"; 
	};
	return;
};

void kleinian_group::list_verts_and_edges(){
	int i;
	cout << (int) VERTICES.size() << " vertices \n";
	for(i=0;i<(int) VERTICES.size();i++){
		cout << "vertex " << i << "\n";
		write(VERTICES[i].M);	// write matrix
		cout << "state:" << VERTICES[i].s << "  depth:" << VERTICES[i].d << "\n"; 
	};
	cout << (int) EDGES.size() << " edges \n";
	for(i=0;i<(int) EDGES.size();i++){
		cout << "edge " << i << "\n";
		write(EDGES[i].vi);
		write(EDGES[i].vt);
	};
	return;
};