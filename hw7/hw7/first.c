#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


struct info {
	char cmd[50];
	int num;
	struct info *next;
};


int getBit(int x, int n) {
	int computation = (x >> n) & 1;
	return computation;
}


int getVal(char arg[], struct info i[], int l, struct info **temp) {

	int x = 0;

	while(x < l) {
		if (strcmp(i[x].cmd, arg) == 0) {
			return i[x].num;
		}

		x++;
	}

	// Only drops down if variable was not found in the input array... Now checking temp vars


	struct info *cur = *temp;

	while(cur != NULL) {
		if (strcmp(cur->cmd, arg) == 0) {
			return cur->num;
		}

		cur = cur->next;
	}

	return -1;

}

void insertVal(char arg[], int val, struct info o[], int l, struct info **temp) {

	int x = 0;

	while(x < l) {
		if (strcmp(o[x].cmd, arg) == 0) {
			o[x].num = val;
			return;
		}
		x++;
	}

	// Only drops down if variable was not found in the input array... Now checking temp vars

	if (*temp == NULL) {
		*temp = malloc(sizeof(struct info));
		strcpy((*temp)->cmd, arg);
		(*temp)->num = val;
		(*temp)->next = NULL;
	} else {
		struct info *cur = *temp;

		while(cur != NULL) {
			if (strcmp(cur->cmd, arg) == 0) {
				cur->num = val;
				return;
			}

			cur = cur->next;
		}

		cur = *temp;

		while(cur->next != NULL) {
			cur = cur->next;
		}

		cur->next = malloc(sizeof(struct info));
		strcpy(cur->next->cmd, arg);
		cur->next->num = val;
		cur->next->next = NULL;
	}

	return;
}


void delete(struct info *node) {
	struct info *temp;

	while (node != NULL) {
		temp = node;
		node = node->next;
		free(temp);
	}

	return;
}


int main(int argc, char *argv[]) {
	// Check if there is a second parameter
	if (argc != 2) {
		printf("error\n");
		exit(1);
	}

	FILE *fp = fopen(argv[1], "r"); // Opens circuit file

	char line[100];

	// Storing input input variables

	fgets(line, sizeof(line), fp);

	// Removing the annoying trailing new line
	unsigned int ln = strlen(line) - 1;

	if (*line && line[ln] == '\n')
		line[ln] = '\0';

	// printf("INPUT VARS LINE: %s\n", line);
	char *token;
	token = strtok(line, " ");

	int c = 0;
	int inputvars = 0;

	token = strtok(NULL, " ");

	// printf("# of inputvars: %s\n", token);
	inputvars = atoi(token);

	struct info input[inputvars];

	token = strtok(NULL, " ");

	while(token != NULL) {
		strcpy(input[c].cmd, token);
		c++;
		token = strtok(NULL, " ");
	}

	// Onto the output variables

	fgets(line, sizeof(line), fp);

	// Removing the annoying trailing new line
	ln = strlen(line) - 1;

	if (*line && line[ln] == '\n')
		line[ln] = '\0';

	token = strtok(line, " ");

	c = 0;
	int outputvars = 0;

	token = strtok(NULL, " ");

	outputvars = atoi(token);

	struct info output[outputvars];

	token = strtok(NULL, " ");

	while(token != NULL) {
		strcpy(output[c].cmd, token);
		c++;
		token = strtok(NULL, " ");
	}

	fclose(fp);

	struct info *tempvars = NULL;

	int maxnum = (int) pow(2.0, (double)inputvars);

	c = 0;

	while (c < maxnum) {

		// Set the input values accordingly
		int a = inputvars - 1;
		int b = 0;

		while(a >= 0) {
			input[a].num = getBit(c, b);
			a--;
			b++;
		}

		FILE *fp = fopen(argv[1], "r"); // Opens circuit file

		while(fgets(line, sizeof(line), fp) != NULL) {
			char templine[100];
			strcpy(templine, line);
			char *block = strtok(templine, " ");


			if(strcmp(block, "NOT") == 0) {
				char dummy[10];
				char arg1[50];
				char arg2[50];
				sscanf(line, "%s %s %s\n", dummy, arg1, arg2);

				// Get value of arg1
				int arg1val = getVal(arg1, input, inputvars, &tempvars);

				if (arg1val == 1) {
					insertVal(arg2, 0, output, outputvars, &tempvars);
				} else if (arg1val == 0) {
					insertVal(arg2, 1, output, outputvars, &tempvars);
				}
			} else if(strcmp(block, "AND") == 0) {
				char dummy[10];
				char arg1[50];
				char arg2[50];
				char arg3[50];
				sscanf(line, "%s %s %s %s\n", dummy, arg1, arg2, arg3);

				// Get value of arg1
				int arg1val = getVal(arg1, input, inputvars, &tempvars);

				// Get value of arg2
				int arg2val = getVal(arg2, input, inputvars, &tempvars);

				if (arg1val == 0 && arg2val == 0) {
					insertVal(arg3, 0, output, outputvars, &tempvars);
				} else if(arg1val == 0 && arg2val == 1) {
					insertVal(arg3, 0, output, outputvars, &tempvars);
				} else if(arg1val == 1 && arg2val == 0) {
					insertVal(arg3, 0, output, outputvars, &tempvars);
				} else if(arg1val == 1 && arg2val == 1) {
					insertVal(arg3, 1, output, outputvars, &tempvars);
				}

			} else if(strcmp(block, "OR") == 0) {
				char dummy[10];
				char arg1[50];
				char arg2[50];
				char arg3[50];
				sscanf(line, "%s %s %s %s\n", dummy, arg1, arg2, arg3);

				// Get value of arg1
				int arg1val = getVal(arg1, input, inputvars, &tempvars);

				// Get value of arg2
				int arg2val = getVal(arg2, input, inputvars, &tempvars);

				if (arg1val == 0 && arg2val == 0) {
					insertVal(arg3, 0, output, outputvars, &tempvars);
				} else if(arg1val == 0 && arg2val == 1) {
					insertVal(arg3, 1, output, outputvars, &tempvars);
				} else if(arg1val == 1 && arg2val == 0) {
					insertVal(arg3, 1, output, outputvars, &tempvars);
				} else if(arg1val == 1 && arg2val == 1) {
					insertVal(arg3, 1, output, outputvars, &tempvars);
				}
			} else if(strcmp(block, "NAND") == 0) {
				char dummy[10];
				char arg1[50];
				char arg2[50];
				char arg3[50];
				sscanf(line, "%s %s %s %s\n", dummy, arg1, arg2, arg3);

				// Get value of arg1
				int arg1val = getVal(arg1, input, inputvars, &tempvars);

				// Get value of arg2
				int arg2val = getVal(arg2, input, inputvars, &tempvars);

				if (arg1val == 0 && arg2val == 0) {
					insertVal(arg3, 1, output, outputvars, &tempvars);
				} else if(arg1val == 0 && arg2val == 1) {
					insertVal(arg3, 1, output, outputvars, &tempvars);
				} else if(arg1val == 1 && arg2val == 0) {
					insertVal(arg3, 1, output, outputvars, &tempvars);
				} else if(arg1val == 1 && arg2val == 1) {
					insertVal(arg3, 0, output, outputvars, &tempvars);
				}
			} else if(strcmp(block, "NOR") == 0) {
				char dummy[10];
				char arg1[50];
				char arg2[50];
				char arg3[50];
				sscanf(line, "%s %s %s %s\n", dummy, arg1, arg2, arg3);

				// Get value of arg1
				int arg1val = getVal(arg1, input, inputvars, &tempvars);

				// Get value of arg2
				int arg2val = getVal(arg2, input, inputvars, &tempvars);

				if (arg1val == 0 && arg2val == 0) {
					insertVal(arg3, 1, output, outputvars, &tempvars);
				} else if(arg1val == 0 && arg2val == 1) {
					insertVal(arg3, 0, output, outputvars, &tempvars);
				} else if(arg1val == 1 && arg2val == 0) {
					insertVal(arg3, 0, output, outputvars, &tempvars);
				} else if(arg1val == 1 && arg2val == 1) {
					insertVal(arg3, 0, output, outputvars, &tempvars);
				}
			} else if(strcmp(block, "XOR") == 0) {
				char dummy[10];
				char arg1[50];
				char arg2[50];
				char arg3[50];
				sscanf(line, "%s %s %s %s\n", dummy, arg1, arg2, arg3);

				// Get value of arg1
				int arg1val = getVal(arg1, input, inputvars, &tempvars);

				// Get value of arg2
				int arg2val = getVal(arg2, input, inputvars, &tempvars);
				// printf("arg2 val: %d\n", arg2val);

				if (arg1val == 0 && arg2val == 0) {
					insertVal(arg3, 0, output, outputvars, &tempvars);
				} else if(arg1val == 0 && arg2val == 1) {
					insertVal(arg3, 1, output, outputvars, &tempvars);
				} else if(arg1val == 1 && arg2val == 0) {
					insertVal(arg3, 1, output, outputvars, &tempvars);
				} else if(arg1val == 1 && arg2val == 1) {
					insertVal(arg3, 0, output, outputvars, &tempvars);
				}
			}

		}

		int x = 0;

		while(x < inputvars) {
			printf("%d ", input[x].num);
			x++;
		}

		x = 0;

		while(x < outputvars) {
			printf("%d ", output[x].num);
			x++;
		}

		printf("\n");

		fclose(fp);

		c++;
	}

	// Freeing temp variables linked list
	delete(tempvars);

}
