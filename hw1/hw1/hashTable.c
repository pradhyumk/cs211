#include <stdio.h>
#include <stdlib.h>

// Define the Hash Table Item here

struct HashNode {
	int data;
	struct HashNode *next;
};

int hash(int val) {

	if(val % 10000 >= 0) {
		return (val % 10000);

	} else {
		return ((val % 10000)+10000);
	}

}

void delete(struct HashNode* node) {

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

int main(int argc, char* argv[]) {

	struct HashNode *HashTable[10000];

	for(int k = 0; k < 10000; k++) {
		HashTable[k] = NULL;
	}

	// printf("HashTable initialisation success!\n");

	FILE* fp = fopen(argv[1], "r");

	char action;
	int num, ss = 0, col = 0;

	while(fscanf(fp, "%c	%d\n", &action, &num) != EOF) {

		if(action == 'i') {

			int index = hash(num);

			if(HashTable[index] == NULL) { // If the index doesn't have an element

				HashTable[index] = (struct HashNode*) malloc(sizeof(struct HashNode));
				HashTable[index]->data = num;
				HashTable[index]->next = NULL;

				// printf("Added %d as the head for index %d\n", num, index);

			} else { // Index points to an element and need to invoke collission chaining

				// Create a temporary pointer and traverse till the end of the chain.
				struct HashNode* ptr = HashTable[index];

				while (ptr->next != NULL && ptr->data != num) {

					ptr = ptr->next;
				}


				if(ptr->next == NULL && ptr->data != num) { // ptr contains the reference to the last node in the chain.

					ptr->next = (struct HashNode*) malloc(sizeof(struct HashNode));
					ptr->next->data = num;
					ptr->next->next = NULL;

					// printf("Added %d at the end of a chain for index %d\n", num, index);

				} else {

					// printf("%d was not added to index %d because it was a duplicate\n", num, index);

				}

				col++;

			}

		}

		else if(action == 's') {
			// printf("Action chosen: search for %d (Index: %d)\n", num, hash(num));
			int index = hash(num);

			if(HashTable[index] != NULL) {
				if (HashTable[index]->data == num) { // If index is the queried number

					ss++;

				} else {

					struct HashNode* ptr = HashTable[index];

					while (ptr->next != NULL && ptr->data != num) {

						ptr = ptr->next;
					}


					if(ptr != NULL && ptr->data == num) {
						ss++;
					}
				}
			}
		}
	}


	printf("%d\n%d\n", col, ss);
	fclose(fp);

	for(int k = 0; k < 10000; k++) {
		if(HashTable[k] != NULL) {
			delete(HashTable[k]);
			HashTable[k] = NULL;
		}
	}

}
