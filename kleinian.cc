/*	rmn.cc	

	Rational Map Navigator version 0.02

	September 7 2012

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

#include "glut_functions.cc";
#include "graphics.cc";
#include "draw.cc"
#include "read_write.cc";
#include "user_interface.cc";

int main(int argc, char *argv[]){ 
	
	ifstream input_file;
	
//	setup_graphics();
	G.initialize();
	G.generate_to_depth(11);
//	G.list_verts_and_edges();

	
	
	if(argc>1){
		input_file.open(argv[1]);
		G.read_from_file(input_file);
		input_file.close();
	} else {
		G.initialize();
	};

	glutInit(&argc, argv);
	Init();
	
	
	
	glutDisplayFunc(glut_display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(key);
	glutSpecialFunc(specialkey);
	glutIdleFunc(idle);
	glutMouseFunc(mouse);
  
  	G.draw_glut();
  	
	glutMainLoop();
        
    return(0);
  
	while(1){
		G.draw_state();
		G.user_interface();
	};
		
	
	return(0);
};
