#include <stdio.h>

#include <string.h>

int main(int argc, char * argv[]) {
	//printf("argc: %d | argv %d\n", argc, (sizeof(argv) / sizeof(argv[0])));
	//printf("argv[1]: %s\n", argv[1]);
	//printf("strlen argv[1]: %d\n", strlen(argv[1]));

	//printf("First letter of argv[1]: %c\n", hello[0]);

	int argCounter = 1;
	int strCounter = 0;
	while (argCounter <= (argc - 1)) {
		char curr[strlen(argv[argCounter]) + 1];
		strcpy(curr, argv[argCounter]);
		curr[strlen(argv[argCounter])] = '\0';

		//printf("Value of (strlen(argv[argCounter]) + 1): %d\n", (strlen(argv[argCounter]) + 1));
		int strLength = (strlen(argv[argCounter]) + 1);

		while (strCounter < strLength) {
			if (curr[strCounter] == 'A' || curr[strCounter] == 'a' || curr[strCounter] == 'E' || curr[strCounter] == 'e' || curr[strCounter] == 'I' || curr[strCounter] == 'i' || curr[strCounter] == 'O' || curr[strCounter] == 'o' || curr[strCounter] == 'U' || curr[strCounter] == 'u') {
				printf("%c", curr[strCounter]);
			}
			strCounter++;
		}
		strCounter = 0;
		argCounter++;
	}

	printf("\n");

}
