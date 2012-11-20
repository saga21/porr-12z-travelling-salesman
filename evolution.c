#include <stdlib.h> // rand
#include <stdio.h> // printf
#include <time.h> // time
#include <math.h> // sqrt
#include <string.h>
#include <float.h> // FLT_MAX

#include "globals.h"
#include "roulette.h"
#include "qsortPopulation.h"
#include "evolutionLib.h"
#include "evolution.h" // This header

// ----------------------------------------------------------------------------

void generate_population() {

	int i, k, j, temp;

	population = (int**)malloc(M_MI * sizeof(int*));

	for (i = 0; i < M_MI; i++) {	

		population[i] = (int*)malloc(towns_count * sizeof(int));

		for (k = 0; k < towns_count; k++){
			population[i][k] = k;
		}
	    
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
	
	return sqrtf( (towns[i].x - towns[j].x)*(towns[i].x - towns[j].x) + 
		(towns[i].y - towns[j].y)*(towns[i].y - towns[j].y) );
}

// ----------------------------------------------------------------------------

float calculate_overall_length(int index) {

	int i;
	float v = 0;

	// for ( i = 0; i < M_MI; ++i)
	// {
	// 	for ( x = 0; i < towns_count; ++x)
	// 		{
	// 				printf("%d ", population[i][x]);
	// 		}
	// 		printf("\n");
	// }

	// Sum of weights
	for(i = 0; i < towns_count-1; i++){	
		v += weights[population[index][i]][population[index][i+1]];
	}
	//TODO last first also?
	
	return v;
}

// ----------------------------------------------------------------------------

void find_best(){
	int i;
	best_value = FLT_MAX;

	for(i = 0; i < mi_constant; ++i){
		if(best_value > overall_lengths[i]){
			best_value = overall_lengths[i];
			best_index = i;
		} 
	}
}

// ----------------------------------------------------------------------------

void destroy_population() {
	
	int i;

	for (i = 0; i < M_MI; i++)
		free(population[i]);
	
	free(population);
}

// ----------------------------------------------------------------------------

void print_best() {
	
	int i;
	float v = 0.0;
	float t = 0.0;
	
	fprintf(stderr, "[%d]", best_index);

	for(i = 0; i < towns_count; i++)
		fprintf(stderr, " %d", population[best_index][i]);
	
}

// ----------------------------------------------------------------------------

/* If force==1 -> it prints always
 * If force==0 -> it prints only if best_value has changed
 */
void print_population_info(int force) {

	static float prev_best_value = FLT_MAX;

	if (force || prev_best_value != best_value) {
		fprintf(stderr, "Iter %lu: %f\n", global_iteration_counter, best_value);
		prev_best_value = best_value;
	}

}

void print_summary_info(int verbose) {
	
	clock_t time;
	float ips;
	float opt;

	time = clock() - global_start_time;
	
	if (verbose) {
		fprintf(stderr, "All iterations %lu in %.1f seconds.\n", 
			global_iteration_counter, (float)(time) / CLOCKS_PER_SEC);
	}

	ips = global_iteration_counter / ((float)(time) / CLOCKS_PER_SEC);
	fprintf(stderr, "Total avarage %6.6f IPS.\n", ips);
	 
	if (verbose) {
		fprintf(stderr, "Best value is: %.2f.\n", best_value);
		opt = 1.6 * MAX_COORD * sqrt((float)towns_count);
		fprintf(stderr, "On %.0fx%.0f with %d towns estimated optimal should be: %.2f.\n",
			MAX_COORD, MAX_COORD, towns_count, opt);
		fprintf(stderr, "Best value is %.2f percent different estimated optimal (lower is better).\n",
			(best_value/opt-1)*100);
	}

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
void generate_population_overall_length(){
	int i;
	overall_lengths_sum = 0;

	overall_lengths = (float*)malloc(M_MI * sizeof(float));

	for(i = 0; i < M_MI; ++i){
		overall_lengths[i] = calculate_overall_length(i);
		if(i < mi_constant){
			overall_lengths_sum += overall_lengths[i];	
		} 
	}
}

// ----------------------------------------------------------------------------
void generate_overall_lenght_weights(){
	int i;
	overall_lengths_weights = (float*)malloc(mi_constant * sizeof(float));	
	for(i = 0; i<mi_constant; ++i){
		overall_lengths_weights[i] = overall_lengths_sum/overall_lengths[i];
	}
}

void destroy_overall_lenght_weights(){
	free(overall_lengths_weights);
}

// ----------------------------------------------------------------------------
void destroy_population_overall_length(){
	free(overall_lengths);
}

// ----------------------------------------------------------------------------

void init(int argc, char **argv) {
		
	towns_count = 0;
	mi_constant = 0;
	m_constant = 0;
	is_dirty = 0;

	// Process execute parameters
	if (argc == 4) {
		towns_count = atoi(argv[1]);
		mi_constant = atoi(argv[2]);
		m_constant = atoi(argv[3]);
	}
	else {
		fprintf(stderr, "Usage: 'prog towns_count mi m'.\n", argc);
		fprintf(stderr, "Initializing with default values (%d, %d, %d).\n",
			DEFAULT_TOWNS, DEFAULT_MI_CONSTANT, DEFAULT_M_CONSTANT);
	}
	if (towns_count==0) towns_count = DEFAULT_TOWNS;
	if (mi_constant==0) mi_constant = DEFAULT_MI_CONSTANT;
	if (m_constant==0) m_constant = DEFAULT_M_CONSTANT;
	
	generate_towns();

	// Generate connectivity weight matrix
	generate_weight_matrix();

	// Generate population
	generate_population();
	
	// Reset iteration counter
	global_iteration_counter = 0;

	//Each element holds current path length
	generate_population_overall_length();
	generate_overall_lenght_weights();
	find_best();

} // init()

// ----------------------------------------------------------------------------

void terminate() {
	print_summary_info(1);
	fprintf(stderr, "Quiting");
	destroy_towns();
	destroy_weight_matrix();
	fprintf(stderr, ".");
	destroy_population();
	fprintf(stderr, ".");
	destroy_population_overall_length();
	destroy_overall_lenght_weights();
	fprintf(stderr, ".\n");
	exit(0);
} // terminate()

// ----------------------------------------------------------------------------

void evo_iter(void) {
	int i,x,y;

	recalculateRouletteStats();

	//dla wszystkich dzieci
	for(i = mi_constant; i < M_MI; i+=2){
		
		x = getParentRoulette();
		y = getParentRoulette();


		//zrob dziecko
		pmx(x,y,i,i+1);
		mutate(i);
		mutate(i+1);

		//policz jego odleglosc
		overall_lengths[i] = calculate_overall_length(i);
	}

	mixinChildren();
	//qsortPopulation(0,M_MI-1);
}
