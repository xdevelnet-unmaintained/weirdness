#include <stdio.h>
#include <strings.h>
#include <stdlib.h>

int comparator(const void *p1, const void *p2) { // proudly copypasted, because I don't give a fuck
	return strcasecmp(* (char * const *) p1, * (char * const *) p2);
}

char *getstring() {
	static char mem[55];
	size_t i = 0;
	while (i < sizeof(mem) - 1) {
		mem[i] = (int) getchar();
		if (mem[i] == '\n') break;
		i++;
	}
	mem[i] = '\0';
	return mem;
}

int binsearch(const char **str, int max, const char *value) {
	int position;
	int begin = 0; 
	int end = max - 1;
	int cond = 0;

	while (begin <= end) {
		position = (begin + end) / 2;
		if ((cond = strcasecmp(str[position], value)) == 0) return position;
		else if (cond < 0) begin = position + 1;
		else end = position - 1;
	}
	return -1;
}

int main() {
	const char **dataset = malloc(5 * sizeof(void *));
	dataset[0] = "Zdsd";
	dataset[1] = "Yool";
	dataset[2] = "Xoooooommmee";
	dataset[3] = "Daq";
	dataset[4] = "Amore";

	qsort(dataset, 5, sizeof(void *), comparator); setbuf(stdout, NULL);

	size_t i = 0;
	while (i < 5) {
		printf("%zu: %s\n", i, dataset[i]);
		i++;
	}
	printf("Enter search str: ");
	printf( "%d\n", binsearch(dataset, 5, getstring()) );
	return 0;
}
