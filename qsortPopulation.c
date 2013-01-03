#include "globals.h"
#include "qsortPopulation.h"
#include "evolutionLib.h"


void qsortPopulation(int l, int r){
	int pivot,i,j,k;
	//float tmp;
	int L[2],R[2];

	i = l;
	j = r;

	if(l>r){
		return;
	}

	pivot = split(l,r);


	do
	{
	  	while (overall_lengths[i] < overall_lengths[pivot]) ++i;
		while (overall_lengths[j] > overall_lengths[pivot]) --j;
		if (i<=j)
		{
			swapRows(population[i],population[j]);			
			swapf(&overall_lengths[i], &overall_lengths[j]);
			++i; --j;
		}
	} while(i < j);

	L[0]=l;R[0]=j;
	L[1]=i;R[1]=r;

	// #pragma omp parallel
	// {
	// 	#pragma omp for nowait
		for(k = 0; k < 2; ++k){
			qsortPopulation(L[k],R[k]);
		}
	//}
}

int split(int l, int r){
	return (l+r)/2;
}