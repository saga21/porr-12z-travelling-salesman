#pragma once

void init(int argc, char **argv);
void terminate(void);
void generate_towns(void);
void destroy_towns(void);
void generate_population(void);
void destroy_population(void);
void generate_weight_matrix(void);
void destroy_weight_matrix(void);

float calculate_weight(int i, int j);
float calculate_overall_length(int index);
void generate_population_overall_length(void);
void destroy_population_overall_length(void);

void print_population_info(int force);
void print_summary_info(int verbose);

float find_best(void);
void print_best(void);

void evo_iter(void);
