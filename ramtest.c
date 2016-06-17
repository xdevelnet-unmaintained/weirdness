#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/time.h>
#include <string.h>
#include <iso646.h>

#define AVOID_MEMSET_OPTIMIZE(addr) *(volatile char *) addr = *(volatile char *) addr;

#define MEGABYTES_MEMAREA 128lu
#define MEMAREA MEGABYTES_MEMAREA*1024*1024lu
#define TESTS 55

typedef int8_t octet;

int main () {
	void *mem = calloc (MEMAREA, sizeof(octet));
	void *mem2 = calloc (MEMAREA, sizeof(octet));

	if (mem == NULL or mem2 == NULL) return EXIT_FAILURE;

	memset(mem, 7, MEMAREA); AVOID_MEMSET_OPTIMIZE(mem);
	memset(mem2, 8, MEMAREA); AVOID_MEMSET_OPTIMIZE(mem2);

	struct timeval tv1, tv2;
	gettimeofday(&tv1, NULL);

	size_t i = TESTS;
	while (i-- > 0) memcpy(mem, mem2, MEMAREA);

	gettimeofday(&tv2, NULL);

	double oncetime = (double) (tv2.tv_usec - tv1.tv_usec) / 1000000 +
		(double) (tv2.tv_sec - tv1.tv_sec);


	printf("one memcpy speed: %f M per sec", MEGABYTES_MEMAREA / (oncetime / TESTS));

	return EXIT_SUCCESS;
}
