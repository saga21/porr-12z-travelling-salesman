
#include <stdio.h> // printf
#include <string.h> // strlen
#include <time.h>
// GLUT and OpenGL libraries
#include <GL/glut.h>
#include <GL/gl.h>

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

	// Clean drawing board
	glClear(GL_COLOR_BUFFER_BIT);

	// Draw outsite box
	glColor3f(0.1F, 0.80F, 0.1F);
	glLineWidth(1);
	glBegin(GL_LINE_LOOP);
	glVertex2f(-MAX_COORD, -MAX_COORD);
	glVertex2f( MAX_COORD, -MAX_COORD);
	glVertex2f( MAX_COORD,  MAX_COORD);
	glVertex2f(-MAX_COORD,  MAX_COORD);
	glEnd();

	// Draw population
	draw_best();
	
	glutSwapBuffers();
}

// ----------------------------------------------------------------------------

void keyboard(unsigned char key_code, int xpos, int ypos) {
	switch (key_code) {
		case 32: // Spacebar
			display();
			break;
		case 'i':
			print_population_info(1);
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
	
	static clock_t start_time; //TODO: Not initialized
	static unsigned long iters = 0;
	
	float ips;
	char buf[30];
	clock_t time;
	
	time = clock();
	
	if (time - start_time >= CLOCKS_PER_SEC) { // one second passed
		
		ips = (global_iteration_counter - iters) / ((float)(time - start_time) / CLOCKS_PER_SEC);
		sprintf_s(buf, 30, "Evo-salesman %6.0f IPS", ips);
		start_time = time;
		iters = global_iteration_counter;
		glutSetWindowTitle(buf);

	} // if

} // fps_window_title()

// ----------------------------------------------------------------------------

void idle(void) {

	// Compute next generation
	evo_iter();

	// Increase counter
	++global_iteration_counter;

	// Every n'th iteration
	// TODO Change 100 to parameter
	if (global_iteration_counter%100 == 0) {
		// Update main and sub window
		glutPostRedisplay(); //display();
		// Force to print population info
		print_population_info(1);
	}
	else {
		// Print only if changed
		print_population_info(0);
	}

	// Iterations per second
	ips_window_title();
}

// ----------------------------------------------------------------------------
