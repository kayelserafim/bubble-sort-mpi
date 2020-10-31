/*
 ============================================================================
 Name        : parallel_master.c
 Author      : Kayel Serafim
 Version     :
 Copyright   :
 Description : the master / slave model, where the master distributes the tasks.
 ============================================================================
 */

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define DEBUG 0

#define ROW 1000
#define COLUMN 50000

#define PROCESSING_TAG 1
#define SUICIDE_TAG 2

/*
 * Display the vector
 */
void print(int array[]) {
	int i;
	for (i = 0; i < COLUMN; i++) {
		printf("%d ", array[i]);
	}
	printf("\n");
}

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

int main(int argc, char **argv) {
	int my_rank; // Identificador do processo
	int proc_n; // Número de processos
	int source; // Identificador do proc.origem
	int message[COLUMN]; // Buffer para as mensagens
	double t0, t1; // Tempo inicial e final
	t0 = MPI_Wtime();
	int (*matrix)[COLUMN] = malloc(ROW * sizeof *matrix); // saco (Matriz) de trabalho
	MPI_Status status; // Status de retorno

	MPI_Init(&argc, &argv); // função que inicializa o MPI
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank); // pega pega o numero do processo atual (rank)
	MPI_Comm_size(MPI_COMM_WORLD, &proc_n); // pega informação do numero de processos (quantidade total)

	if (my_rank == 0) {
		// papel do mestre, mando o trabalho para os escravos fazerem
		int outgoing_messages = 0;
		int received_messages = 0;
		populate_matrix(matrix);

		while (outgoing_messages < ROW && received_messages < ROW) {
			// Envio as mensagens (vetores)
			for (source = 1; source < proc_n && outgoing_messages < ROW; source++) {
				MPI_Send(matrix[outgoing_messages], COLUMN, MPI_INT, source, PROCESSING_TAG, MPI_COMM_WORLD);
				outgoing_messages++;
			}
			// Recebo as mensagens ordenadas (vetores ordenados)
			for (source = 1; source < proc_n && received_messages < ROW; source++) {
				MPI_Recv(&message, COLUMN, MPI_INT, source, PROCESSING_TAG, MPI_COMM_WORLD, &status);
				received_messages++;
				printf("Source process ID: %d -> ", source);
#if DEBUG
				printf("Mensagens ordenadas:\n");
				print(message);
#endif
				printf("\n");
			}
		}

		// Caso o saco esteja vazio, neste momento envio mensagens de suicídio aos escravos.
		for (source = 1; source < proc_n; source++) {
			MPI_Send(0, 0, MPI_INT, source, SUICIDE_TAG, MPI_COMM_WORLD);
		}
	} else {
		do {
			// recebo do mestre
			MPI_Recv(&message, COLUMN, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
			// Caso receba uma tag de suicídio
			if (status.MPI_TAG == 2) {
				MPI_Finalize();
				return 0;
			}
			bs(COLUMN, message);
			// retorno resultado para o mestre
			MPI_Send(&message, COLUMN, MPI_INT, 0, PROCESSING_TAG, MPI_COMM_WORLD);
		} while (status.MPI_TAG == 1);
	}
	t1 = MPI_Wtime();
	printf("Time %f\n", t1 - t0);
	free(matrix);
	MPI_Finalize();
	return 0;
}
