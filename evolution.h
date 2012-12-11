#pragma once

void init(int argc, char **argv);
void terminate();
void generate_towns();
void destroy_towns();
void generate_population();
void destroy_population();
void generate_weight_matrix();
void destroy_weight_matrix();

float calculate_weight(int i, int j);
float calculate_overall_length(int index);
void generate_population_overall_length();
void destroy_population_overall_length();

void print_population_info(int force);
void print_summary_info(int verbose);

float find_best();
void print_best();

void evo_iter(void);
