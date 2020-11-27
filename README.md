# Enunciado TPP1: MPI Mestre/Escravo (ME)

O objetivo do trabalho é implementar, usando a biblioteca MPI, uma versão paralela seguindo o modelo mestre escravo, de um programa que ordena vários vetores usando o algoritmo Bubble Sort (código abaixo). Após implementado, o programa deve ser executado no cluster grad com 2 nós (16 e 32 processadores no total - com e sem HT) para realização das medições de desempenho com 1000 vetores no saco de trabalho, cada um com 100.000 elementos (sem os prints de tela).

O mestre ficará responsável pela gerência do saco de trabalho (uma matriz), distribuindo os vetores para os escravos e recebendo de volta os vetores ordenados e guardando-os novamente no saco (a recepção deve ser por ordem de chegada, ou seja, de quem ficar pronto primeiro). A ordem original dos vetores no saco deve ser mantida. Os escravos por sua vez, receberão os vetores a serem ordenados, realizarão a ordenação com o algoritmo fornecido e retornarão o vetor ordenada para o mestre. A iniciativa deve ser dos escravos, ou seja, os escravos pedem trabalho ao mestre que vai atendendo estas demandas até que o saco esteja vazio. Neste momento são enviadas mensagens de suicídio aos escravos. O programa SPMD deve funcionar para qualquer numero de escravos para permitir uma análise da curva do Speed-UP e Eficiência.

![diagram](images/MS.gif?raw=true "Funcionamento do modelo mestre escravo na ordenação de vários vetores (saco de trabalho)")

Os itens para avaliação são:

* execução da versão sequencial para medir o tempo total (referência cálculo do fator de aceleração);
* implementação da versão paralela SPMD do algoritmo em C e MPI seguindo o modelo mestre/escravo;
* medição dos tempos de execução para a versão sequencial em uma máquina qualquer do aluno ou laboratório e da versão paralela (usando 2 nós exclusivos da máquina grad totalizando 16 e 32 processos - cada nó possui 8 processadores capazes de executar 16 threads);
* cálculo do speed up e da eficiência para o caso de teste e diferentes números de processadores;
* análise do balanceamento da carga na execução do programa paralelo;
* clareza do código (utilização de comentários e nomes de variáveis adequadas);

Código do algoritmo Bubble Sort

```
#include <stdio.h>
#include <stdlib.h>

#define DEBUG 1            // comentar esta linha quando for medir tempo
#define ARRAY_SIZE 40      // trabalho final com o valores 10.000, 100.000, 1.000.000

void bs(int n, int * vetor)
{
    int c=0, d, troca, trocou =1;

    while (c < (n-1) & trocou )
        {
        trocou = 0;
        for (d = 0 ; d < n - c - 1; d++)
            if (vetor[d] > vetor[d+1])
                {
                troca      = vetor[d];
                vetor[d]   = vetor[d+1];
                vetor[d+1] = troca;
                trocou = 1;
                }
        c++;
        }
}
```

```
int main()
{
    int vetor[ARRAY_SIZE];
    int i;

    for (i=0 ; i<ARRAY_SIZE; i++)              /* init array with worst case for sorting */
        vetor[i] = ARRAY_SIZE-i;
   

    #ifdef DEBUG
    printf("\nVetor: ");
    for (i=0 ; i<ARRAY_SIZE; i++)              /* print unsorted array */
        printf("[%03d] ", vetor[i]);
    #endif

    bs(ARRAY_SIZE, vetor);                     /* sort array */


    #ifdef DEBUG
    printf("\nVetor: ");
    for (i=0 ; i<ARRAY_SIZE; i++)                              /* print sorted array */
        printf("[%03d] ", vetor[i]);
    #endif

    return 0;
}
```

Pseudocódigo ME básico (orientação no mestre)

```
#define TAREFAS 7 // Numero de tarefas no saco de trabalho para np = 8, processo 0 é o mestre

int my_rank;       // Identificador deste processo
int proc_n;        // Numero de processos disparados pelo usuário na linha de comando (np)
int message;       // Buffer para as mensagens 
int saco[TAREFAS]; // saco de trabalho


MPI_Init(); // funcao que inicializa o MPI, todo o código paralelo esta abaixo

my_rank = MPI_Comm_rank();  // pega pega o numero do processo atual (rank)
proc_n  = MPI_Comm_size();  // pega informação do numero de processos (quantidade total)

if ( my_rank == 0 ) // qual o meu papel: sou o mestre ou um dos escravos?
   {
   // papel do mestre

   for ( i=0 ; i < TAREFAS ; i++) // mando o trabalho para os escravos fazerem
       {
       message = saco[i];
       MPI_Send(&message, i+1); // envio trabalho saco[i] para escravo com id = i+1;
       } 

    // recebo o resultado

    for ( i=0 ; i < TAREFAS ; i++)
        {
        // recebo mensagens de qualquer emissor e com qualquer etiqueta (TAG)

        MPI_Recv(&message, MPI_ANY_SOURCE, MPI_ANY_TAG, status);  // recebo por ordem de chegada com any_source

        saco[status.MPI_SOURCE-1] = message;   // coloco mensagem no saco na posição do escravo emissor
        }
     }              
else               
     {
     // papel do escravo

     MPI_Recv(&message, 0);    // recebo do mestre

     message = message+1;      // icremento conteúdo da mensagem

     MPI_Send(&message, 0);    // retorno resultado para o mestre
     }

MPI_Finalize();
```

# Tamanho do vetor no TPP1

Como o algoritmo Bubble é quadrático, executar os testes com 1000 vetores de 100.000 elementos com poucos processos (1 e 2 por exemplo) acaba demorando bastante. Para facilitar os testes vocês podem usar um vetor um pouco menor, tipo 50.000 que ja deve amenizar a situação. 

Se quiserem escolher algum ponto e testar com um tamanho de vetor diferente pra ver se o fator de aceleração se mantem, pode ser interessante (tipo 16 e 32).

Ex: comparar o fator de aceleração de 1000 vetores de 50000 elementos para 16 e 32 processos (HT em duas máquinas) contra 1000 vetores de 100000 elementos com os mesmos 16 e 32 processos (só destacando que vão precisar medir para 1 processo pra conseguir calcular o fator de aceleração com o novo tamanho de vetor). 


# Instalação do OpenMPI no Ubuntu 20.04

```
$ sudo apt-get update -y
```

```
$ sudo apt-get install -y openmpi-bin
```

# Configuração Eclipse para C/C++

```
$ whereis openmpi
openmpi: /usr/lib/x86_64-linux-gnu/openmpi /etc/openmpi /usr/share/openmpi
```

**OpenMPI path:** /usr/lib/x86_64-linux-gnu/openmpi/include

![diagram](images/PathsAndSymbols.png?raw=true "Caminho para configurar o build do MPI no Eclipse")


# Exemplos de compilação e execução

**Compilação C em máquina local:** gcc sequential.c -o sequential.o

**Execução C em máquina local:** ./sequential.o

**Compilação MPI em máquina local:** mpicc parallel_slave.c -o parallel_slave.o

**Execução MPI em máquina local:** mpirun --hostfile ./hostfile -np 4 ./parallel_slave.o

**Execução no lad:** ladrun -np 16 ./parallel_slave.o

**Execução no lad:** ladrun -np 32 ./parallel_slave.o


