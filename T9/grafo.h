#ifndef _GRAFO_H_
#define _GRAFO_H_

#include <stdlib.h>
#include <stdbool.h>
#include "vertice.h"
#include "lista.h"

typedef struct {
	lista_t* vertices;
	int nvertices;
} grafo_t;

grafo_t* grafo_cria(void);
bool grafo_insere_vertice(grafo_t* g, vertice_t* v);
vertice_t* grafo_busca_vertice(grafo_t* g, char* chave);
bool grafo_insere_aresta(grafo_t* g, char* v1, char* v2);
void grafo_imprime(grafo_t* g);
void grafo_destroi(grafo_t* g);
void grafo_busca_largura(grafo_t* g, vertice_t* s);
void grafo_caminho_curto(grafo_t* g, char* fonte, char* destino);

#endif
