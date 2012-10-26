#include <stdlib.h> // rand
#include <stdio.h> // printf
#include <time.h> // time
#include <math.h> // sqrt
#include <string.h>

//TODO: Remove sleep function
#ifdef _WIN32
	#include <windows.h> // Sleep
#else
	#include <sys/time.h>
	#define sprintf_s(buffer,size,...) snprintf (buffer,size, __VA_ARGS__)
	

#endif

// Default dimension of OpenGL Window
#define DIM 600
// Maximum coordinate of town
#define MAX_COORD 1000.0f

// GLUT and OpenGL libraries
#include <GL/glut.h>
#include <GL/gl.h>
#include "evolutionLib.h"


// ----------------------------------------------------------------------------

void drawString (char *s)
{
	unsigned int i;
	for (i = 0; i < strlen (s); i++)
		glutBitmapCharacter (GLUT_BITMAP_HELVETICA_10, s[i]);
}

// ----------------------------------------------------------------------------

void generate_population() {

	int i, k, j, temp;

	population = (int**)malloc(mi_constant * sizeof(int*));

	for (i = 0; i < mi_constant; i++) {	

		population[i] = (int*)malloc(towns_count * sizeof(int));

		for (k = 0; k < towns_count; k++)
			population[i][k] = k;
	    
		for (k = towns_count-1; k > 0; k--) {
			j = rand() % (k+1);
			temp = population[i][j];
			population[i][j] = population[i][k];
			population[i][k] = temp;
		}
	}
}

// ----------------------------------------------------------------------------

float calculate_weight(int i, int j) {
	
	return sqrt( pow(towns[i].x - towns[j].x, 2) + 
		pow(towns[i].y - towns[j].y, 2) );
}

// ----------------------------------------------------------------------------

float calculate_best() {

	int i;
	float v = 0.0;

	// Sum of weights
	for(i = 0; i < towns_count-1; i++)
		v += calculate_weight(population[best_index][i], population[best_index][i+1]);
	
	return v;
}
// ----------------------------------------------------------------------------

void draw_best() {

	int i;
	static char label[100];

	glColor3f(1.0f, 0.80f, 0.1f);
	glLineWidth(1);
	glBegin(GL_LINE_LOOP);
	for(i = 0; i < towns_count; i++) {
		glVertex2f(towns[population[best_index][i]].x,
			towns[population[best_index][i]].y);
	}
	glEnd();
	
	// Axis Labels
	glColor3f (1.0F, 1.0F, 1.0F);
	sprintf_s(label, 100, "Best value: %f Iteration: %lu", calculate_best(), global_iteration_counter);
	glRasterPos2f (-MAX_COORD, -MAX_COORD*1.05);
	drawString (label);

	//TODO remove this line, now it is for illustrate animation
	best_index = ++best_index % mi_constant;
}

// ----------------------------------------------------------------------------

void destroy_population() {
	
	int i;

	for (i = 0; i < mi_constant; i++)
		free(population[i]);
	
	free(population);
}

// ----------------------------------------------------------------------------

void print_best() {
	
	int i;
	float v = 0.0;
	float t = 0.0;

	fprintf(stderr, "%f [%d]", calculate_best(), best_index);
	for(i = 0; i < towns_count; i++)
		fprintf(stderr, " %d", population[best_index][i]);
	
	fprintf(stderr, "\n");
}

// ----------------------------------------------------------------------------

void print_population_info() {

	fprintf(stderr, "Iteration %lu\n", global_iteration_counter);
	print_best();
	return;
}

// ----------------------------------------------------------------------------

void generate_weight_matrix() {
	
	int i, j;
	float tmp;

	// Allocate memory
	weights = (float**)malloc(towns_count * sizeof(float*));
	
	for (i = 0; i < towns_count; i++)
		weights[i] = (float*)malloc(towns_count * sizeof(float));

	for (i = 0; i < towns_count; i++) {	
		
		for (j = 0; j < towns_count; j++) {
			tmp = calculate_weight(i, j);
			weights[i][j] = tmp;
			weights[j][i] = tmp;
		} // for j
	} // for i
}

// ----------------------------------------------------------------------------

void destroy_weight_matrix() {
	
	int i;

	for (i = 0; i < towns_count; i++)
		free(weights[i]);
		
	free(weights);
}

// ----------------------------------------------------------------------------

void generate_towns() {

	int i;
	
	// Allocate memory
	towns = (struct town*)malloc(sizeof(struct town)*towns_count);

	// Generate towns
	srand(time(NULL));
	for (i = 0; i < towns_count; i++) {
		towns[i].x = - MAX_COORD + (float)rand()/((float)RAND_MAX/MAX_COORD/2);
		towns[i].y = - MAX_COORD + (float)rand()/((float)RAND_MAX/MAX_COORD/2);
	}
}

// ----------------------------------------------------------------------------

void destroy_towns() {

	free(towns);
}

// ----------------------------------------------------------------------------

void init(int argc, char **argv) {
		
	towns_count = 0;
	mi_constant = 0;
	m_constant = 0;

	// Process execute parameters
	if (argc == 4) {
		towns_count = atoi(argv[1]);
		mi_constant = atoi(argv[2]);
		m_constant = atoi(argv[3]);
	}
	else {
		fprintf(stderr, "argc: %d\n", argc);
	}
	if (towns_count==0) towns_count = 30;
	if (mi_constant==0) mi_constant = 1000;
	if (m_constant==0) m_constant = 10;
	
	generate_towns();

	// Generate connectivity weight matrix
	generate_weight_matrix();

	// Generate population
	generate_population();
	
	// Reset iteration counter
	global_iteration_counter = 0;

} // init()

// ----------------------------------------------------------------------------

void terminate() {
	print_population_info();
	fprintf(stderr, "Quiting");
	destroy_towns();
	destroy_weight_matrix();
	destroy_population();
	fprintf(stderr, ".\n");
	exit(0);
} // terminate()

// ----------------------------------------------------------------------------

void reshape (int w, int h) {
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

void display (void) {
	// Clean drawing board
	glClear (GL_COLOR_BUFFER_BIT);

	// Draw outsite box
	glColor3f (0.1F, 0.80F, 0.1F);
	glLineWidth (1);
	glBegin (GL_LINE_LOOP);
	glVertex2f (-MAX_COORD, -MAX_COORD);
	glVertex2f ( MAX_COORD, -MAX_COORD);
	glVertex2f ( MAX_COORD,  MAX_COORD);
	glVertex2f (-MAX_COORD,  MAX_COORD);
	glEnd ();
	
	// Draw towns
	/*glLineWidth (1);
	for (i = 0; i < towns_count; i++) {
		glBegin(GL_LINE_LOOP);
		glVertex2f(towns[i].x-1, towns[i].y-1);
		glVertex2f(towns[i].x-1, towns[i].y+1);
		glVertex2f(towns[i].x+1, towns[i].y+1);
		glVertex2f(towns[i].x+1, towns[i].y-1);
		glEnd();
	}*/

	// Draw population
	draw_best();
	
	glutSwapBuffers();
}

// ----------------------------------------------------------------------------

void keyboard (unsigned char key_code, int xpos, int ypos) {
	switch (key_code) {
		case 32: // Spacebar
			display();
			break;
		case 'i':
			print_population_info();
			break;
		case 'q':
		case 'Q':
		case 27 : // Esc
			glFinish();
			terminate();
		}
}

// ----------------------------------------------------------------------------

void evo_iter(void) {
	//TODO Evolve - Iteration step
	
}

// ----------------------------------------------------------------------------

void idle (void) {
	// Next generation
	evo_iter();

	//TODO - remove this sleep functions
#ifdef _WIN32
	Sleep(10); // ms
#else
	sleep(1);
#endif

	// Every n'th iteration
	if (global_iteration_counter++%100 == 0) {
		// Update main and sub window
		glutPostRedisplay(); //display();
		print_population_info();
	}
};

// ----------------------------------------------------------------------------

int main (int argc, char **argv) {	
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
