/* glut_functions.cc */

void glut_display(){
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  /* This is where the polyhedron is drawn. 
  	G is a global variable because glut_display can't
  	take an argument (because of glutDisplayFunc())  */
  
  G.draw_glut();
  
  glutSwapBuffers();
}

void reshape(int width, int height){
  glViewport(0, 0, width, height);
}

void idle(void){
  glutPostRedisplay();
}


void key(unsigned char key, int x, int y)
{
	switch (key) {
		case 'e':	// generate .eps
			G.sort_triangles();
			G.draw_eps();
			break;
		case 'w':	// write triangles to file (unsorted)
			G.write_triangles_to_file();
			break;
		case 'g':	// write group to file
			G.write_group_to_file();
			break;
		default:
			break;
	};
	return;
}

void mouse(int button, int state, int x, int y)
{
	return;
}

void specialkey(int key, int x, int y)
{
	switch (key) {
 		case GLUT_KEY_UP:
			glRotatef(5,1,0,0);
			break;
		case GLUT_KEY_DOWN:
			glRotatef(-5,1,0,0);
			break;
		case GLUT_KEY_RIGHT:
			glRotatef(5,0,0,1);
			break;
		case GLUT_KEY_LEFT:
			glRotatef(-5,0,0,1);
			break;
		default:
			break;
	}
	glutPostRedisplay();
}

void glut_setup(void){
	glutInitWindowSize(1000, 1000);
	glutInitWindowPosition(2,2);
	glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE | GLUT_DEPTH);
	glutCreateWindow("Kleinian");
	glViewport(0, 0, 1000, 1000);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(50,1,0.1,8);
	
	gluLookAt(0.01,0.01,2.5,0,0,0,0,0,1);			// looking from above
		// strangely fails when x and y coordinates are 0 instead of 0.01
	
	glClearDepth(1);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);

	glEnable(GL_COLOR_MATERIAL);

	glClearColor(0.9, 0.9, 0.99, 1.0);		// background very light blue
	
	glutDisplayFunc(glut_display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(key);
	glutSpecialFunc(specialkey);
	glutIdleFunc(idle);
	glutMouseFunc(mouse);
}