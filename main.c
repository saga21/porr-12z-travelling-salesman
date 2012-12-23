// Buttons:
// q/Esc - exit
// s - print summary
// i - print best and iteration
// r/spacebar - draw best

//#define USE_OMP 1
#define USE_MPI 1
#include <stdlib.h> // srand
#include <time.h> // time

#ifdef USE_OMP	
	#include <omp.h>
#endif
#ifdef USE_MPI
	#include <mpi.h>
#endif

// GLUT and OpenGL libraries
#include <GL/glut.h>
#include <GL/gl.h>
#include "globals.h" // DIM, ...
#include "evolution.h" // init
#include "glut_impl.h" // display, reshape, ...

// ----------------------------------------------------------------------------

int main (int argc, char **argv) {

	srand ( time(NULL) );

#ifdef USE_OMP	
	// Set dynamic thread count
	omp_set_dynamic(1);
	// Set maximum thread count
	omp_set_num_threads(thread_count);
#endif
#ifdef USE_MPI
	//init wymagany
	MPI_Init(&argc,&argv);
	
	//pobierz id i iloœæ zadañ
	MPI_Comm_rank(MPI_COMM_WORLD, &mpi_node_id);
	MPI_Comm_size(MPI_COMM_WORLD, &mpi_node_count);

#endif
	
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
