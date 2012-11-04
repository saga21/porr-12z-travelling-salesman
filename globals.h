#pragma once

// Default dimension of OpenGL Window
#define DIM 600
// Maximum coordinate of town
#define MAX_COORD 1000.0f

// Global iteration counter
unsigned long global_iteration_counter;

extern int best_index;
extern float best_value;

// Parameters from command line
int towns_count;
int mi_constant;
int m_constant;

struct town {
	float x;
	float y;
};

// Globals
struct town *towns; // []
int **population; // [][]
float **weights; // [][]
float *overall_lengths; // []
