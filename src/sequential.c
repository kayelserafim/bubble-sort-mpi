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

#define DEBUG 0
#define ROW 1000
#define COLUMN 50000

/*
 * Fills a matrix according to the number of
 * rows and columns defined in the worst case
 */
void populate_matrix(int (*matrix)[COLUMN]) {
	int i, j;
	for (i = 0; i < ROW; i++) {
		for (j = 0; j < COLUMN; j++) {
			matrix[i][j] = COLUMN - j;
		}
	}
}

/*
 * Bubble sort algorithm.
 */
void bs(int n, int *vetor) {
	int c = 0, d, troca, trocou = 1;

	while ((c < (n - 1)) & trocou) {
		trocou = 0;
		for (d = 0; d < n - c - 1; d++)
			if (vetor[d] > vetor[d + 1]) {
				troca = vetor[d];
				vetor[d] = vetor[d + 1];
				vetor[d + 1] = troca;
				trocou = 1;
			}
		c++;
	}
}

/*
 * Sort each row in the matrix
 */
void sort_matrix(int (*matrix)[COLUMN]) {
	int i;
	for (i = 0; i < ROW; i++) {
		bs(COLUMN, matrix[i]);
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

	printf("Populando vetores com inteiros aleatórios... \n");
	populate_matrix(matrix);
	printf("Saco de trabalho populado. \n");

#if DEBUG
	printf("Matriz desordenada:\n");
	print(matrix);
#endif

	printf("Ordenando vetores... \n");
	sort_matrix(matrix);
	printf("Saco de trabalho ordenado. \n");

#if DEBUG
	printf("Matriz ordenada:\n");
	print(matrix);
#endif

	clock_t end = clock();
	printf("Tempo: %f segundos\n", (double) (end - begin) / CLOCKS_PER_SEC);
	free(matrix);
	return 0;
}
