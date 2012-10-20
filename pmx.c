#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>

#define NO_TOWNS 8
#define MAP_SIZE 1000

typedef struct {
	int id;
	int x;
	int y;
} town;


town townArrayA[] = {{.id = 0},{.id = 1},{.id = 4},{.id = 5},{.id = 3},{.id = 2},{.id = 7},{.id = 6}};
town townArrayB[] = {{.id = 0},{.id = 3},{.id = 1},{.id = 2},{.id = 5},{.id = 4},{.id = 6},{.id = 7}};


// float distance(town *a, town* b){
// 	float x,y;
// 	x = (float)abs(a->x - b->x);
// 	y = (float)abs(a->y - b->y);
// 	return sqrt(x*x + y*y);
// }

// void initTowns(){
// 	int i;

// 	townArray = (town *)malloc((sizeof town)*NO_TOWNS);

// 	for(i = 0; i< NO_TOWNS; ++i){
// 		townArray[i].x = rand() % MAP_SIZE;
// 		townArray[i].y = rand() % MAP_SIZE;
// 		townArray[i].id = i;
// 	}

// }

void swap(int *a, int *b){
	int tmp;
	tmp = *a;
	*a = *b;
	*b = tmp;
}

void prepareChild(town* a, town* b, town** child, int x, int y){
	int i = 0;
	for(i = 0; i<x; ++i){
		child[i] = &b[i];
	}
	for(i = x;i<y;i++){
		child[i] = &a[i];
	}
	for(i = y;i<NO_TOWNS;i++){
		child[i] = &b[i];
	}
}

void checkDuplicates(town *a, town* b, town** child, int x, int y){
	int *counts;
	int i,tmp;

	counts = (int*)malloc((sizeof(int))*NO_TOWNS);
	memset(counts,-1,(sizeof(int))*NO_TOWNS);

	for(i = x; i<y; ++i){
		counts[child[i]->id] = i;
	}
	for(i = 0; i<x; ++i){
		counts[child[i]->id] = i;
	}
	for(i = y; i<NO_TOWNS; ++i){
		counts[child[i]->id] = i;
	}

	for(i = x; i<y; ++i){
		if((counts[child[i]->id] < x) || (counts[child[i]->id] >= y)){
			tmp = i;

			while(counts[b[tmp].id] >= 0){
				tmp = counts[b[tmp].id];	
			}
			child[i] = &b[tmp];
		}
	}

	free(counts);
}

void print(town* t){
	int i;
	for(i=0;i<NO_TOWNS;++i){
		printf("%d ",t[i].id);
	}
	printf("\n");
}
void printp(town** t){
	int i;
	for(i=0;i<NO_TOWNS;++i){
		printf("%d ",t[i]->id);
	}
	printf("\n");
}

int pmx(town *a, town *b, town **childA, town **childB){
	int x,y,i;

	//x = (rand() % (NO_TOWNS-20))+10;
	//y = (rand() % (NO_TOWNS-20))+10;
	srand ( time(NULL) );
	x = rand() % NO_TOWNS;
	y = rand() % NO_TOWNS;

	if(x>y){
		swap(&x,&y);
	}

	printf("x: %d , y: %d\n",x,y);
	

	printf("A: "); print(a);
	printf("B: "); print(b);

	prepareChild(a,b,childA,x,y);
	prepareChild(b,a,childB,x,y);

	printf("preparedA: "); printp(childA);
	printf("preparedB: "); printp(childB);
		

	checkDuplicates(a,b,childA,x,y);
	checkDuplicates(b,a,childB,x,y);

	printf("cA: "); printp(childA);
	printf("cB: "); printp(childB);
	return 0;
}

int main(int argc, const char* argv[]){
	//initTowns();
	town **ca; 
	town **cb;
	ca = (town **)malloc((sizeof(town *))*NO_TOWNS);
	cb = (town **)malloc((sizeof(town *))*NO_TOWNS);

	pmx(townArrayA,townArrayB,ca,cb);

	memset(ca,0,(sizeof(town *))*NO_TOWNS);
	memset(cb,0,(sizeof(town *))*NO_TOWNS);

	free(ca);
	free(cb);
	return 0;
}






