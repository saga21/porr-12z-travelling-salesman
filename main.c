#include <stdlib.h> // srand
//#include <stdio.h> // printf
#include <time.h> // time

// GLUT and OpenGL libraries
#include <GL/glut.h>
#include <GL/gl.h>
#include "globals.h" // DIM, ...
#include "evolution.h" // init
#include "glut_impl.h" // display, reshape, ...

// ----------------------------------------------------------------------------

int main (int argc, char **argv) {	
	
	srand ( time(NULL) );
	
	// Init data
	init(argc, argv);

	// Glut initializations
	glutInit (&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition (100, 100);
	glutInitWindowSize (DIM, DIM);

	// Main window creation and setup
	glutCreateWindow ("Evo-salesman");
	glutDisplayFunc    (display);
	glutReshapeFunc   (reshape);
	glutKeyboardFunc (keyboard);
	glutIdleFunc         (idle);
	
	//fprintf(stderr, "Init success\n");

	glutMainLoop();

	return 0;
}
