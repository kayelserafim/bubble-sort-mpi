# Enunciado TPP1: MPI Mestre/Escravo (ME)

O objetivo do trabalho é implementar, usando a biblioteca MPI, uma versão paralela seguindo o modelo mestre escravo, de um programa que ordena vários vetores usando o algoritmo Bubble Sort (código abaixo). Após implementado, o programa deve ser executado no cluster grad com 2 nós (16 e 32 processadores no total - com e sem HT) para realização das medições de desempenho com 1000 vetores no saco de trabalho, cada um com 100.000 elementos (sem os prints de tela).

O mestre ficará responsável pela gerência do saco de trabalho (uma matriz), distribuindo os vetores para os escravos e recebendo de volta os vetores ordenados e guardando-os novamente no saco (a recepção deve ser por ordem de chegada, ou seja, de quem ficar pronto primeiro). A ordem original dos vetores no saco deve ser mantida. Os escravos por sua vez, receberão os vetores a serem ordenados, realizarão a ordenação com o algoritmo fornecido e retornarão o vetor ordenada para o mestre. A iniciativa deve ser dos escravos, ou seja, os escravos pedem trabalho ao mestre que vai atendendo estas demandas até que o saco esteja vazio. Neste momento são enviadas mensagens de suicídio aos escravos. O programa SPMD deve funcionar para qualquer numero de escravos para permitir uma análise da curva do Speed-UP e Eficiência.

![diagram](docs/MS.gif?raw=true "Funcionamento do modelo mestre escravo na ordenação de vários vetores (saco de trabalho)")

Os itens para avaliação são:

* execução da versão sequencial para medir o tempo total (referência cálculo do fator de aceleração);
* implementação da versão paralela SPMD do algoritmo em C e MPI seguindo o modelo mestre/escravo;
* medição dos tempos de execução para a versão sequencial em uma máquina qualquer do aluno ou laboratório e da versão paralela (usando 2 nós exclusivos da máquina grad totalizando 16 e 32 processos - cada nó possui 8 processadores capazes de executar 16 threads);
* cálculo do speed up e da eficiência para o caso de teste e diferentes números de processadores;
* análise do balanceamento da carga na execução do programa paralelo;
* clareza do código (utilização de comentários e nomes de variáveis adequadas);


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

![diagram](docs/PathsAndSymbols.png?raw=true "Caminho para configurar o build do MPI no Eclipse")


# Exemplo de compilação e execução

**Compilação em máquina local:** mpicc parallel.c -o parallel.o

**Execução em máquina local:** mpirun --hostfile ../hostfile -np 4 ./parallel.o

**Execução no lad:** ladrun -np 16 /sequential.o

**Execução no lad:** ladrun -np 16 /sequential.o


