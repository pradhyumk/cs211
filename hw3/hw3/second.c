#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned short getBit(unsigned short x, int n);
int length(unsigned short x);

int main(int argc, char* argv[]) {
	if (argc != 2) {
		printf("error - no file\n");
		exit(1);
	}

	unsigned short num = atoi(argv[1]);
	// printf("Number input: %u\n", num);
	int num_of_bits = length(num);
	int pairs = 0;
	int i1 = 0, i2 = 1;

	// Getting the adjacent pairs

	while(i1 < num_of_bits && i2 < num_of_bits) {
		if (getBit(num, i1) == 1 && getBit(num, i2) == 1) {
			pairs++;
			i1 += 2;
			i2 += 2;
		} else {
			i1 += 1;
			i2 += 1;
		}
	}

	// printf("Pairs: %d\n", pairs);
	int num_of_1_bits = 0;
	i1 = 0;

	while(i1 < num_of_bits) {
		if(getBit(num, i1) == 1) {
			num_of_1_bits++;
		}

		i1++;
	}

	if (num_of_1_bits % 2 == 0) {
		printf("Even-Parity\t%d\n", pairs);
	} else {
		printf("Odd-Parity\t%d\n", pairs);
	}
}

// reused the get method from the first mini program

unsigned short getBit(unsigned short x, int n) {
	unsigned short computation = (x >> n) & 1;
	return computation;
}

int length(unsigned short x) {
	int length = 0;
	unsigned short num = x;

	while(num != 0) {
		num = num >> 1;
		// printf("Shifting: %u\n", num);
		length++;
	}

	// printf("Length:%d\n", length);
	return length;
}