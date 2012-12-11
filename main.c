// Buttons:
// q/Esc - exit
// s - print summary
// i - print best and iteration
// r/spacebar - draw best

#include <stdlib.h> // srand
#include <time.h> // time
#include <omp.h>

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
	
	// Set dynamic thread count
	omp_set_dynamic(1);
	// Set maximum thread count
	omp_set_num_threads(thread_count);

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
