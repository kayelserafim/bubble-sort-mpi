/*
 ============================================================================
 Name        : sequential.c
 Author      : Kayel Serafim
 Version     :
 Copyright   :
 Description : The sequential version to measure the total time
 ============================================================================
 */
#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#define ROW 10
#define COLUMN 10

/*
 * Swap function definition
 */
void swap(int *a, int *b) {
	int temp = *a;
	*a = *b;
	*b = temp;
}

/*
 * Fill in a matrix according to the number of defined rows and columns.
 */
void populate_matrix(int (*matrix)[COLUMN]) {
	int i, j;
	for (i = 0; i < ROW; i++) {
		for (j = 0; j < COLUMN; j++) {
			matrix[i][j] = rand() % COLUMN;
		}
	}
}

/*
 * Bubble Sort for a vector of integers, reference:
 * https://www.inf.pucrs.br/~pinho/ProgEngII/Exercicios/Struct/ExerStructs2.html
 */
void bubble_sort(int vector[]) {
	int i, j;
	for (i = COLUMN - 1; i > 0; i--) {
		for (j = 0; j < i; j++) {
			if (vector[j] > vector[j + 1]) {
				swap(&vector[j], &vector[j + 1]);
			}
		}
	}
}

/*
 * Sort each row in the matrix
 */
void sort_matrix(int (*matrix)[COLUMN]) {
	int i;
	for (i = 0; i < ROW; i++) {
		bubble_sort(matrix[i]);
	}
}

/*
 * Display the matrix
 */
void print(int (*matrix)[COLUMN]) {
	int i, j;
	for (i = 0; i < ROW; i++) {
		for (j = 0; j < COLUMN; j++) {
			printf("%d ", matrix[i][j]);
		}
		printf("\n");
	}
}

int main() {
	int (*matrix)[COLUMN] = malloc(ROW * sizeof *matrix);
	clock_t begin = clock();

	populate_matrix(matrix);
	printf("\n Display unsorted array \n");
	print(matrix);

	sort_matrix(matrix);
	printf("\n Display sorted array \n");
	print(matrix);

	clock_t end = clock();
	printf("Elapsed: %f seconds\n", (double) (end - begin) / CLOCKS_PER_SEC);
	free(matrix);
	return 0;
}
