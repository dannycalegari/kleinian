/* glut_functions.cc */


void glut_display(){

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
//  glEnable(GL_LIGHTING);
  
//  glShadeModel(GL_FLAT);
  
//  static GLfloat red[4] =
//    {1.0, 0.8, 0.8, 0.2};
//  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, red);
  
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
  //renderknot(K, &P, detail, WHITE);

  glutPostRedisplay();
}

void Init(void){

	glutInitWindowSize(1000, 1000);
	glutInitWindowPosition(2,2);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE | GLUT_DEPTH);
	glutCreateWindow("Kleinian");
	glViewport( 0, 0, 1000, 1000);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(50,1,0.01,30);
	
	glClear(GL_DEPTH_BUFFER_BIT);
	gluLookAt(0.1,0.1,2.5,0,0,0,0,0,1);

	
	glClearDepth(1);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);

	glEnable(GL_COLOR_MATERIAL);

	glClearColor(0.9, 0.9, 0.99, 1.0);
}