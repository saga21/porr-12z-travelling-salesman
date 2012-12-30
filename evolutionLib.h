#pragma once

void swap(int *a, int *b);
void swapf(float *a, float* b);
void swapRows(int* a, int* b);
void prepareChild(int a, int b, int child, int x, int y);
void checkDuplicates(int a, int b, int child, int x, int y);
int pmx(int parentA, int parentB, int childA, int childB, unsigned *seed);
void mutate_random(int child, unsigned *seed);
void mutate_reverse_swap(int child, unsigned* seed);
void mutate_swap_neighbours(int child, unsigned *seed);
void mixinChildren();
int is_crossing(int a, int b, int i, int j);
void mutate_remove_crossover(int child, unsigned *seed);
void mixin(int max);


