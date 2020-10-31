/*
 ============================================================================
 Name        : parallel_slave.c
 Author      : Kayel Serafim
 Version     :
 Copyright   :
 Description : the master / slave model, where the slave who requests tasks.
 ============================================================================
 */

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define DEBUG 0

#define ROW 1000
#define COLUMN 50000

#define TAG_RESULT 0
#define TAG_ASK_FOR_JOB 1
#define TAG_JOB_DATA 2
#define TAG_SUICIDE 3

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
		int outgoing_messages = 0;
		int received_messages = 0;
		populate_matrix(matrix);

		while (outgoing_messages < ROW && received_messages < ROW) {
			MPI_Recv(&message, COLUMN, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
			int slave_rank = status.MPI_SOURCE;
			int slave_tag = status.MPI_TAG;

			// Esperamos a solicitação de tarefa por um escravo
			if (slave_tag == TAG_ASK_FOR_JOB) {
				MPI_Send(matrix[outgoing_messages], COLUMN, MPI_INT, slave_rank, TAG_JOB_DATA, MPI_COMM_WORLD);
				outgoing_messages++;
			}
			// Recebo as mensagens ordenadas (vetores ordenados)
			if (slave_tag == TAG_RESULT) {
				received_messages++;
				printf("Source process ID: %d -> ", slave_rank);
#if DEBUG
				printf("Mensagens ordenadas:\n");
				print(message);
#endif
				printf("\n");
			}
		}

		// Caso o saco esteja vazio, neste momento envio mensagens de suicídio aos escravos.
		for (source = 1; source < proc_n; source++) {
			MPI_Send(0, 0, MPI_INT, source, TAG_SUICIDE, MPI_COMM_WORLD);
		}
	} else {
		while (1) {
			// Aqui nós enviamos uma mensagem para o mestre pedindo um trabalho
			MPI_Send(&message, COLUMN, MPI_INT, 0, TAG_ASK_FOR_JOB, MPI_COMM_WORLD);

			// Recebo do mestre
			MPI_Recv(&message, COLUMN, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

			// Caso receba uma tag de suicídio
			if (status.MPI_TAG == TAG_SUICIDE) {
				MPI_Finalize();
				return 0;
			}
			// Caso recebamos uma tarefa do mestre
			if (status.MPI_TAG == TAG_JOB_DATA) {
				bs(COLUMN, message);
				// retorno resultado para o mestre
				MPI_Send(&message, COLUMN, MPI_INT, 0, TAG_RESULT, MPI_COMM_WORLD);
			}
		}
	}
	t1 = MPI_Wtime();
	printf("Time %f\n", t1 - t0);
	free(matrix);
	MPI_Finalize();
	return 0;
}
