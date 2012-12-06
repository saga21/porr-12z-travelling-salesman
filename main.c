// Buttons:
// q/Esc - exit
// s - print summary
// i - print best and iteration
// r/spacebar - draw best

#include <stdlib.h> // srand
#include <time.h> // time
#include <omp.h>

// GLUT and OpenGL libraries
#include "globals.h" // DIM, ...
#include "evolution.h" // init

// ----------------------------------------------------------------------------

int main (int argc, char **argv) {

	// Set dynamic thread count
	omp_set_dynamic(1);
	// Set maximum thread count
	omp_set_num_threads(THREAD_COUNT);

	srand ( time(NULL) );
	
	// Init data
	init(argc, argv);
	
	while(true) {
		// Compute next generation
		evo_iter();
		
		// Increase counter
		++global_iteration_counter;

		// Every n'th iteration
		if (global_iteration_counter%500 == 0) {
			// Force to print population info
			print_summary_info(1);
		}
	}

	return 0;
}
