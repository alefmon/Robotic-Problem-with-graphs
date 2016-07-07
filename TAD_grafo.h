#ifndef GRAFO_H
#define GRAFO_H
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// A estrutura é utilizada para representar o vértice de um grafo usando uma lista de adjacências.
typedef struct vertice{
	int flag;
	int campo;
	int peso_aresta;
	int peso_origem;
	int num_vizinhos;
	struct vertice* prox;
	struct vertice* antecessor;	
}vertice;

// A estrutura é utilizada para representar um grafo.
typedef vertice** Grafo; 

// A função aloca memória e inicializa os valores para um novo grafo.
void Cria_grafo_lista(Grafo G, int num_vertices);

// A função insere uma nova aresta A-B no grafo G.
void Insere_aresta(Grafo G, int A, int B, int peso);

// A função utiliza o algoritmo de Dijkstra para calcular o custo mínimo entre o vértice de origem e um outro vértice qualquer.
void Dijkstra(Grafo G, vertice* vertice_origem, int num_vertices);

// A função é utilizada no algoritmo de Djikstra para verificar se o peso de uma aresta diminui o custo mínimo do caminho ou não.
void Relax(Grafo G, vertice* v1, vertice* v2);

// A função coloca o conteúdo do mapa em uma matriz.
void Mapa_matriz(FILE* input, int **mapa, int M, int N);

// A função é utilizada para transformar o mapa fornecido pela entrada em um grafo.
void Mapa_grafo(Grafo G, int **mapa, int M, int N, int restricao_x, int restricao_y, int dx_inicial, int dy_inicial, int dx_final, int dy_final);

// A função insere uma aresta que representa um movimento do robô.
void Insere_aresta_movimento(Grafo G, int **mapa, int **atalhos, int M, int N, int i, int j, int* k, int restricao_x, int restricao_y);

// A função é utilizada na função "Insere_aresta_movimento". Ela insere a aresta quando o movimento do robô está na direção direita-baixo.
void Insere_aresta_direita_baixo(Grafo G, int **mapa, int M, int N, int i, int j, int restricao_x, int restricao_y);

// A função é utilizada na função "Insere_aresta_movimento". Ela insere a aresta quando o movimento do robô está na direção direita-cima.
void Insere_aresta_direita_cima(Grafo G, int **mapa, int M, int N, int i, int j, int restricao_x, int restricao_y);

// A função é utilizada na função "Insere_aresta_movimento". Ela insere a aresta quando o movimento do robô está na direção esquerda-baixo.
void Insere_aresta_esquerda_baixo(Grafo G, int **mapa, int M, int N, int i, int j, int restricao_x, int restricao_y);

// A função é utilizada na função "Insere_aresta_movimento". Ela insere a aresta quando o movimento do robô está na direção esquerda-cima.
void Insere_aresta_esquerda_cima(Grafo G, int **mapa, int M, int N, int i, int j, int restricao_x, int restricao_y);

// A função é utilizada na função "Insere_aresta_movimento". Ela insere as arestas quando o movimento do robô não tem restrições.
void Insere_aresta_sem_restricoes(Grafo G, int **mapa, int M, int N, int i, int j, int restricao_x, int restricao_y);

// A função calcula o peso da aresta que representa o movimento do robô quando ele passa primeiro pelo eixo x.
int Calcula_peso_eixo_x(int **mapa, int i, int j, int restricao_x, int direcao);

// A função calcula o peso da aresta que representa o movimento do robô quando ele passa primeiro pelo eixo y.
int Calcula_peso_eixo_y(int **mapa, int i, int j, int restricao_y, int direcao);

// A função verifica o peso mínimo do caminho entre a posição inicial I e a posição final F do percurso.
void Calcula_peso_minimo(Grafo G, int *peso_caminho_minimo, int restricao_x, int restricao_y, int I, int F, int M, int N);

//A função libera a memória alocada dinâmicamente pelo grafo G.
void Libera_memoria_grafo(Grafo G, int num_vertices);

#endif
