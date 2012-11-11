#pragma once

void swap(int *a, int *b);
void swapf(float *a, float* b);
void swapRows(int* a, int* b);
void prepareChild(int a, int b, int child, int x, int y);
void checkDuplicates(int a, int b, int child, int x, int y);
int pmx(int parentA, int parentB, int childA, int childB);
void mutate(int child);
void mixinChildren();



