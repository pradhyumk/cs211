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
double* multiply2D1DMatrix(double **matA, double *matB, int r1, int c1, int r2);
void clear2DArray(double** mat, int r);
// main method starts here
int main(int argc, char* argv[]) {
	// your code goes here
	if(argc < 3) {
		printf("error\n");
		exit(0);
	}

	int attributes = 0;
	int training_data = 0;
	FILE* fp = fopen(argv[1], "r"); // Opens training data
	fscanf(fp, "%d\n", &attributes);
	fscanf(fp, "%d\n", &training_data);

	if(attributes == 0 && training_data == 0) {
		printf("no training data in the supplied file\n");
		exit(0);
	}

	double** X = malloc(training_data * sizeof(double*));
	double* Y = malloc(training_data * sizeof(double));

	for(int k = 0; k < training_data; k++) {
		X[k] = malloc((attributes + 1) * sizeof(double));
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

	fclose(fp);
	fp = NULL;

	// Calculates the transpose of matrix 'X'
	double** transpose_of_X = transposeMatrix(X, training_data, (attributes + 1));
	// printf("Transpose of 'X'\n");
	// print2DMatrix(transpose_of_X, (attributes + 1), training_data);
	// printf("\n");

	// Calculates the transpose of matrix 'X' times 'X'
	// printf("Transpose of 'X' time 'X'\n");
	double** transpose_of_X_times_X = multiplyMatrix(transpose_of_X, X, (attributes + 1), training_data,  training_data, (attributes + 1));
	// print2DMatrix(transpose_of_X_times_X, (attributes + 1), (attributes + 1));
	// printf("\n");

	// printf("Inverse of Transpose of 'X' time 'X'\n");
	double** inverse = inverseMatrix(transpose_of_X_times_X, (attributes + 1));
	// print2DMatrix(inverse, (attributes + 1), (attributes + 1));
	// printf("\n");

	// printf("First Multiply: \n");
	double** firstmultiply = multiplyMatrix(inverse, transpose_of_X, (attributes + 1), (attributes + 1), (attributes + 1), training_data);
	// print2DMatrix(firstmultiply, (attributes + 1), training_data);
	// printf("\n");

	// printf("Matrix W (Weights)\n");
	double* W = multiply2D1DMatrix(firstmultiply, Y, (attributes + 1), training_data, training_data);
	// print1DMatrix(W, (attributes + 1));
	// printf("\n");

	// now perform calculations to predict prices for sample houses
	fp = fopen(argv[2], "r");
	int test_data = 0;
	fscanf(fp, "%d\n", &test_data);

	if (test_data == 0) {
		printf("no test data in the supplied file\n");
		exit(0);
	}

	// printf("Estimated prices: \n");

	for (int i = 0; i < test_data ; i++) {
		double price = W[0];
		double attr = 0;
		int k = 1;

		while(k < attributes) {
			fscanf(fp, "%lf,", &attr);
			price += (W[k] * attr);
			k++;
		}

		fscanf(fp, "%lf\n", &attr);
		price += (W[k] * attr);
		printf("%0.0lf\n", price);
	}

	fclose(fp);
	fp = NULL;

	free(Y);
	Y = NULL;

	clear2DArray(X, training_data);
	X = NULL;

	clear2DArray(transpose_of_X, (attributes + 1));
	transpose_of_X = NULL;

	clear2DArray(transpose_of_X_times_X, (attributes + 1));
	transpose_of_X_times_X = NULL;

	clear2DArray(inverse, (attributes + 1));
	inverse = NULL;

	clear2DArray(firstmultiply, (attributes + 1));
	firstmultiply = NULL;

	free(W);
	W = NULL;

	return 0;
}

// method for multiply a 2D matrix by a 2D Matrix

double** multiplyMatrix(double **matA, double **matB, int r1, int c1, int r2, int c2) {
	//printf("r1 = %d | c2 = %d\n", r1, c2);
	double** result = malloc(r1 * sizeof(double*));

	for(int k = 0; k < r1; k++) {
		result[k] = malloc(c2 * sizeof(double));
	}

	// Initialising all elements to 0

	for(int r = 0; r < r1; r++) {
		for(int c = 0; c < c2; c++) {
			result[r][c] = 0;
		}
	}

	// should be iterating through the elements of resulting matrix because dimensions depend on r1 and c2

	// res_row iterating through the rows of resulting matrix.
	for(int res_row = 0; res_row < r1; res_row++) {

		// res_col iterating through the columns of the resulting matrix.
		for(int res_col = 0; res_col < c2; res_col++) {
			double product = 0;

			// 'c'  iterates through the row 'res_col' for matA and column 'res_col' for matB
			for(int c = 0; c < c1; c++) {
				product += matA[res_row][c] * matB[c][res_col];
			}

			result[res_row][res_col] = product;
		}
	}

	return result;
}

// Method for multiplying a 2D Matrix with a vector

double* multiply2D1DMatrix(double **matA, double *matB, int r1, int c1, int r2) {
	// printf("r1 = %d | c2 = %d\n", r1, 1);
	double* result = malloc(r1 * sizeof(double));

	// Initialising all elements to 0

	for(int r = 0; r < r1; r++) {
		for(int c = 0; c < 1; c++) {
			result[r] = 0;
		}
	}

	// modified loop of multiply method, but does not need res_col because the result will be a vector.

	for(int res_row = 0; res_row < r1; res_row++) {
		double product = 0;

		for (int c = 0; c < c1; c++) {
			product += matA[res_row][c] * matB[c];
		}

		result[res_row] = product;
	}

	return result;
}


double** transposeMatrix(double** mat, int row, int col) {
	double** matTran = malloc(col * sizeof(double*));

	for(int k = 0; k < col; k++) {
		matTran[k] = malloc(row * sizeof(double));
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
	double** N = malloc(dimension * sizeof(double*));

	for(int k = 0; k < dimension; k++) {
		N[k] = malloc(dimension * sizeof(double*));
	}

	// Initialising Identity Matrix Values:

	for(int r = 0; r < dimension; r++) {
		for(int c = 0; c < dimension; c++) {
			if(r == c) {
				N[r][c] = 1;
			} else {
				N[r][c] = 0;
			}
		}
	}

	// First going to copy values of parameter matrix into new matrix
	double** M = malloc(dimension * sizeof(double*));

	for(int k = 0; k < dimension; k++) {
		M[k] = malloc(dimension * sizeof(double));
	}

	for(int r = 0; r < dimension; r++) {
		for(int c = 0; c < dimension; c++) {
			double temp = matA[r][c];
			M[r][c] = temp;
		}
	}

	// Simplified Gauss Jordan Elimination using Jeff Ames' Pseudocode

	for(int p = 0; p < dimension; p++) {
		double f = M[p][p];

		// divide M_p & N_p by f

		for (int k = 0; k < dimension; k++) {
			double tempM = M[p][k] / f;
			M[p][k] = tempM;
			double tempN = N[p][k] / f;
			N[p][k] = tempN;
		}

		for (int i = p + 1; i < dimension; i++) {
			f = M[i][p];

			// subtract M_p * f from M_i and N_p * f from N_i

			for (int k = 0; k < dimension; k++) {
				double tempM = M[i][k] - (f * M[p][k]);
				double tempN = N[i][k] - (f * N[p][k]);
				M[i][k] = tempM;
				N[i][k] = tempN;
			}
		}
	}

	for (int p = dimension - 1; p >= 0; p--) {
		for (int i = p - 1; i >= 0; i--) {
			double f = M[i][p];

			// subtract M_p * f from M_i & N_p * f from N_i

			for (int k = 0; k < dimension; k++) {
				double tempM = M[i][k] - (f * M[p][k]);
				double tempN = N[i][k] - (f * N[p][k]);
				M[i][k] = tempM;
				N[i][k] = tempN;
			}
		}
	}

	clear2DArray(M, dimension);
	return N;
}

// Clear method for 2D arrays

void clear2DArray(double** mat, int r) {
	for (int i = 0; i < r; ++i) {
		free(mat[i]);
		mat[i] = NULL;
	}

	free(mat);
	mat = NULL;
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

