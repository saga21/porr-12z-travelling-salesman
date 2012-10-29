#include <stdlib.h> // srand
//#include <stdio.h> // printf
#include <time.h> // time

#ifdef _WIN32

#else
	#define sprintf_s(buffer,size,...) snprintf (buffer,size, __VA_ARGS__)
#endif

// GLUT and OpenGL libraries
#include <GL/glut.h>
#include <GL/gl.h>
#include "globals.h" // DIM, ...
#include "mechanics.h" // init
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
