#include <stdio.h>

int main(int argc, char* argv[]) {
	int total = 0, oddNum=0, evenNum=0, argInc = 1, n = 0;


	// Counting odd + even for all files:

	// printf("argc: %d | argv: %d\n", argc, (sizeof(argv)/sizeof(argv[0])));

	while (argInc <= argc-1) {
		int t = 0;
		FILE* fp = fopen(argv[argInc], "r");
		fscanf(fp, "%d", &t); // First num is inserted into total
		total = total + t;

		while(fscanf(fp, "%d", &n) != EOF) {

			if ((n % 2) == 0) {
				evenNum++;
			} else {
				oddNum++;
			}
		}

		fclose(fp);
		argInc++;
	} // At this point, I should have the total size of the entire array, entire even + off

	// printf("Even: %d | Odd: %d\n", evenNum, oddNum);

	argInc = 1;
	int* even = (int *)malloc(sizeof(int) * evenNum);
	int* odd = (int *)malloc(sizeof(int) * oddNum);
	int a = 0;
	int c = 0;

	while (argInc <= argc-1) {
		FILE* fp = fopen(argv[argInc], "r");
		fscanf(fp, "%d", &n);

		while (fscanf(fp, "%d", &n) != EOF) {

			if ((n % 2) == 0) {
				even[c] = n;
				c++;
			} else if ((n % 2) != 0) {
				odd[a] = n;
				a++;
			}
		}

		fclose(fp);
		argInc++;
	}


	// Before sorting, even and odd arrays in original order

	/*
	printf("Before Sorting in individual arrays\n");

	for(int k = 0; k < evenNum; k++){
		printf("%d\t", even[k]);
	}
	printf("\n");

	for(int k = 0; k < oddNum; k++){
		printf("%d\t", odd[k]);
	}
	printf("\n");
	*/

	// Sorting begins

	for (int x = 0; x < evenNum; ++x) {
		for (int y = x + 1; y < evenNum; ++y) {
			if (even[x] > even[y]) {
				int temp = even[x];
				even[x] = even[y];
				even[y] = temp;
			}
		}
	}
	for (int x = 0; x < oddNum; x++) {
		for (int y = x + 1; y < oddNum; y++) {
			if (odd[x] < odd[y]) {
				int temp = odd[x];
				odd[x] = odd[y];
				odd[y] = temp;
			}
		}
	}

	// After sorting, even and odd arrays in sorted order

	/*
	printf("After sorting within individual arrays\n");


	for(int k = 0; k < evenNum; k++){
		printf("%d\t", even[k]);
	}
	printf("\n");

	for(int k = 0; k < oddNum; k++){
		printf("%d\t", odd[k]);
	}
	printf("\n");

	*/

	c = 0;

	int* final = malloc(sizeof(int) * total); // New array which holds all ints in sorted order

	for(int k = 0; k < total; k++) {
		if (k < evenNum) {
			final[k] = even[k];
		} else {
			final[k] = odd[c];
			c++;
		}
	}

	// After sorting, even and odd arrays in sorted order

	for(int k = 0; k < total; k++) {
		printf("%d\t", final[k]);
	}

	printf("\n");

	free(even);
	free(odd);
	free(final);
}


