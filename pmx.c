#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define NO_TOWNS 100
#define MAP_SIZE 1000

typedef struct {
	int id;
	int x;
	int y;
} town;


town *townArray;


float distance(town *a, town* b){
	float x,y;
	x = (float)abs(a->x - b->x);
	y = (float)abs(a->y - b->y);
	return sqrt(x*x + y*y);
}

void initTowns(){
	int i;

	townArray = (town *)malloc((sizeof town)*NO_TOWNS);

	for(i = 0; i< NO_TOWNS; ++i){
		townArray[i].x = rand() % MAP_SIZE;
		townArray[i].y = rand() % MAP_SIZE;
		townArray[i].id = i;
	}

}

void swap(int *a, int *b){
	int *tmp;
	tmp = a;
	a = b;
	b = tmp;
}

void prepareChild(town* a, town* b, town* child int x, int y){
	int i = 0;
	for(i = 0; i<x; ++i){
		child[i] = b[i];
	}
	for(i = x;i<y;i++){
		child[i] = a[i];
	}
	for(i = y;i<NO_TOWNS;i++){
		child[i] = b[i];
	}
}

void checkDuplicates(town *a, town* b, town* child){
	short *counts;
	int i;

	counts = (short*)malloc((sizeof short)*NO_TOWNS);
	memset(counts,0,(sizeof short)*NO_TOWNS);

	for(i = 0; i< NO_TOWNS; ++i){
		counts[child[i]->id]++;
	}

	free(counts);
}

int pmx(town *a, town *b, town *childA, town *childB){
	int x,y,i;

	x = (rand() % (NO_TOWNS-20))+10;
	y = (rand() % (NO_TOWNS-20))+10;
	if(x>y){
		swap(&x,&y);
	}

	prepareChild(a,b,childA,x,y);
	prepareChild(b,a,childB,x,y);

}

int main(int argc, const char* argv[]){
	initTowns();



	free(townArray);
	return 0;
}






