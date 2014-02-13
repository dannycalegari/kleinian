/* sort.cc	quicksort implementation for vector of vertex.
	each vertex has a component .M which is a 4x4 matrix.
	I want to sort these lexicographically by entries 
	ROUNDED DOWN to the nearest multiple of 0.0001 */
	
bool leq(mat A, mat B){		// approximate lexicographic ordering
	bool F;
	int i,j;
	bool diff;
	diff=false;
	F=true;
	i=0;
	while(i<4 && diff==false){
		j=0;
		while(j<4 && diff==false){
			if(A[i][j]<B[i][j]-0.0001){
				F=true;
				diff=true;
			} else if(A[i][j]>B[i][j]+0.0001){
				F=false;
				diff=true;
			};
			j++;
		};
		i++;
	};
	return(F);
};

bool leq(triangle U, triangle T){	// sort by normalized z coordinate of centroid
	dbl zU, zT;
	zU=(U.v[0][2]/(U.v[0][3]+1.0))+(U.v[1][2]/(U.v[1][3]+1.0))+(U.v[2][2]/(U.v[2][3]+1.0));
	zT=(T.v[0][2]/(T.v[0][3]+1.0))+(T.v[1][2]/(T.v[1][3]+1.0))+(T.v[2][2]/(T.v[2][3]+1.0));
	if(zU<=zT){
		return(true);
	} else {
		return(false);
	};
};

vector<vertex >  sort(vector<vertex > H){
	vector<vertex > L, G;
	vertex P;
	int i,j;
	
	if(H.size()<=1){	// base case of induction
		return(H);
	} else {
		i=(int) H.size();
		P=H[i/2];	// choose a pivot
		L.resize(0);
		G.resize(0);
		for(j=0;j<(int) H.size();j++){	// for each element of H
			if(j!=i/2){		// not the pivot!
				if(leq(H[j].M,P.M)){
					L.push_back(H[j]);
				} else {
					G.push_back(H[j]);
				};
			};
		};
		L=sort(L);	// sort left sublist
		G=sort(G);	// sort right sublist
		L.push_back(P);	// L becomes L+P
		L.insert( L.end(), G.begin(), G.end() );	// L+P becomes L+P+G
		return(L);	// return L+P+G
	};
};

vector<triangle > sort(vector<triangle > H){
	vector<triangle > L, G;
	triangle P;
	int i,j;
	
	if(H.size()<=1){	// base case of induction
		return(H);
	} else {
		i=(int) H.size();
		P=H[i/2];	// choose a pivot
		L.resize(0);
		G.resize(0);
		for(j=0;j<(int) H.size();j++){	// for each element of H
			if(j!=i/2){		// not the pivot!
				if(leq(H[j],P)){
					L.push_back(H[j]);
				} else {
					G.push_back(H[j]);
				};
			};
		};
		L=sort(L);	// sort left sublist
		G=sort(G);	// sort right sublist
		L.push_back(P);	// L becomes L+P
		L.insert( L.end(), G.begin(), G.end() );	// L+P becomes L+P+G
		return(L);	// return L+P+G
	};
};