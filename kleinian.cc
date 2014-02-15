/*	kleinian.cc	

	Kleinian group renderer version 0.01

	February 13 2014

	Copyright Danny Calegari

	released under the terms of the GNU GPL
	
*/

// standard libraries to include

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <math.h>
#include <vector>
#include <complex>
#include <sstream>
#include <limits>
#include <ctime>
#include <assert.h>
#include <GL/glut.h>
#include <GL/glu.h>

using namespace std;

// preprocessor definitions

#define debug 	1							//	debug is true
#define verbose 1							//	verbose is true by default
#define PI 		3.14159265358979
#define TWOPI	6.28318530717959
#define ACC		0.00000000001				// accuracy 1.0e-11
#define dbl		double						// double
#define vec 	vector<dbl >				// vector of numbers
#define mat		vector< vector<dbl > >		// matrix of complex
#define automaton 	vector< vector< pair< int, int > > >		// finite directed labeled graph

// global constants and structs

struct point{
	int x,y,z;
};

struct vertex{
	mat	M;	// matrix; vertex is last column of M
	int s;	// state of automaton
	int d;	// depth = word length in combing
};

struct edge{
	vec vi;
	vec vt;
};

struct triangle{
	vec v[3];
};



#include "algebra.cc";
#include "sort.cc";
#include "kleinian_group.cc";

kleinian_group G;

#include "generate.cc";
#include "glut_functions.cc";
#include "graphics.cc";
#include "draw.cc"
#include "read_write.cc";
#include "user_interface.cc";

int main(int argc, char *argv[]){ 
	
	ifstream input_file;
	string T,S = "";
	int depth;
	char c;
		
	if(argc>1){
		T=argv[1];	// should be flag
		if(T=="-t"){	// read file in triangle mode
			S=std::string(argv[2]);
			cout << "reading triangles from file " << S << "\n";
			input_file.open(S.c_str(), std::fstream::in);
			G.read_triangles_from_file(input_file);
			input_file.close();
			cout << "entering GLUT mode \n";
			G.MODE="GLUT";
		} else if(T=="-g"){		// read file in group mode
			S=std::string(argv[2]);
			cout << "reading generators from file " << S << "\n";
			input_file.open(S.c_str(), std::fstream::in);
			G.read_group_from_file(input_file);
			input_file.close();
			cout << "enter depth to generate to:";
			cin >> depth;
			cout << "generating triangles \n";
			G.generate_to_depth(depth);
			cout << "fancy curvilinear triangles (y/n)?:";
			cin >> c;
			if(c=='y'){
				G.generate_fancy=true;
				cout << "mesh size (should be between 0.01 and 0.2):";
				cin >> G.mesh;
			} else {
				G.generate_fancy=false;
			};
			cout << "entering GLUT mode \n";
			G.MODE="GLUT";
		} else if(T=="-c"){
			G.MODE="command";	// not implemented yet
			cout << "command mode not implemented yet \n";
		};
	} else {
		// interactive mode?
		G.example_initialize();	// go to default example
		cout << "enter depth to generate to:";
		cin >> depth;
		cout << "generating triangles \n";
		G.generate_to_depth(depth);
		cout << "fancy curvilinear triangles (y/n)?:";
		cin >> c;
		if(c=='y'){
			G.generate_fancy=true;
			cout << "mesh size (should be between 0.01 and 0.2):";
			cin >> G.mesh;
		} else {
			G.generate_fancy=false;
		};
		cout << "entering GLUT mode \n";
		G.MODE="GLUT";
	};

	if(G.MODE=="GLUT"){
		glutInit(&argc, argv);
		glut_setup();
		glutMainLoop();
	} else if(G.MODE=="X"){
		setup_graphics();  
		while(1){
			G.draw_X();
			G.X_user_interface();
		};
	} else if(G.MODE=="command"){
		// nothing yet
	};
	
	return(0);
};
