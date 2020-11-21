#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct regr {
	char reg[50];
	int data;
	struct regr *next;
};

struct regr registry[101];

struct instruction {
	char ins[50];
	int line;
	char src[50];
	char des[50];
};

// linked list with stores all registers

void update(char r[50], int v) {

	// Finding the location to add this new register
	int index = 0;

	while(index < 101 && (strcmp(registry[index].reg, r) != 0 && strcmp(registry[index].reg, "") != 0)) {
		index++;
	}

	if(strcmp(registry[index].reg, "") == 0) {
		strcpy(registry[index].reg, r);
		registry[index].data = v;
	} else {
		registry[index].data = v;
	}

	return;

}

int getData(char r[50]) {
	int index = 0;

	// printf("%c\n", r[1]);
	while(strcmp(registry[index].reg, r) != 0) {
		index++;
	};

	return registry[index].data;

}


int main(int argc, char *argv[]) {
	if (argc != 2) {
		printf("Assembly file missing!\n");
		exit(1);
	}

	FILE *fp = fopen(argv[1], "r"); // Opens assembly file

	char line[50];

	if (fp == NULL) {
		printf("Invalid File!");
		exit(1);
	}

	// Building instructions array

	struct instruction instructions[101];
	int lineNo = 0;

	while(fgets(line, sizeof(line), fp) != NULL) {
		char ins[50];
		strcpy(ins, line);
		// Treating an empty line (Handles both UNIX and Windows line endings

		if(line[0] == '\n' || (line[0] == '\r' && line[1] == '\n')) {
			strcpy(instructions[lineNo].ins, "");
			strcpy(instructions[lineNo].src, "");
			strcpy(instructions[lineNo].des, "");
			instructions[lineNo].line = -1;
			// printf("Empty Line\n");

		}

		// Treating an instruction

		else {
			char *token;
			token = strtok(line, " ");

			if (strcmp(token, "mov") == 0 || strcmp(token, "add") == 0 || strcmp(token, "sub") == 0 || strcmp(token, "mul") == 0 || strcmp(token, "div") == 0) {
				sscanf(ins, "%s %s %s", instructions[lineNo].ins, instructions[lineNo].src, instructions[lineNo].des);
				instructions[lineNo].line = -1;
				// printf("%s %s %s\n", instructions[lineNo].ins, instructions[lineNo].src, instructions[lineNo].des);
			} else if (strcmp(token, "read") == 0 || strcmp(token, "print") == 0) {
				sscanf(ins, "%s %s", instructions[lineNo].ins, instructions[lineNo].src);
				instructions[lineNo].line = -1;
				strcpy(instructions[lineNo].des, "");
				// printf("%s %s\n", instructions[lineNo].ins, instructions[lineNo].src);
			} else if (strcmp(token, "jmp") == 0) {
				sscanf(ins, "%s %d", instructions[lineNo].ins, &instructions[lineNo].line);
				strcpy(instructions[lineNo].src, "");
				strcpy(instructions[lineNo].des, "");
				// printf("%s %d\n", instructions[lineNo].ins, instructions[lineNo].line);
			} else if (strcmp(token, "je") == 0 || strcmp(token, "jne") == 0 || strcmp(token, "jg") == 0 || strcmp(token, "jge") == 0 || strcmp(token, "jl") == 0 || strcmp(token, "jle") == 0) {
				sscanf(ins, "%s %d %s %s", instructions[lineNo].ins, &instructions[lineNo].line, instructions[lineNo].src, instructions[lineNo].des);
				// printf("%s %d %s %s\n", instructions[lineNo].ins, instructions[lineNo].line, instructions[lineNo].src, instructions[lineNo].des);
			}

		}

		lineNo++;
	}

	fclose(fp);

	// struct regr* registry = NULL;

	int index = 0;

	while(index < 101) {
		strcpy(registry[index].reg, "");
		index++;
	}

	int maxlines = lineNo;
	lineNo = 0;
	// printf("%d\n", maxlines);

	while(lineNo <= maxlines) {
		// printf("%s %d %s %s\n", instructions[lineNo].ins, instructions[lineNo].line, instructions[lineNo].src, instructions[lineNo].des);

		if(strcmp(instructions[lineNo].ins, "") == 0) {
			// printf("In the empty line block\n");
			lineNo++;
		} else if(strcmp(instructions[lineNo].ins, "read") == 0) {
			// printf("In the read line block\n");
			int tempint = 0;
			scanf("%d", &tempint);

			update(instructions[lineNo].src, tempint);

			lineNo++;
		} else if(strcmp(instructions[lineNo].ins, "print") == 0) {
			// printf("In the print line block\n");

			// If the src is a register
			if(isalpha(instructions[lineNo].src[0]) > 0) {
				printf("%d", getData(instructions[lineNo].src));
			}
			// If src is a positive/negative number
			else {
				printf("%d", atoi(instructions[lineNo].src));
			}

			lineNo++;
		} else if(strcmp(instructions[lineNo].ins, "mov") == 0) {
			// printf("In the move line block\n");

			int src = 0;

			// If the src is a register
			if(isalpha(instructions[lineNo].src[0]) > 0) {
				src = getData(instructions[lineNo].src);
			}
			// If src is a positive/negative number
			else {
				src = atoi(instructions[lineNo].src);
			}

			update(instructions[lineNo].des, src);

			lineNo++;
		} else if(strcmp(instructions[lineNo].ins, "add") == 0) {
			// printf("In the add line block\n");
			int src = 0;

			// If the src is a register
			if(isalpha(instructions[lineNo].src[0]) > 0) {
				src = getData(instructions[lineNo].src);
			}
			// If src is a positive/negative number
			else {
				src = atoi(instructions[lineNo].src);
			}

			int des = src + getData(instructions[lineNo].des);
			update(instructions[lineNo].des, des);


			lineNo++;
		} else if(strcmp(instructions[lineNo].ins, "sub") == 0) {
			//printf("In the sub line block\n");
			int src = 0;

			// If the src is a register
			if(isalpha(instructions[lineNo].src[0]) > 0) {
				src = getData(instructions[lineNo].src);
			}
			// If src is a positive/negative number
			else {
				src = atoi(instructions[lineNo].src);
			}

			int des =  getData(instructions[lineNo].des) - src;
			update(instructions[lineNo].des, des);

			lineNo++;
		} else if(strcmp(instructions[lineNo].ins, "mul") == 0) {
			// printf("In the mul line block\n");

			int src = 0;

			// If the src is a register
			if(isalpha(instructions[lineNo].src[0]) > 0) {
				src = getData(instructions[lineNo].src);
			}
			// If src is a positive/negative number
			else {
				src = atoi(instructions[lineNo].src);
			}

			int des =  getData(instructions[lineNo].des) * src;
			update(instructions[lineNo].des, des);

			lineNo++;
		} else if(strcmp(instructions[lineNo].ins, "div") == 0) {
			// printf("In the div line block\n");

			int src = 0;

			// If the src is a register
			if(isalpha(instructions[lineNo].src[0]) > 0) {
				src = getData(instructions[lineNo].src);
			}
			// If src is a positive/negative number
			else {
				src = atoi(instructions[lineNo].src);
			}

			int des = src / getData(instructions[lineNo].des);
			update(instructions[lineNo].des, des);

			lineNo++;
		} else if(strcmp(instructions[lineNo].ins, "jmp") == 0) {
			// printf("In the jmp line block\n");
			lineNo = instructions[lineNo].line;
		} else if(strcmp(instructions[lineNo].ins, "je") == 0) {
			// printf("In the je line block\n");
			int x = 0;

			// If the src is a register
			if(isalpha(instructions[lineNo].src[0]) > 0) {
				x = getData(instructions[lineNo].src);
			}
			// If src is a positive/negative number
			else {
				x = atoi(instructions[lineNo].src);
			}

			int y = 0;

			// If the des is a register
			if(isalpha(instructions[lineNo].des[0]) > 0) {
				y = getData(instructions[lineNo].des);
			}
			// If des is a positive/negative number
			else {
				y = atoi(instructions[lineNo].des);
			}

			if(x == y)
				lineNo = instructions[lineNo].line;
			else
				lineNo++;
		} else if(strcmp(instructions[lineNo].ins, "jne") == 0) {
			// printf("In the jne line block\n");

			int x = 0;

			// If the src is a register
			if(isalpha(instructions[lineNo].src[0]) > 0) {
				x = getData(instructions[lineNo].src);
			}
			// If src is a positive/negative number
			else {
				x = atoi(instructions[lineNo].src);
			}

			int y = 0;

			// If the des is a register
			if(isalpha(instructions[lineNo].des[0]) > 0) {
				y = getData(instructions[lineNo].des);
			}
			// If des is a positive/negative number
			else {
				y = atoi(instructions[lineNo].des);
			}

			if(x != y)
				lineNo = instructions[lineNo].line;
			else
				lineNo++;
		} else if(strcmp(instructions[lineNo].ins, "jg") == 0) {
			// printf("In the jg line block\n");
			int x = 0;

			// If the src is a register
			if(isalpha(instructions[lineNo].src[0]) > 0) {
				x = getData(instructions[lineNo].src);
			}
			// If src is a positive/negative number
			else {
				x = atoi(instructions[lineNo].src);
			}

			int y = 0;

			// If the des is a register
			if(isalpha(instructions[lineNo].des[0]) > 0) {
				y = getData(instructions[lineNo].des);
			}
			// If des is a positive/negative number
			else {
				y = atoi(instructions[lineNo].des);
			}

			if(x > y)
				lineNo = instructions[lineNo].line;
			else
				lineNo++;
		} else if(strcmp(instructions[lineNo].ins, "jge") == 0) {
			// printf("In the jge line block\n");
			int x = 0;

			// If the src is a register
			if(isalpha(instructions[lineNo].src[0]) > 0) {
				x = getData(instructions[lineNo].src);
			}
			// If src is a positive/negative number
			else {
				x = atoi(instructions[lineNo].src);
			}

			int y = 0;

			// If the des is a register
			if(isalpha(instructions[lineNo].des[0]) > 0) {
				y = getData(instructions[lineNo].des);
			}
			// If des is a positive/negative number
			else {
				y = atoi(instructions[lineNo].des);
			}

			if(x >= y)
				lineNo = instructions[lineNo].line;
			else
				lineNo++;
		} else if(strcmp(instructions[lineNo].ins, "jl") == 0) {

			// printf("In the jl line block\n");
			int x = 0;

			// If the src is a register
			if(isalpha(instructions[lineNo].src[0]) > 0) {
				x = getData(instructions[lineNo].src);
			}
			// If src is a positive/negative number
			else {
				x = atoi(instructions[lineNo].src);
			}

			int y = 0;

			// If the des is a register
			if(isalpha(instructions[lineNo].des[0]) > 0) {
				y = getData(instructions[lineNo].des);
			}
			// If des is a positive/negative number
			else {
				y = atoi(instructions[lineNo].des);
			}

			if(x < y)
				lineNo = instructions[lineNo].line;
			else
				lineNo++;
		} else if(strcmp(instructions[lineNo].ins, "jle") == 0) {
			// printf("In the jle line block\n");

			int x = 0;


			// If the src is a register
			if(isalpha(instructions[lineNo].src[0]) > 0) {
				x = getData(instructions[lineNo].src);
			}
			// If src is a positive/negative number
			else {
				x = atoi(instructions[lineNo].src);
			}

			int y = 0;

			// If the des is a register
			if(isalpha(instructions[lineNo].des[0]) > 0) {
				y = getData(instructions[lineNo].des);
			}
			// If des is a positive/negative number
			else {
				y = atoi(instructions[lineNo].des);
			}

			if(x <= y)
				lineNo = instructions[lineNo].line;
			else
				lineNo++;
		}
	}

}
