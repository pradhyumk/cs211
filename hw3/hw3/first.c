#include <stdio.h>
#include <stdlib.h>
#include <string.h>
unsigned short getBit(unsigned short x, int n);
unsigned short setBit(unsigned short x, int n, unsigned short v);
unsigned short comp(unsigned short x, int n);

int main(int argc, char* argv[]) {
	if (argc < 2 || argc > 2) {
		printf("error - no file\n");
		exit(1);
	}

	unsigned short num;
	FILE* fp = fopen(argv[1], "r");
	fscanf(fp, "%hu\n", &num);
	char command[5];
	int index;
	unsigned short val;

	while(fscanf(fp, "%s\t%d\t%hu\n", command, &index, &val) != EOF) {
		
		// printf("%s\n", command); // Debug: prints command
		
		if (strcmp(command, "get") == 0) {
			printf("%u\n", getBit(num, index));
		} else if (strcmp(command, "set") == 0) {
			num = setBit(num, index, val);
			printf("%u\n", num);
		} else if (strcmp(command, "comp") == 0) {
			num = comp(num, index);
			printf("%u\n", num);
		} else {
			printf("invalid command in text file\n");
			exit(1);
		}
	}

	fclose(fp);
}

unsigned short getBit(unsigned short x, int n) {
	unsigned short computation = (x >> n) & 1;
	return computation;
}

unsigned short setBit(unsigned short x, int n, unsigned short v) {
	unsigned short currentval = getBit(x, n);
	unsigned short computation = x;

	if (currentval == 1 && v == 0) {
		// basically create a mask of ones except the value you want to change and use 'bitwise and'.
		unsigned short mask = ~(1 << n); // moves the 1 to the index which needs to be changed and invert the entire thing.
		computation = x & mask;
	} else if (currentval == 0 && v == 1) {
		// create  mask of zeros except the index which needs to be set and use 'bitwise or'
		unsigned short mask = 1 << n;
		computation = x | mask;
	}

	return computation;
}

unsigned short comp(unsigned short x, int n) {
	// basically a repitition of set, but bit needs to be switched regardess of what it holds.
	unsigned short currentval = getBit(x, n);
	unsigned short computation;

	if (currentval == 0) {
		// create  mask of zeros except the index which needs to be set and use 'bitwise or'
		unsigned short mask = 1 << n;
		computation = x | mask;
	} else if (currentval == 1) {
		// basically create a mask of ones except the value you want to change and use 'bitwise and'.
		unsigned short mask = ~(1 << n); // moves the 1 to the index which needs to be changed and invert the entire thing.
		computation = x & mask;
	}

	return computation;
}