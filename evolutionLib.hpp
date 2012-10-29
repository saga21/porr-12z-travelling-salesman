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

int pmx(int a, int b, int childA, int childB){
	int x,y,i;

	x = rand() % towns_count;
	y = rand() % towns_count;

	if(x>y){
		swap(&x,&y);
	}

	//printf("x: %d , y: %d\n",x,y);
	
	prepareChild(a,b,childA,x,y);
	checkDuplicates(a,b,childA,x,y);

	if(childB >= 0){
		prepareChild(b,a,childB,x,y);
		checkDuplicates(b,a,childB,x,y);
	}

	return 0;
 }

// int main(int argc, const char* argv[]){
// 	//initTowns();
// 	town **ca; 
// 	town **cb;
// 	ca = (town **)malloc((sizeof(town *))*towns_count);
// 	cb = (town **)malloc((sizeof(town *))*towns_count);

// 	pmx(townArrayA,townArrayB,ca,cb);

// 	//memset(ca,0,(sizeof(town *))*towns_count);
// 	//memset(cb,0,(sizeof(town *))*towns_count);

// 	free(ca);
// 	free(cb);
// 	return 0;
// }






