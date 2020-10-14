#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned short getBit(unsigned short x, int n);
int main(int argc, char* argv[]) {
	if (argc != 2) {
		printf("error - no file\n");
		exit(1);
	}

	unsigned short num = atoi(argv[1]);
	int i1 = 15;
	int i2 = 0;
	int flag = 0;

	while(i1 != 8 && i2 != 8) {
		if (getBit(num, i1) != getBit(num, i2)) {
			flag = 1;
		}

		if(flag == 1)
			break;

		i1--;
		i2++;
	}

	if (flag == 0) {
		printf("Is-Palindrome\n");
	} else {
		printf("Not-Palindrome\n");
	}
}

// copied from first mini program

unsigned short getBit(unsigned short x, int n) {
	unsigned short computation = (x >> n) & 1;
	return computation;
}