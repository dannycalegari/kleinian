/* algebra.cc */

// algebraic operations with matrices

// we are especially interested in matrices in O(3,1)

mat id_mat(){	// 4x4 identity matrix
	mat M;
	vec V;
	int i,j;
	M.clear();
	for(i=0;i<4;i++){
		V.clear();
		for(j=0;j<4;j++){
			if(i==j){
				V.push_back(1.0);
			} else {
				V.push_back(0.0);
			};
		};
		M.push_back(V);
	};
	return(M);
};

mat build_mat(dbl a00, dbl a01, dbl a02, dbl a03, dbl a10, dbl a11, dbl a12, dbl a13,
	dbl a20, dbl a21, dbl a22, dbl a23, dbl a30, dbl a31, dbl a32, dbl a33){	// direct entries
	mat M;
	M=id_mat();	// initialize to identity matrix
	M[0][0]=a00;
	M[0][1]=a01;
	M[0][2]=a02;
	M[0][3]=a03;
	M[1][0]=a10;
	M[1][1]=a11;
	M[1][2]=a12;
	M[1][3]=a13;
	M[2][0]=a20;
	M[2][1]=a21;
	M[2][2]=a22;
	M[2][3]=a23;
	M[3][0]=a30;
	M[3][1]=a31;
	M[3][2]=a32;
	M[3][3]=a33;
	return(M);
};

vec build_vec(dbl a0, dbl a1, dbl a2, dbl a3){
	vec V;
	V.clear();
	V.push_back(a0);
	V.push_back(a1);
	V.push_back(a2);
	V.push_back(a3);
	return(V);
};

mat build_mat(int a, int b, dbl alpha){	// rotation through angle alpha in the (a,b)-plane
	mat M;
	M=id_mat();	// initialize to identity matrix
	if(a!=b){
		if((a-3)*(b-3)!=0){		// spacelike rotation
			M[a][a]=cos(alpha);
			M[a][b]=sin(alpha);
			M[b][a]=-sin(alpha);
			M[b][b]=cos(alpha);
		} else {				// Lorentz rotation
			M[a][a]=cosh(alpha);
			M[a][b]=sinh(alpha);
			M[b][a]=sinh(alpha);
			M[b][b]=cosh(alpha);
		};
	} else {
		cout << "Index incompatibility! " << a << " = " << b << "\n";
	};
	return(M);
};

void write(mat M){	// writes M to cout
	int i,j;
	for(i=0;i<(int) M.size();i++){
		cout << "( ";
		for(j=0;j<(int) M[i].size();j++){
			cout << M[i][j] << " ";
		};
		cout << " )\n";
	};
	cout << "\n";
};

void write(vec V){	// writes V to cout
	int i;
	cout << "( ";
	for(i=0;i<(int) V.size(); i++){
		cout << V[i] << " ";
	};
	cout << " )\n";
};

mat operator*(mat A, mat B){		// returns A*B
	mat M;
	vec V;
	dbl z;
	int i,j,k;

	if(A[0].size() == B.size()){
		M.clear();
		for(i=0;i< (int) A.size();i++){
			V.clear();
			for(j=0;j< (int) B[0].size();j++){
				z=0.0;
				for(k=0;k< (int) A[0].size();k++){
					z=z+(A[i][k]*B[k][j]);
				};
				V.push_back(z);
			};
			M.push_back(V);
		};
	} else {
		cout << "Matrix multiplication size incompatibility!\n";
		cout << "A has size " << (int) A.size() << " by " << (int) A[0].size() << "\n";
		cout << "B has size " << (int) B.size() << " by " << (int) B[0].size() << "\n";
	};
	return(M);
};

vec operator*(mat A, vec v){	// returns A*v
	vec w;
	int i,k;
	
	w=v;
	for(i=0;i< (int) A.size();i++){
		w[i]=0.0;
		for(k=0;k< (int) A[0].size();k++){
			w[i]=w[i]+(A[i][k]*v[k]);
		};
	};
	return(w);
};

triangle operator*(mat A, triangle T){
	triangle U;
	U.v[0]=A*T.v[0];
	U.v[1]=A*T.v[1];
	U.v[2]=A*T.v[2];
	return(U);
};

mat operator+(mat A, mat B){	// returns A+B
	mat M;
	int i,j;
	M=A;
	if(A.size()==B.size() && A[0].size() == B[0].size()){
		for(i=0;i<(int) A.size();i++){	
			for(j=0;j<(int) A[i].size();j++){
				M[i][j]=(M[i][j]+B[i][j]);
			};
		};
	} else {
		cout << "Matrix addition size incompatibility!\n";
		cout << "A has size " << (int) A.size() << " by " << (int) A[0].size() << "\n";
		cout << "B has size " << (int) B.size() << " by " << (int) B[0].size() << "\n";	
	};
	return(M);
};

vec operator+(vec V, vec W){	// returns V+W
	vec U;
	int i;
	U=V;
	for(i=0;i<(int) V.size();i++){
		U[i]=V[i]+W[i];
	};
	return(U);
};

vec operator-(vec V, vec W){	// returns V-W
	vec U;
	U=V;
	int i;
	for(i=0;i<(int) V.size();i++){
		U[i]=V[i]-W[i];
	};
	return(U);
};

mat operator*(dbl t, mat A){	// returns t*A
	mat M;
	int i,j;
	M=A;
	for(i=0;i<(int) A.size();i++){	
		for(j=0;j<(int) A[i].size();j++){
			M[i][j]=t*M[i][j];
		};
	};
	return(M);
};

vec operator*(dbl t, vec V){	// returns t*V
	vec W;
	int i;
	W=V;
	for(i=0;i<(int) V.size();i++){
		W[i]=t*W[i];
	};
	return(W);
};

vec cross(vec V, vec W){
	vec U;
	U=V;
	U[0]=(V[1]*W[2])-(V[2]*W[1]);
	U[1]=(W[0]*V[2])-(W[2]*V[0]);
	U[2]=(V[0]*W[1])-(V[1]*W[0]);
	U[3]=0.0;
	if(U[2]<0.0){
		U=-1.0*U;
	};
	return(U);
};

dbl norm(mat A){	// returns the sum of the absolute values of the coefficients
	dbl t;
	t=0.0;
	int i,j;
	for(i=0;i<(int) A.size();i++){
		for(j=0;j<(int) A[i].size();j++){
			t=t+fabs(A[i][j]);
		};
	};
	return(t);
};

dbl entry_sum(mat A){	// returns the sum of the entries of the coefficients
	dbl t;
	t=0.0;
	int i,j;
	for(i=0;i<(int) A.size();i++){
		for(j=0;j<(int) A[i].size();j++){
			t=t+fabs(A[i][j]);
		};
	};
	return(t);
};

void test_mat_routine(){	// routine to test matrix functions
	mat A,B,C;
	
	A=build_mat(0,3,PI/3.0);
	B=build_mat(3,0,PI/3.0);
	C=build_mat(0,3,-PI/3.0);
	
	write(A);
	write(B);
	write(C);
	write(A*B);
	write(A*C);
	write(B*C);
	write(A*B*C*A*B);
	
	return;
};
