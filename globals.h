#pragma once

// Default dimension of OpenGL Window
#define DIM 600
// Maximum coordinate of town
#define MAX_COORD 1000.0f
#define DEFAULT_TOWNS 50
#define DEFAULT_MI_CONSTANT 5000
#define DEFAULT_M_CONSTANT 5000
#define DEFAULT_THREAD_COUNT 2

#define PRINT_EVERY_ITERS 100
#define TRANSFER_COUNT 5

#ifdef _WIN32
	
#else
	#define sprintf_s(buffer,size,...) snprintf (buffer,size, __VA_ARGS__)
#endif

#define M_MI (mi_constant+m_constant)

#ifdef USE_MPI
int mpi_node_id;
int mpi_node_count;
#endif

// Global iteration counter
unsigned long global_iteration_counter;
long global_start_time;
// Minimum main for loop execute time
extern int global_benchmark;

extern int best_index;
extern float best_value;

// Parameters from command line
int towns_count;
int mi_constant;
int m_constant;
int thread_count;

struct town {
	float x;
	float y;
};

// Globals
struct town *towns; // []
int **population; // [][]
float **weights; // [][]
float *overall_lengths; // []

float *overall_lengths_weights; // []
float overall_lengths_sum;
float overall_lengths_weights_sum;
int is_dirty;

long clock_ms();
int rand_my(unsigned int* seed);
