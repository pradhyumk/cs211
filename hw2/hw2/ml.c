/*
 *
 * This code calculates the house price of a house by learing from
 * training data. It uses pseudo inverse of a given matrix to find the
 * weight of different features.
 *
 * Predicted Price : Y = W0 + W1*x1 + W2*X2 + W3*X3 + W4*X4
 * Weight Matrix : W = pseudoInv(X)*Y
 * pseudoInv(X) = inverse(transpose(X)*X) * transpose(X)
 *
 * weight(w) = pseudoInv(X) * Y
 * 			where	X = Input data matrix
 * 					Y = Target vector
 *
 */

#include<stdio.h>
#include<stdlib.h>

// all methods declarations
double** multiplyMatrix(double **matA, double **matB, int r1, int c1, int r2, int c2);
double** transposeMatrix(double** mat, int row, int col);
double** inverseMatrix(double **matA, int dimension);
void print2DMatrix(double** mat, int r, int c);
void print1DMatrix(double* mat, int length);

// main method starts here
int main(int argc, char* argv[]) {
	// your code goes here
	if(argc < 2) { // change to 3
		printf("error");
		exit(0);
	}

	int attributes = 0;
	int training_data = 0;
	FILE* fp = fopen(argv[1], "r"); // Opens training data

	fscanf(fp, "%d\n", &attributes);
	fscanf(fp, "%d\n", &training_data);

	double** X = malloc(training_data * sizeof(double));
	double* Y = malloc(training_data * sizeof(double*));

	for(int k = 0; k < training_data; k++) {
		X[k] = malloc((attributes + 1) * sizeof(double*));
	}

	int j = 0;

	while(j < training_data) {
		int y = 0;
		X[j][y] = 1;
		y = 1;

		while(y <= attributes) {
			fscanf(fp, "%lf,", &X[j][y]);
			y++;
		}

		fscanf(fp, "%lf\n", &Y[j]);
		j++;
	}

	// Test - Print 2D Array Values
	printf("Attributes: %d | Training Data: %d\n\nX (Attributes):\n", attributes, training_data);
	print2DMatrix(X, training_data, (attributes + 1));
	printf("\nY (Prices):\n");
	print1DMatrix(Y, training_data);
	printf("\nX^T (Transpose of X):\n");
	print2DMatrix(transposeMatrix(X, training_data, (attributes + 1)), (attributes + 1), training_data);
	fclose(fp);
	return 0;
}

double** multiplyMatrix(double **matA, double **matB, int r1, int c1, int r2, int c2) {
	double** result = malloc(r1 * sizeof(double*));
	// your code goes here
	return result;
}


double** transposeMatrix(double** mat, int row, int col) {
	double** matTran = malloc(col * sizeof(double*));

	for(int k = 0; k < col; k++) {
		matTran[k] = malloc(row * sizeof(double*));
	}

	for(int r = 0; r < col; r++) {
		for(int c = 0; c < row; c++) {
			double t = mat[c][r];
			matTran[r][c] = t;
		}
	}

	return matTran;
}


double** inverseMatrix(double **matA, int dimension) {
	double** matI = malloc(dimension * sizeof(double*));
	// your code goes here
	return matI;
}

// Debug Methods

void print2DMatrix(double** mat, int r, int c) {
	for(int x = 0; x < r; x++) {
		for(int y = 0; y < c; y++) {
			printf("%lf	", mat[x][y]);
		}

		printf("\n");
	}
}

void print1DMatrix(double* mat, int length) {
	for(int x = 0; x < length; x++) {
		printf("%lf\n", mat[x]);
	}
}
