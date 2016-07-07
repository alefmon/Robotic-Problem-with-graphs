#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include "TAD_grafo.h"

int main(int argc, char const *argv[])
{
	int i = 0;
	int M = 0, N = 0, I = 0, F = 0, dx_inicial = 0, dy_inicial = 0;
	int dx_final = 0, dy_final = 0, restricao_x = 0, restricao_y = 0, peso_caminho_minimo = 0;
	int** mapa;
	FILE *input;
	//Verifica os parâmetros passados pelo terminal.
	input = fopen(argv[1],"r");
  	dx_inicial = atoi(argv[2]);
  	dy_inicial = atoi(argv[3]);
  	dx_final = atoi(argv[4]);
  	dy_final = atoi(argv[5]);
  	restricao_x = atoi(argv[6]);
  	restricao_y = atoi(argv[7]);
  	//Verificar o tamanho do mapa.
 	fscanf(input, "%d %d", &M, &N);
 	//Verificar se as posições passadas como parâmetro pertencem ao mapa.
 	if(dx_inicial >= M || dy_inicial >= N || dx_final >= M || dy_final >= N){
 		printf("ERRO! Posição de início ou posição de término do deslocamento não pertencem ao mapa!\n");
 		exit(0);
 	}
 	if(dx_inicial < 0 || dy_inicial < 0 || dx_final < 0 || dy_final < 0){
 		printf("ERRO! Posição de início ou posição de término do deslocamento não pertencem ao mapa!\n");
 		exit(0);
 	}
 	//Colocar o conteúdo do mapa em uma matriz.
 	mapa = (int**) malloc(N*sizeof(int*));
 	Mapa_matriz(input, mapa, M, N);
 	//Se a posição de inicio I e a posição final F forem atalhos, o custo do percurso é 0.
 	if(mapa[dy_inicial][dx_inicial] == -1 && mapa[dy_final][dx_final] == -1)
 		peso_caminho_minimo = 0; 	
 	//Se as posições I e F não forem atalhos, calcular o custo mínimo do percurso.
 	else{
 		//Criar um grafo que receberá as células do mapa em cada um de seus vértices.
	 	Grafo G;
	 	G = (vertice**) malloc((M*N+1)*sizeof(vertice*));
	 	Cria_grafo_lista(G, M*N);
	 	//Transforma o mapa em um grafo.
	 	Mapa_grafo(G, mapa, M, N, restricao_x, restricao_y, dx_inicial, dy_inicial, dx_final, dy_final);
	 	I = dy_inicial*M + dx_inicial + 1;
	 	F = dy_final*M + dx_final + 1;
	 	//Calcula o caminho mínimo entre o início e o fim do percurso do Robô.
	 	Dijkstra(G, G[I], M*N);	
		Calcula_peso_minimo(G, &peso_caminho_minimo, restricao_x, restricao_y, I, F, M, N);
	 	//Liberar a memória alocada dinâmicamente.
		Libera_memoria_grafo(G, M*N);
	}	
 	//Imprime o peso do caminho mínimo entre I e F.
 	printf("%d\n", peso_caminho_minimo);

 	for(i = 0; i < N; i++)
 		free(mapa[i]);
 	free(mapa);
 	fclose(input);
	return 0;
}