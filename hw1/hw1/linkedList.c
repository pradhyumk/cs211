#include <stdio.h>
#include <stdlib.h>

struct Node {
	int data;
	struct Node *next;
};

void delete(struct Node* node) {
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

int main(int argc, char* argv[]) {

	if (argc == 1) {
		printf("error\n");
		exit(0);
	}

	char action;
	int num;
	struct Node* root = NULL;
	struct Node* prev = NULL;
	struct Node* ptr = NULL;

	FILE* fp = fopen(argv[1], "r");

	while(fscanf(fp, "%c	%d\n", &action, &num) != EOF) {

		// printf("Action: %c | Num: %d\n", action, num);

		if(action == 'i') { // action indicates to insert
			// printf("Entered Insert Mode\n");
			if(root == NULL) { // Empty Node
				root = (struct Node*) malloc(sizeof(struct Node));
				root->data = num;
				root->next = NULL;
			} else { // Existing list

				//Traverse ptr and prev to appropriate locations

				prev = NULL;
				ptr = root;

				while(ptr != NULL && ptr->data < num) {
					prev = ptr;
					ptr = ptr->next;
				}

				if(ptr == root) { // If number should be placed before root
					struct Node* temp = (struct Node*) malloc(sizeof(struct Node));
					temp->data = num;
					temp->next = root;
					root = temp;
					temp = NULL;
				} else if(ptr != NULL && prev != NULL) { // In between two elements
					struct Node* temp = (struct Node*) malloc(sizeof(struct Node));
					temp->data = num;
					prev->next = temp;
					temp->next = ptr;
					temp = NULL;
				} else if(ptr == NULL && prev != NULL) { // At the end of the list
					struct Node* temp = (struct Node*) malloc(sizeof(struct Node));
					temp->data = num;
					temp->next = NULL;
					prev->next = temp;
					temp = NULL;
				}
			}

		} else { // action indicates to delete specified number or ignore if not found.
			// printf("Entered Delete Mode\n");

			ptr = root;
			if(root != NULL && root->data == num) {
				root = ptr->next;
				free(ptr);
				ptr = NULL;
			} else {
				prev = NULL;
				while(ptr != NULL && ptr->data != num) { // Find the key to delete
					prev = ptr;
					ptr = ptr->next;
				} // ptr will hold the key, prev will be the previous node.

				if(ptr != NULL) {
					if (ptr->next == NULL) {
						prev->next = NULL;
					} else {
						prev->next = ptr->next;
					}
					free(ptr);
					ptr = NULL;

				}
			}
		}
	}

	fclose(fp);
	ptr = NULL;
	ptr = root;

	// Count number of nodes

	int c = 0;
	ptr = root;
	while (ptr != NULL) {
		c++;
		ptr = ptr->next;
	}

	printf("%d\n", c);

	// Print
	ptr = root;
	prev = NULL;

	while(ptr != NULL) {
		if(prev == NULL && ptr != NULL) {
			printf("%d	", ptr->data);
		} else if(prev->data != ptr->data)
			printf("%d	", ptr->data);

		prev = ptr;
		ptr = ptr->next;
	}
	printf("\n");

	// Clearing linked list
	/* while(ptr != NULL) {
		struct Node* temp;
		temp = ptr;
		ptr = ptr->next;
		free(temp);
	}*/

	delete(root);

	//free(root);
	// free(prev);
	//free(ptr);

	root = NULL;
	prev = NULL;
	ptr = NULL;
}
