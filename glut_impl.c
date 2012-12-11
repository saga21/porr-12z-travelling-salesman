#include <stdio.h> // printf
#include <string.h> // strlen
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

	// Compute next generation
	evo_iter();
	// Increase counter
	++global_iteration_counter;

	// Every n'th iteration
	// TODO Change 100 to parameter
	if (global_iteration_counter%100 == 0) {
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
