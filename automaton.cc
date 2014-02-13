/* automaton.cc routine for finite state automaton */

// #define automaton vector< vector< pair< int, int > > >;

	/* An automaton V is a directed graph whose vertices are the states
	and whose edges are labeled by elements of a finite alphabet. We encode
	it by a vector of vectors of pairs of integers. */
	
	
	/* V[i] is the vector of outgoing edges at vertex i; */
	/* V[i][j] is edge j outgoing from vertex i */
	/* V[i][j].first is the label of edge j from vertex i; this is a generator */
	/* V[i][j].second is the "to" vertex of edge j from vertex i; this is a state */	
