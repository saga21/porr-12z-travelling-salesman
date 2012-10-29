#pragma once
//#ifndef GLOBALS_H
//#define GLOBALS_H

#include <float.h> // FLT_MAX

// Default dimension of OpenGL Window
#define DIM 600
// Maximum coordinate of town
#define MAX_COORD 1000.0f

#define M_MI (mi_constant+m_constant)

// Global iteration counter
unsigned long global_iteration_counter;

// TODO: why do we need static here?
// It won't compile if not static
// Compiler says these values are already defined in main.obj and mechanics.obj
// If static, OpenGL draw wrong value
static int best_index = 0;
static float best_value = FLT_MAX;

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

//#endif
