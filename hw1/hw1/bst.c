#include <stdio.h>
#include <stdlib.h>

struct BSTNode {

	int data;
	struct BSTNode *left;
	struct BSTNode *right;

};

// Inserts value into the tree if value does not already exist.

struct BSTNode* createBSTNode(int num) {

	struct BSTNode* temp = malloc(sizeof(struct BSTNode));
	temp->data = num;
	temp->left = NULL;
	temp->right = NULL;
	return temp;

}

struct BSTNode* insert(struct BSTNode *root, int num) {

	if(root == NULL) {
		struct BSTNode* temp = createBSTNode(num);
		return temp;
	}

	if(root->data > num) {
		// Recurses to the left side
		root->left = insert(root->left, num);
	} else {
		// Recurses to the right side
		root->right = insert(root->right, num);
	}

	return root;

}

// Method returns 1 if value exists and 0 otherwise.

int ifExists(struct BSTNode *root, int num) {

	if(root == NULL) {
		return 0;
	}


	if(root->data == num) {
		return 1;
	} else {
		if(num < root->data) {

			if(root->left == NULL) {
				return 0;

			} else {
				return ifExists(root->left, num);
			}

		} else if (num > root->data) {

			if(root->right == NULL) {
				return 0;
			} else {
				return ifExists(root->right, num);

			}
		}

	}

	return 0;

}

void printBST(struct BSTNode *root) {

	if(root == NULL) {
		return;
	}

	printBST(root->left);
	printf("%d\t", root->data);
	printBST(root->right);

	return;
}

// Does post-order traversal to free all nodes

void clearTree(struct BSTNode *root) {

	if(root == NULL) return;

	// printf("Value of root: %d\n", root->data);

	struct BSTNode *tempLeft = root->left;
	struct BSTNode *tempRight = root->right;

	free(root);
	root = NULL;

	clearTree(tempLeft);
	clearTree(tempRight);
}

// Main method

int main(int argc, char* argv[]) {

	char action;
	int num;

	struct BSTNode *root = NULL;

	FILE* fp = fopen(argv[1], "r");

	while(fscanf(fp, "%c	%d\n", &action, &num) != EOF) {

		if(action == 'i' && ifExists(root, num) == 0) {

			root = insert(root, num);

		}

		// printBST(root);
		// printf("\n");

	}

	fclose(fp);

	printBST(root);

	clearTree(root);

}
