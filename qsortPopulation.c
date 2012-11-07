#include "globals.h"
#include "qsortPopulation.h"


void qsortPopulation(int l, int r){
	int pivot,i,j;
	float tmp;
	i = l;
	j = r;

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
	if (l < j) qsortPopulation(l, j);
	if (r > i) qsortPopulation(i, r);

}

int split(l,r){
	return (l+r)/2;
}