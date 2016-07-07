#ifndef HEAP_H
#define HEAP_H
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "TAD_grafo.h"

// A função transforma um vetor de inteiros em um heap.
void build_heap(vertice** vetor, int tam_vetor);

// A função é utilizada na função anterior para modifica um vetor para que ele tenha as condições de um heap, com o menor valor na primeira posição.
void Min_heap(vertice** vetor, int i, int tam_heap);

#endif
