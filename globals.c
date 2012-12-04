#include <float.h> // FLT_MAX
#include <limits.h> // INT_MAX
#ifdef _WIN32
#define _CRT_RAND_S
#endif
#include <stdlib.h> // rand
#include <stdio.h>
#include <time.h>

int best_index = 0;
float best_value = FLT_MAX;
int global_benchmark = INT_MAX;

// TODO: Make these functions inline
long clock_ms()
{
#ifdef _WIN32
	return (long)(clock() * 1000 / CLOCKS_PER_SEC);
#else
	struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
	return (long)(ts.tv_sec * 1000.0f + ts.tv_nsec / 1000.0f);
#endif
}

int rand_my(unsigned int* seed) {
#ifdef _WIN32

	// TODO: Obmyslec
	// Ten kod jest 4-5 razy wolniejszy od zwyklego rand()
	// Czy zwykly rand() wystarczy?
	//http://stackoverflow.com/questions/4287531/how-to-generate-random-numbers-in-parallel
	//It seems like that rand has a global shared state between all threads on Linux and a thread local storage state for it on Windows. The shared state on Linux is causing your slowdowns because of the necessary synchronization.
	//int s = 0;
    //rand_s((unsigned int*)&s);
	//return s&0x7FFFFFFF;
	return rand();
#else
	return rand_r(seed);
#endif
}
