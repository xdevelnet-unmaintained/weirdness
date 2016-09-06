#include <stdio.h>
#include <stdlib.h>

int getdecimal() {
	static char mem[55]; mem[54] = '0';
	fgets(mem, sizeof(mem), stdin);
	return atoi(mem);
}

int BinarySearch(int *A, int key, int right) {
	int left = 0, mid;
	while (left <= right) {
		mid = left + (right - left) / 2;
		if (key < A[mid]) right = mid - 1;
		else if (key > A[mid]) left = mid+1;
		else return mid;
	}
	return -1;
}

int main() {
	int arr[] = {5, 8, 10, 23, 55, 100, 101};
	int search = getdecimal();
	printf("%d\n", BinarySearch(arr, search, sizeof(arr)/sizeof(int)));
}
