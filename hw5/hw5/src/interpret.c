#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct regr {
	char reg[10];	
	int data;
	struct regr *next;
};

struct instruction {
	char ins[50];
	int line;
	char src[50];
	char des[50];
};

// linked list with stores all registers

void add(struct regr* root, char r[10], int v){

	if(root == NULL){
		root = (struct regr*) malloc(sizeof(struct regr));
		strcpy(r, root->reg);
		root->data = v;
		root->next = NULL;
		return;
	}


	struct regr* ptr = root;
	
	while(ptr->next != NULL) {
		ptr = ptr->next;
	}

	ptr->next = (struct regr*) malloc(sizeof(struct regr));
	strcpy(r, ptr->next->reg);
	ptr->next->data = v;
	ptr->next->next = NULL;	

	return;
}

void update(struct regr* root, char r[10], int v){

	struct regr* ptr = root;

	while(strcmp(ptr->reg, r) != 0){
		ptr = ptr->next;
	}

	ptr->data = v;

}

int getData(struct regr* root, char r[10]){

	struct regr* ptr = root;

	while(strcmp(ptr->reg, r) != 0){
		ptr = ptr->next;
	}

	return ptr->data;

}

void delete(struct regr* node) {
	if(node == NULL)
		return;

	if (node->next == NULL) {
		free(node);
		node = NULL;
		return;
	} else {
		delete(node->next);
		node->next = NULL;
	}
	free(node);
	node = NULL;

	return;
}


int main(int argc, char* argv[])
{	
	if (argc != 2){
		printf("Assembly file missing!\n");
		exit(1);
	}
	
	FILE* fp = fopen(argv[1], "r"); // Opens assembly file

	char line[50];
	
	if (fp == NULL){
		printf("Invalid File!");
		exit(1);
	}

	// Building instructions array

	struct instruction instructions[100];
	int lineNo = 0;
	
	while(fgets(line, sizeof(line), fp) != NULL){
	char ins[50];
	strcpy(ins, line);
	// Treating an empty line (Handles both UNIX and Windows line endings

	if(line[0] == '\n' || (line[0] == '\r' && line[1] == '\n')){
		strcpy(instructions[lineNo].ins, "");
		strcpy(instructions[lineNo].src, "");
		strcpy(instructions[lineNo].des, "");
		instructions[lineNo].line = -1;
		printf("Empty Line\n");

	}
	
	// Treating an instruction	
	
	else{
		char* token;
		token = strtok(line, " ");

		if (strcmp(token, "mov") == 0 || strcmp(token, "add") == 0 || strcmp(token, "sub") == 0 || strcmp(token, "mul") == 0 || strcmp(token, "div") == 0){
			sscanf(ins, "%s %s %s", instructions[lineNo].ins, instructions[lineNo].src, instructions[lineNo].des);
			instructions[lineNo].line = -1;
			printf("%s %s %s\n", instructions[lineNo].ins, instructions[lineNo].src, instructions[lineNo].des);
		}
		else if (strcmp(token, "read") == 0 || strcmp(token, "print") == 0){
			sscanf(ins, "%s %s", instructions[lineNo].ins, instructions[lineNo].src);
			instructions[lineNo].line = -1;
			strcpy(instructions[lineNo].des, "");
			printf("%s %s\n", instructions[lineNo].ins, instructions[lineNo].src);
		}
		else if (strcmp(token, "jmp") == 0){
			sscanf(ins, "%s %d", instructions[lineNo].ins, &instructions[lineNo].line);
			strcpy(instructions[lineNo].src, "");
			strcpy(instructions[lineNo].des, "");
			printf("%s %d\n", instructions[lineNo].ins, instructions[lineNo].line);
		}
		else if (strcmp(token, "je") == 0 || strcmp(token, "jne") == 0 || strcmp(token, "jg") == 0 || strcmp(token, "jge") == 0 || strcmp(token, "jl") == 0 || strcmp(token, "jle") == 0){
			sscanf(ins, "%s %d %s %s", instructions[lineNo].ins, &instructions[lineNo].line, instructions[lineNo].src, instructions[lineNo].des);
			printf("%s %d %s %s\n", instructions[lineNo].ins, instructions[lineNo].line, instructions[lineNo].src, instructions[lineNo].des);
		}

	}
	lineNo++;
	}
	fclose(fp);
	
	struct regr* registry = NULL;
	int maxlines = lineNo;
	lineNo = 0;

	while(lineNo <= maxlines){
		
		if(strcmp(instructions[lineNo].ins, "") == 0){
			lineNo++;
		}
		else if(strcmp(instructions[lineNo].ins, "read") == 0){
			int temp = getint();
			
					
	
		}

	}
		
	







	
	




	

}
