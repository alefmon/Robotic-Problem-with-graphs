#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "TAD_grafo.h"
#include "TAD_heap.h"

void Cria_grafo_lista(Grafo G, int num_vertices){
	int i = 0;	
	// Aloca memória e inicializa os valores para um novo grafo G.
	for(i = 1; i <= num_vertices; i++){
		G[i] = (vertice*) malloc(sizeof(vertice));
		G[i]->flag = 0;
		G[i]->num_vizinhos = 0;
		G[i]->peso_aresta = -1;
		G[i]->peso_origem = -1;
		G[i]->campo = i;
		G[i]->prox = NULL;
		G[i]->antecessor = NULL;
	}
}

void Insere_aresta(Grafo G, int A, int B, int peso){
	vertice *nova_aresta;
	vertice *aux;
	// Aloca memória e inicializa os valores para uma nova aresta A-B.
	nova_aresta = (vertice*) malloc((int)sizeof(vertice)); 
	nova_aresta->flag = 0;
	nova_aresta->num_vizinhos = 0;
	nova_aresta->peso_aresta = peso;
	nova_aresta->peso_origem = -1;
	nova_aresta->campo = B;
	nova_aresta->prox = NULL;
	nova_aresta->antecessor = NULL;   
	G[A]->num_vizinhos++;

	// Insere a nova aresta na primeira posição da lista.
	aux = G[A]->prox;	
	G[A]->prox = nova_aresta;
	nova_aresta->prox = aux;
}

void Relax(Grafo G, vertice* v1, vertice* v2){
	vertice *aux;
	int i = 0, peso = 0;
	aux = v1->prox;
	i = 1;
	// Procura a aresta v1-v2 no grafo.
	while(aux->campo != v2->campo && i<= v1->num_vizinhos){
		aux = aux->prox;
		i++;
	}
	peso = aux->peso_aresta;
	// Se o custo do vértice de origem até v2 pode diminuir passando pela aresta v1-v2, armazena o novo peso da origem até v2 no vértice v2.
	if(v2->peso_origem > (v1->peso_origem+ peso)){
		v2->peso_origem = v1->peso_origem + peso;
		v2->antecessor = v1; 
	}
}

void Dijkstra(Grafo G, vertice* vertice_origem, int num_vertices){
	int i = 0, j = 0, tam_heap = 0, posicao_vizinho = 0;
	vertice *prox_fila;
	vertice** heap;
	// Inicializa o peso de todos os vértices até a origem com "infinito".
	for(i = 1; i <= num_vertices; i++){
		G[i]->peso_origem = 2147483646; /* Valor máximo possível para um inteiro. (Representa "infinito") */
		G[i]->antecessor = NULL;
	}
	vertice_origem->peso_origem = 0;
	heap = (vertice**) malloc((num_vertices+1)*sizeof(vertice*));
	// Formar um vetor com o peso acumulado(desde a orgiem) de todos os vértices.
	for(i = 1; i <= num_vertices; i++)
		heap[i] = G[i];
	tam_heap = num_vertices;
	// Construir um heap, que deixará o o vértice com o menor peso na frente.
	build_heap(heap, tam_heap);
	j = 1;
	while(tam_heap > 0){
		// Verifica o vértice com o menor peso.
		prox_fila = heap[j];
		// Marca o vértice com uma flag, para indicar que o peso final do seu caminho mínimo já foi determinado.
		prox_fila->flag = 1;
		// Retira o vértice do heap.
		tam_heap--;
		// Verificar o peso da aresta para cada um dos vértices vizinhos.
		vertice* proximo_vizinho;
		proximo_vizinho = prox_fila->prox;
		if(proximo_vizinho != NULL)
			posicao_vizinho = proximo_vizinho->campo;
		if(prox_fila->peso_origem == 2147483646)
			break;
		for(i = 0; i < prox_fila->num_vizinhos; i++){
			// Se o peso final do do caminho até o vértice ainda não foi determinado, verifica se o peso do caminho mínimo pode ser "relaxado".
			if(G[posicao_vizinho]->flag != 1)
				Relax(G, prox_fila, G[posicao_vizinho]);
			// Verifica qual vértice é o próximo vizinho a ser analisado.
			proximo_vizinho = proximo_vizinho->prox;
			if(proximo_vizinho != NULL)
				posicao_vizinho = proximo_vizinho->campo;
		}
		// Reconstrói o heap.
		build_heap(&(heap[j]), tam_heap);
		j++;
	}
	free(heap);
}


void Mapa_matriz(FILE* input, int **mapa, int M, int N){
	int i = 0, j = 0;
	// Insere o mapa fornecido pela entrada em uma matriz.
	for(i = 0; i < N; i++){
		mapa[i] = (int*) malloc(M*sizeof(int));
		for(j = 0; j < M; j++){
			fscanf(input, "%d", &mapa[i][j]);
		}	
	}
}

void Mapa_grafo(Grafo G, int **mapa, int M, int N, int restricao_x, int restricao_y, int dx_inicial, int dy_inicial, 
	int dx_final, int dy_final){
	int i = 0, j = 0, k = 0, x = 0;
	int num_atalhos = 0, peso = 0;
	int** atalhos;
	atalhos = (int**) malloc(N*M*sizeof(int*));
	// Inserir arestas entre as células que podem representar um movimento do robô.
	for(i = 0; i < N; i++){
		for(j = 0; j < M; j++){
			Insere_aresta_movimento(G, mapa, atalhos, M, N, i, j, &k, restricao_x, restricao_y);
		}
	}
	num_atalhos = k;
	// Inserir arestas entre todos os atalhos.
	for(k = 0; k < num_atalhos; k++){
		for(x = 0; x < num_atalhos; x++){
			i = atalhos[k][0];
			j = atalhos[k][1];
			// O peso da aresta entre dois atalhos é zero.
			peso = 0;
			// Se os atalhos não representam a mesma célula, inserir a aresta entre os atalhos.
			if(atalhos[k][0] != atalhos[x][0] && atalhos[k][1] != atalhos[x][1]){
				Insere_aresta(G, i*M + j +1, M*(atalhos[x][0]) + atalhos[x][1] +1, peso);
			}
		}		
	}
	for(k = 0; k < num_atalhos; k++)
		free(atalhos[k]);
	free(atalhos);
}

void Insere_aresta_movimento(Grafo G, int **mapa, int **atalhos, int M, int N, int i, int j, int* k, int restricao_x, 
	int restricao_y){
	// Se a célula não representa um obstáculo, inserir as arestas a partir dela.
	if(mapa[i][j] != 0){
		// Se não houver restrições ao movimento, insere arestas entre todos os vizinhos.
		if(restricao_x == 0 && restricao_y == 0){
			restricao_x = 1;
			restricao_y = 1;
			Insere_aresta_sem_restricoes(G, mapa, M, N, i, j, restricao_x, restricao_y);
		}
		// Se houver restrições ao movimento.
		else{
			// Insere aresta na diagonal no sentido "para direita e para baixo".
			if((j + restricao_x) < M && (i + restricao_y) < N){
				Insere_aresta_direita_baixo(G, mapa, M, N, i, j, restricao_x, restricao_y);
			}	
			// Insere aresta na diagonal no sentido "para direita e para cima".
			if((j + restricao_x) < M && (i - restricao_y) >= 0){
				Insere_aresta_direita_cima(G, mapa, M, N, i, j, restricao_x, restricao_y);
			}	
			// Insere aresta na diagonal no sentido "para esquerda e para baixo".
			if((j - restricao_x) >= 0 && (i + restricao_y) < N){
				Insere_aresta_esquerda_baixo(G, mapa, M, N, i, j, restricao_x, restricao_y);
			}	
			// Insere aresta na diagonal no sentido "para esquerda e para cima".
			if((j - restricao_x) >= 0 && (i - restricao_y) >= 0){
				Insere_aresta_esquerda_cima(G, mapa, M, N, i, j, restricao_x, restricao_y);
			}	
		}		
	}	
	// Se a célula representa um atalho, armazenar suas coordenadas e seu conteúdo em um vetor.
	if(mapa[i][j] == -1){
		atalhos[*k] = (int*) malloc(3*sizeof(int));
		atalhos[*k][0] = i;
		atalhos[*k][1] = j;
		atalhos[*k][2] = mapa[i][j];
		(*k)++;
	}
}


void Insere_aresta_direita_baixo(Grafo G, int **mapa, int M, int N, int i, int j, int restricao_x, int restricao_y){
	int peso_eixo_x1 = 0, peso_eixo_x2 = 0, peso_eixo_y1 = 0, peso_eixo_y2 = 0;
	// Verifica o peso do caminho que passa primeiro pelo eixo x.
	if(mapa[i][j + restricao_x] != 0)
		peso_eixo_x1 = Calcula_peso_eixo_x(mapa, i, j, restricao_x, 1);
	else
		peso_eixo_x1 = -1;
	if(mapa[i + restricao_y][j + restricao_x] != 0)
		peso_eixo_y1= Calcula_peso_eixo_y(mapa, i , j + restricao_x, restricao_y, 1);
	else
		peso_eixo_y1 = -1;


	// Verifica o peso do caminho que passa primeiro pelo eixo y.
	if(mapa[i + restricao_y][j] != 0)
		peso_eixo_y2= Calcula_peso_eixo_y(mapa, i, j, restricao_y, 1);
	else
		peso_eixo_y2 = -1;
	if(mapa[i + restricao_y][j + restricao_x] != 0)
		peso_eixo_x2 = Calcula_peso_eixo_x(mapa, i + restricao_y, j, restricao_x, 1);
	else
		peso_eixo_x2 = -1;


	// Verificar qual dos caminhos tem o menor peso e inserir a aresta entre os véritices com o menor peso.
	if((peso_eixo_x1 != -1 && peso_eixo_y1 != -1) && (peso_eixo_x2 != -1 && peso_eixo_y2 != -1)){	
		if((peso_eixo_x1 + peso_eixo_y1) < (peso_eixo_x2 + peso_eixo_y2))
			Insere_aresta(G, (M*i + j+ 1), (M*(i + restricao_y) + j + 1 + restricao_x), peso_eixo_x1 + peso_eixo_y1);			
		else
			Insere_aresta(G, (M*i + j+ 1), (M*(i + restricao_y) + j + 1 + restricao_x), peso_eixo_x2 + peso_eixo_y2);	
	}
	// Se o caminho que passa primeiro pelo eixo x é o único que não tem obstáculo.
	else if( peso_eixo_x1 != -1 && peso_eixo_y1 != -1 ){
		Insere_aresta(G, (M*i + j+ 1), (M*(i + restricao_y) + j + 1 + restricao_x), peso_eixo_x1 + peso_eixo_y1);			
	}
	// Se o caminho que passa primeiro pelo eixo y é o único que não tem obstáculo.
	else if( peso_eixo_x2 != -1 && peso_eixo_y2 != -1 ){
		Insere_aresta(G, (M*i + j+ 1), (M*(i + restricao_y) + j + 1 + restricao_x), peso_eixo_x2 + peso_eixo_y2);	
	}

}

void Insere_aresta_direita_cima(Grafo G, int **mapa, int M, int N, int i, int j, int restricao_x, int restricao_y){
	int peso_eixo_x1 = 0, peso_eixo_x2 = 0, peso_eixo_y1 = 0, peso_eixo_y2 = 0;
	// Verifica o peso do caminho que passa primeiro pelo eixo x.
	if(mapa[i][j + restricao_x] != 0)
		peso_eixo_x1 = Calcula_peso_eixo_x(mapa, i, j, restricao_x, 1);
	else
		peso_eixo_x1 = -1;
	if(mapa[i - restricao_y][j + restricao_x] != 0)
		peso_eixo_y1= Calcula_peso_eixo_y(mapa, i , j + restricao_x, restricao_y, -1);
	else
		peso_eixo_y1 = -1;

	
	// Verifica o peso do caminho que passa primeiro pelo eixo y.
	if(mapa[i - restricao_y][j] != 0)
		peso_eixo_y2= Calcula_peso_eixo_y(mapa, i, j, restricao_y, -1);
	else
		peso_eixo_y2 = -1;
	if(mapa[i - restricao_y][j + restricao_x] != 0)
		peso_eixo_x2 = Calcula_peso_eixo_x(mapa, i - restricao_y, j, restricao_x, 1);
	else
		peso_eixo_x2 = -1;


	// Verificar qual dos caminhos tem o menor peso e inserir a aresta entre os véritices com o menor peso.
	if((peso_eixo_x1 != -1 && peso_eixo_y1 != -1) && (peso_eixo_x2 != -1 && peso_eixo_y2 != -1)){	
		if((peso_eixo_x1 + peso_eixo_y1) < (peso_eixo_x2 + peso_eixo_y2))
			Insere_aresta(G, (M*i + j+ 1), (M*(i - restricao_y) + j + 1 + restricao_x), peso_eixo_x1 + peso_eixo_y1);			
		else
			Insere_aresta(G, (M*i + j+ 1), (M*(i - restricao_y) + j + 1 + restricao_x), peso_eixo_x2 + peso_eixo_y2);	
	}
	// Se o caminho que passa primeiro pelo eixo x é o único que não tem obstáculo.
	else if( peso_eixo_x1 != -1 && peso_eixo_y1 != -1 ){
		Insere_aresta(G, (M*i + j+ 1), (M*(i - restricao_y) + j + 1 + restricao_x), peso_eixo_x1 + peso_eixo_y1);			
	}
	// Se o caminho que passa primeiro pelo eixo y é o único que não tem obstáculo.
	else if( peso_eixo_x2 != -1 && peso_eixo_y2 != -1 ){
		Insere_aresta(G, (M*i + j+ 1), (M*(i - restricao_y) + j + 1 + restricao_x), peso_eixo_x2 + peso_eixo_y2);	
	}

}

void Insere_aresta_esquerda_baixo(Grafo G, int **mapa, int M, int N, int i, int j, int restricao_x, int restricao_y){
	int peso_eixo_x1 = 0, peso_eixo_x2 = 0, peso_eixo_y1 = 0, peso_eixo_y2 = 0;
	// Verifica o peso do caminho que passa primeiro pelo eixo x.
	if(mapa[i][j - restricao_x] != 0)
		peso_eixo_x1 = Calcula_peso_eixo_x(mapa, i, j, restricao_x, -1);
	else
		peso_eixo_x1 = -1;
	if(mapa[i + restricao_y][j - restricao_x] != 0)
		peso_eixo_y1= Calcula_peso_eixo_y(mapa, i , j - restricao_x, restricao_y, 1);
	else
		peso_eixo_y1 = -1;
	

	// Verifica o peso do caminho que passa primeiro pelo eixo y.
	if(mapa[i + restricao_y][j] != 0)
		peso_eixo_y2= Calcula_peso_eixo_y(mapa, i, j, restricao_y, 1);
	else
		peso_eixo_y2 = -1;
	if(mapa[i + restricao_y][j - restricao_x] != 0)
		peso_eixo_x2 = Calcula_peso_eixo_x(mapa, i + restricao_y, j, restricao_x, -1);
	else
		peso_eixo_x2 = -1;


	// Verificar qual dos caminhos tem o menor peso e inserir a aresta entre os véritices com o menor peso.
	if((peso_eixo_x1 != -1 && peso_eixo_y1 != -1) && (peso_eixo_x2 != -1 && peso_eixo_y2 != -1)){	
		if((peso_eixo_x1 + peso_eixo_y1) < (peso_eixo_x2 + peso_eixo_y2))
			Insere_aresta(G, (M*i + j+ 1), (M*(i + restricao_y) + j + 1 - restricao_x), peso_eixo_x1 + peso_eixo_y1);			
		else
			Insere_aresta(G, (M*i + j+ 1), (M*(i + restricao_y) + j + 1 - restricao_x), peso_eixo_x2 + peso_eixo_y2);	
	}
	// Se o caminho que passa primeiro pelo eixo x é o único que não tem obstáculo.
	else if( peso_eixo_x1 != -1 && peso_eixo_y1 != -1 ){
		Insere_aresta(G, (M*i + j+ 1), (M*(i + restricao_y) + j + 1 - restricao_x), peso_eixo_x1 + peso_eixo_y1);			
	}
	// Se o caminho que passa primeiro pelo eixo y é o único que não tem obstáculo.
	else if( peso_eixo_x2 != -1 && peso_eixo_y2 != -1 ){
		Insere_aresta(G, (M*i + j+ 1), (M*(i + restricao_y) + j + 1 - restricao_x), peso_eixo_x2 + peso_eixo_y2);	
	}

}

void Insere_aresta_esquerda_cima(Grafo G, int **mapa, int M, int N, int i, int j, int restricao_x, int restricao_y){
	int peso_eixo_x1=0, peso_eixo_x2=0, peso_eixo_y1=0, peso_eixo_y2=0;
	// Verifica o peso do caminho que passa primeiro pelo eixo x.
	if(mapa[i][j - restricao_x] != 0)
		peso_eixo_x1 = Calcula_peso_eixo_x(mapa, i, j, restricao_x, -1);
	else
		peso_eixo_x1 = -1;
	if(mapa[i - restricao_y][j - restricao_x] != 0)
		peso_eixo_y1 = Calcula_peso_eixo_y(mapa, i , j - restricao_x, restricao_y, -1);
	else
		peso_eixo_y1 = -1;
	

	// Verifica o peso do caminho que passa primeiro pelo eixo y.
	if(mapa[i - restricao_y][j] != 0)
		peso_eixo_y2= Calcula_peso_eixo_y(mapa, i, j, restricao_y, -1);
	else
		peso_eixo_y2 = -1;
	if(mapa[i - restricao_y][j - restricao_x] != 0)
		peso_eixo_x2 = Calcula_peso_eixo_x(mapa, i - restricao_y, j, restricao_x, -1);
	else
		peso_eixo_x2 = -1;


	// Verificar qual dos caminhos tem o menor peso e inserir a aresta entre os véritices com o menor peso.
	if((peso_eixo_x1 != -1 && peso_eixo_y1 != -1) && (peso_eixo_x2 != -1 && peso_eixo_y2 != -1)){	
		if((peso_eixo_x1 + peso_eixo_y1) < (peso_eixo_x2 + peso_eixo_y2))
			Insere_aresta(G, (M*i + j+ 1), (M*(i - restricao_y) + j + 1 - restricao_x), peso_eixo_x1 + peso_eixo_y1);			
		else
			Insere_aresta(G, (M*i + j+ 1), (M*(i - restricao_y) + j + 1 - restricao_x), peso_eixo_x2 + peso_eixo_y2);	
	}
	// Se o caminho que passa primeiro pelo eixo x é o único que não tem obstáculo.
	else if( peso_eixo_x1 != -1 && peso_eixo_y1 != -1 ){
		Insere_aresta(G, (M*i + j+ 1), (M*(i - restricao_y) + j + 1 - restricao_x), peso_eixo_x1 + peso_eixo_y1);			
	}
	// Se o caminho que passa primeiro pelo eixo y é o único que não tem obstáculo.
	else if( peso_eixo_x2 != -1 && peso_eixo_y2 != -1 ){
		Insere_aresta(G, (M*i + j+ 1), (M*(i - restricao_y) + j + 1 - restricao_x), peso_eixo_x2 + peso_eixo_y2);	
	}
}

void Insere_aresta_sem_restricoes(Grafo G, int **mapa, int M, int N, int i, int j, int restricao_x, int restricao_y){
	int peso=0;
	// Insere aresta à direita do vértice, dada a restrição do movimento no eixo x.
	if((j + restricao_x) < M){
		if(mapa[i][j + restricao_x] != 0){
			peso = Calcula_peso_eixo_x(mapa, i, j, restricao_x, 1);
			// Se não houver obstáculo no caminho, insere a aresta.
			if(peso != -1)
				Insere_aresta(G, (M*i + j+ 1), (M*i + j + 1 + restricao_x), peso);
		}	
	}	
	peso = 0;
	// Insere aresta à esquerda do vértice, dada a restrição do movimento no eixo x.
	if((j - restricao_x) > 0){
		if(mapa[i][j - restricao_x] != 0){
			peso = Calcula_peso_eixo_x(mapa, i, j, restricao_x, -1);
			// Se não houver obstáculo no caminho, insere a aresta.
			if(peso != -1)
				Insere_aresta(G, (M*i + j+ 1), (M*i + j + 1 - restricao_x), peso);
		}	
	}	
	peso = 0;
	// Insere aresta abaixo do vértice, dada a restrição do movimento no eixo y.
	if((i + restricao_y) < N){
		if(mapa[i + restricao_y][j] != 0){
			peso = Calcula_peso_eixo_y(mapa, i, j, restricao_y, 1);
			// Se não houver obstáculo no caminho, insere a aresta.
			if(peso != -1)
				Insere_aresta(G, (M*i + j+ 1), (M*(i + restricao_y) + j + 1), peso);
		}	
	}	
	peso = 0;
	// Insere aresta acima do vértice, dada a restrição do movimento no eixo y.
	if((i - restricao_y) > 0){
		if(mapa[i - restricao_y][j] != 0){
			peso = Calcula_peso_eixo_y(mapa, i, j, restricao_x, -1);
			// Se não houver obstáculo no caminho, insere a aresta.
			if(peso != -1)
				Insere_aresta(G, (M*i + j+ 1), (M*(i - restricao_y) + j + 1), peso);
		}	
	}	
}

int Calcula_peso_eixo_x(int **mapa, int i, int j, int restricao_x, int direcao){
	int cont_eixo_x = 1, peso = 0;
	//Se o movimento do robô é no sentido esquerda-direita.
	if(direcao > 0){
		while(cont_eixo_x <= restricao_x){
			if(mapa[i][j + cont_eixo_x - 1] > 0)
				peso+= mapa[i][j + cont_eixo_x - 1];
			if(mapa[i][j + cont_eixo_x] == 0){
				return -1; 
			}
			else if(mapa[i][j + cont_eixo_x] > 0){		
				peso += mapa[i][j+ cont_eixo_x];
				cont_eixo_x++;
			}
			else
				cont_eixo_x++;
		}
		return peso;
	}
	//Se o movimento do robô é no sentido direita-esquerda.
	else{
		while(cont_eixo_x <= restricao_x){
			if(mapa[i][j - cont_eixo_x + 1] > 0)
				peso+= mapa[i][j - cont_eixo_x + 1];
			if(mapa[i][j - cont_eixo_x] == 0){
				return -1; 
			}
			else if(mapa[i][j - cont_eixo_x] > 0){		
				peso += mapa[i][j- cont_eixo_x];
				cont_eixo_x++;
			}
			else
				cont_eixo_x++;
		}
		return peso;
	}	
}
int Calcula_peso_eixo_y(int **mapa, int i, int j, int restricao_y, int direcao){
	int cont_eixo_y=1, peso=0;
	//Se o movimento do robô é no sentido cima-baixo.
	if(direcao > 0){
		while(cont_eixo_y <= restricao_y){
			if(mapa[i + cont_eixo_y - 1][j] > 0)
				peso+= mapa[i + cont_eixo_y - 1][j];
			if(mapa[i + cont_eixo_y][j] == 0){
				return -1; 
			}
			else if(mapa[i + cont_eixo_y][j] > 0){		
				peso += mapa[i + cont_eixo_y][j];
				cont_eixo_y++;
			}
			else
				cont_eixo_y++;
		}
		return peso;		
	}
	//Se o movimento do robô é no sentido baixo-cima.
	else{
		while(cont_eixo_y <= restricao_y){
			if(mapa[i - cont_eixo_y + 1][j] > 0)
				peso+= mapa[i - cont_eixo_y + 1][j];
			if(mapa[i - cont_eixo_y][j] == 0){
				return -1; 
			}
			else if(mapa[i - cont_eixo_y][j] > 0){		
				peso += mapa[i - cont_eixo_y][j];
				cont_eixo_y++;
			}
			else
				cont_eixo_y++;
		}
		return peso;			
	}	
}

void Calcula_peso_minimo(Grafo G, int *peso_caminho_minimo, int restricao_x, int restricao_y, int I, int F, int M, int N){
	// Se não existe caminho entre os vértices, o peso é -1.
	if(G[F]->peso_origem == 0 || G[F]->peso_origem == 2147483646)
		*peso_caminho_minimo = -1;
 	// Se existe caminho entre os vértices, verificar o peso de caminho mínimo.
	else
		*peso_caminho_minimo = G[F]->peso_origem; 
}

void Libera_memoria_grafo(Grafo G, int num_vertices){
	int i = 0;
	for(i = 1; i <= num_vertices; i++){
		vertice *auxiliar;
		auxiliar = G[i];
		// Vai até a última aresta da lista.
		while(auxiliar->prox != NULL){
			auxiliar->prox->antecessor = auxiliar;
			auxiliar = auxiliar->prox;
		}
		//Começa a dar free nas arestas desde a última até a primeira da lista.
		while(auxiliar->antecessor != G[i] && auxiliar != G[i]){
			auxiliar = auxiliar->antecessor;
			free(auxiliar->prox);
		}
		if(auxiliar != G[i])
			free(auxiliar);
		free(G[i]);
	}
	free(G);
}
