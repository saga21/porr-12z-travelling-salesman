#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <float.h>
#include "globals.h"

//8948061100002391241

void swap(int *a, int *b){
	int tmp;
	tmp = *a;
	*a = *b;
	*b = tmp;
}

void swapf(float *a, float* b){
	float tmp;
	tmp = *a;
	*a = *b;
	*b = tmp;
}

void swapRows(int* a, int* b){
	int i,tmp;
	for(i = 0; i<towns_count; ++i){
		tmp = a[i];
		a[i] = b[i];
		b[i] = tmp;
	}
}

void prepareChild(int a, int b, int child, int x, int y){
	int i = 0;
	for(i = 0; i < x; ++i){
		population[child][i] = population[b][i];
	}
	for(i = x; i < y; i++){
		population[child][i] = population[a][i];
	}
	for(i = y; i < towns_count; i++){
		population[child][i] = population[b][i];
	}
}

void checkDuplicates(int a, int b, int child, int x, int y){
	int *counts;
	int i,tmp;

	counts = (int*)malloc((sizeof(int))*towns_count);
	memset(counts,-1,(sizeof(int))*towns_count);

	for(i = x; i<y; ++i){
		counts[population[child][i]] = i;
	}
	for(i = 0; i<x; ++i){
		counts[population[child][i]] = i;
	}
	for(i = y; i<towns_count; ++i){
		counts[population[child][i]] = i;
	}

	for(i = x; i<y; ++i){
		if((counts[population[child][i]] < x) || (counts[population[child][i]] >= y)){
			tmp = i;

			while(counts[population[b][tmp]] >= 0){
				tmp = counts[population[b][tmp]];	
			}
			population[child][i] = population[b][tmp];
		}
	}

	free(counts);
}

int pmx(int parentA, int parentB, int childA, int childB){
	int x,y,i;

	x = rand() % towns_count;
	y = rand() % towns_count;

	if(x>y){
		swap(&x,&y);
	}

	//printf("x: %d , y: %d\n",x,y);
	
	prepareChild(parentA,parentB,childA,x,y);
	checkDuplicates(parentA,parentB,childA,x,y);

	if(childB >= 0){
		prepareChild(parentB,parentA,childB,x,y);
		checkDuplicates(parentB,parentA,childB,x,y);
	}

	return 0;
 }

int getParentRoulette(){
	int x,i = 0,y;
	float sum = 0;

	x = rand() % (int)ceil(overall_lengths_sum);
	
	//TO DO jest rozjazd miedzy trzymana suma a faktyczna - tylko czemu?
	// for(y = 0; y < mi_constant; ++y){
	// 	sum += overall_lengths[y];
	// }

	// printf("%d, %f\n",x,overall_lengths_sum - sum);
	// sum = 0;
	
	while(sum < x && i < mi_constant){
		sum += overall_lengths[i];		
		++i;
	}
	
	return i-1;
}








