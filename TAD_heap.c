#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "TAD_heap.h"

void Min_heap(vertice** vetor, int i, int tam_heap){
	int esquerda = 0, direita = 0, menor = 0;
	esquerda = 2*i;
	direita = 2*i +1;
	vertice* aux;
	if(esquerda <= tam_heap){
		if(vetor[esquerda]->peso_origem < vetor[i]->peso_origem)
			menor = esquerda;
		else
			menor = i;	
	}	
	else
		menor = i;
	if(direita <= tam_heap){ 
		if(vetor[direita]->peso_origem < vetor[menor]->peso_origem)
			menor = direita;
	}	
	if(menor != i){
		// Trocar os elementos de posição.
		aux = vetor[i];
		vetor[i] = vetor[menor];
		vetor[menor] = aux;
		Min_heap(vetor, menor, tam_heap);
	}
}

void build_heap(vertice** vetor, int tam_vetor){
	int i=0;
	// Realizar a manutenção do heap no vetor.
	for(i = (tam_vetor/2); i>=1; i--){
		Min_heap(vetor, i, tam_vetor);
	}
}
