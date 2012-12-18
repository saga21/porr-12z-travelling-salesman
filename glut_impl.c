#include <stdio.h> // printf
#include <string.h> // strlen
#include <stdlib.h> // malloc?
// GLUT and OpenGL libraries
#include <GL/glut.h>
#include <GL/gl.h>

// TODO: Dlaczego musze jeszcze raz uzyc define, skoro jest juz w main?
#define USE_MPI 1

#ifdef USE_MPI
#include <mpi.h>
#define MPI_NEXT_NODE (mpi_node_id+1)%mpi_node_count
#define MPI_PREV_NODE (mpi_node_id+mpi_node_count-1)%mpi_node_count
#endif

#include "globals.h" // MAX_COORD
#include "evolution.h" // evo_iter, print_population_info
#include "glut_impl.h" // This header

// ----------------------------------------------------------------------------

void drawString(char *s) {
	unsigned int i;
	for (i = 0; i < strlen(s); i++)
		glutBitmapCharacter (GLUT_BITMAP_HELVETICA_10, s[i]);
}

// ----------------------------------------------------------------------------

void draw_best() {

	int i;
	static char label[100];

	find_best();

	glColor3f(1.0f, 0.8f, 0.2f);
	glLineWidth(1);
	glBegin(GL_LINE_LOOP);
	for(i = 0; i < towns_count; i++) {
		glVertex2f(towns[population[best_index][i]].x,
			towns[population[best_index][i]].y);
	}
	glEnd();
	
	// Axis Labels
	glColor3f (1.0F, 1.0F, 1.0F);
	sprintf_s(label, 100, "Best value: %f Iteration: %lu", best_value, global_iteration_counter);
	glRasterPos2f (-MAX_COORD, -MAX_COORD*1.05);
	drawString(label);

}

// ----------------------------------------------------------------------------

void reshape(int w, int h) {
	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity ();
	glViewport (0, 0, w, h);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	gluOrtho2D (-MAX_COORD*1.1, MAX_COORD*1.1, -MAX_COORD*1.1, MAX_COORD*1.1);
	glEnable (GL_LINE_SMOOTH);
	glEnable (GL_LINE_STIPPLE);
}

// ----------------------------------------------------------------------------

void display(void) {
	
	int i;

	// Clean drawing board
	glClear(GL_COLOR_BUFFER_BIT);

	// Draw outsite box
	glColor3f(0.1f, 0.8f, 0.1f);
	glLineWidth(1);
	glBegin(GL_LINE_LOOP);
	glVertex2f(-MAX_COORD, -MAX_COORD);
	glVertex2f( MAX_COORD, -MAX_COORD);
	glVertex2f( MAX_COORD,  MAX_COORD);
	glVertex2f(-MAX_COORD,  MAX_COORD);
	glEnd();
	
	// Draw population
	draw_best();

	// Draw towns
	glColor3f(1.0f, 0.2f, 0.2f);
	for(i = 0; i < towns_count; i++) {
		glBegin(GL_LINE_LOOP);
		glVertex2f(towns[i].x+5, towns[i].y+5);
		glVertex2f(towns[i].x+5, towns[i].y-5);
		glVertex2f(towns[i].x-5, towns[i].y-5);
		glVertex2f(towns[i].x-5, towns[i].y+5);
		glEnd();
 	}
	
	glutSwapBuffers();
}

// ----------------------------------------------------------------------------

void keyboard(unsigned char key_code, int xpos, int ypos) {
	switch (key_code) {
		case 'r':
		case 'R':
		case 32: // Spacebar
			display();
			break;
		case 'i':
		case 'I':
			print_population_info(1);
			break;
		case 's':
		case 'S':
			print_summary_info(1);
			break;
		case 'q':
		case 'Q':
		case 27 : // Esc
			glFinish();
			terminate();
		}
}

// ----------------------------------------------------------------------------

void ips_window_title(void) {

	static long start_time = 0;
	static unsigned long iters = 0;
	
	long time;
	float ips;
	char buf[30];
	
	if (iters == 0) {
		global_start_time = clock_ms();
	}

	time = clock_ms();

	if (time - start_time >= 1000) { // one second passed
		
		ips = (float)(global_iteration_counter - iters) / (time - start_time) * 1000;
		sprintf_s(buf, 30, "Evo-salesman %6.2f IPS", ips);
		start_time = time;
		iters = global_iteration_counter;
		glutSetWindowTitle(buf);

	} // if

} // fps_window_title()

// ----------------------------------------------------------------------------

void idle(void) {
	
#ifdef USE_MPI
	int *cities_array;
	int recv_flag;
	MPI_Status status;
#endif

	// Compute next generation
	evo_iter();
	// Increase counter
	++global_iteration_counter;

	// Every n'th iteration
	if (global_iteration_counter%PRINT_EVERY_ITERS == 0) {
		// Force to print population info
		print_population_info(1);
	}
	else {
		// Print only if changed
		print_population_info(0);
	}

	// Iterations per second
	ips_window_title();

#ifdef USE_MPI

	//******** Sending ********//
	// If not sending to itself
	if (mpi_node_id != MPI_NEXT_NODE && (rand_my(0)%SEND_EVERY_ITER)==0) {
		
		// Alloc buffer
		cities_array = (int*)malloc(TRANSFER_COUNT * towns_count * sizeof(int));
		
		// TODO: Prepare cities_array to send from best of population
		cities_array[0] = 123456789;
		cities_array[1] = mpi_node_id;
		cities_array[2] = rand_my(0)%10;

		// Debug what we send
		printf("Node %d sending (to next %d) buffer: %d, %d, %d ...\n", 
			mpi_node_id, MPI_NEXT_NODE, cities_array[0], cities_array[1], cities_array[2]);
		
		// Blocking sending
		MPI_Send((void*)cities_array, TRANSFER_COUNT * towns_count, MPI_INT, MPI_NEXT_NODE, 0, MPI_COMM_WORLD);

		// Free buffer
		free(cities_array);
	}
	
	//******** Receiving ********//
	// Test if can receive message
	MPI_Iprobe(MPI_PREV_NODE, 0, MPI_COMM_WORLD, &recv_flag, &status);
	// If can receive
	if (recv_flag) {

		// Alloc buffer
		cities_array = (int*)malloc(TRANSFER_COUNT * towns_count * sizeof(int));

		// Blocking receive
		MPI_Recv((void*)cities_array, TRANSFER_COUNT * towns_count, MPI_INT, MPI_PREV_NODE, 0, MPI_COMM_WORLD, &status);

		// Print what've received
		printf("Node %d received (from prev: %d) buffer: %d, %d, %d ...\n",
			mpi_node_id, MPI_PREV_NODE, cities_array[0], cities_array[1], cities_array[2]);
	
		//TODO: Insert to population data from received cities_array

		// Free buffer
		free(cities_array);
	}
#endif
}

// ----------------------------------------------------------------------------
