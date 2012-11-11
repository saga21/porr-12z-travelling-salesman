
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <float.h>
#include "globals.h"
#include "evolutionLib.h"

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

 void mutate(int child){
 	int i,r,tmp;
 	int* newChild;
 	float childOverallLength = 0;

 	newChild = (int*)malloc(sizeof(int)*towns_count);
 	memset(newChild,-1,(sizeof(int))*towns_count);

 	for(i = 0; i < towns_count; ++i){
	 			newChild[i] = population[child][i];
 	}

 	for(i = 0; i < towns_count; ++i){
		if(rand()%8 == 0){
			r = rand()%towns_count;
			tmp = newChild[r];
			newChild[r] = newChild[i];
			newChild[i] = tmp;
		}
 	}

	for(i = 0; i < towns_count-1; i++){	
		childOverallLength += weights[newChild[i]][newChild[i+1]];
	}
 	
 	if(childOverallLength < overall_lengths[child]){
 		swapRows(newChild,population[child]);
 		overall_lengths[child] = childOverallLength;
 	}

 	free(newChild);
 }

 void mixinChildren(){
 	int i,j,c,r;

 	c = mi_constant/m_constant;

 	for(i = mi_constant; i < M_MI; ++i){
 		for(j = 0; j<c; ++j){
 			r = rand() % mi_constant;
 			if(overall_lengths[r]>overall_lengths[i]){
 				swapRows(population[r],population[i]);
 				swapf(&overall_lengths[r],&overall_lengths[i]);
 				break;
 			}
 		}
 	}
 }









