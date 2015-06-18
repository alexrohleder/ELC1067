
#include <stdlib.h>
#include <stdbool.h>
#include "grafo.h"
#include "memo.h"

grafo_t *grafo_cria(void)
{
	grafo_t *g = memo_aloca(sizeof(grafo_t));
	g->vertices = lista_inicia();
	g->nvertices = 0;
	return g;
}

bool grafo_insere_vertice(grafo_t *g, vertice_t *v)
{
	lista_adiciona(g->vertices, v);

	return true;
}

vertice_t *grafo_busca_vertice(grafo_t *g, char *chave)
{
	int i;

	for (i = 0; i < g->nvertices; i++) {
		vertice_t *v = lista_enesimo(g->vertices, i);
		if (v->chave == chave) {
			return v;
		}
	}

	return NULL;
}

bool grafo_insere_aresta(grafo_t *g, char *v1, char *v2)
{
	vertice_t *v = grafo_busca_vertice(g, v1);
	vertice_t *a = grafo_busca_vertice(g, v2);

	if (v == NULL || a == NULL) {
		return false;
	}

	lista_adiciona(v->adjacentes, a);

	return true;
}

void grafo_imprime(grafo_t *g)
{
	int i, j;

	for (i = 0; i < g->nvertices; i++) {
		vertice_t v = lista_vertice(g->vertices, i);
		int vs = lista_tamanho(v->adjacentes);

		printf("%s -> ", v->chave);

		for (j = 0; j < vs; j++) {
			printf("%s ". lista_chave(v->adjacentes, j);
		}

		printf("\n");
	}
}

void grafo_destroi(grafo_t *g)
{
	lista_destroi(g->vertices);
	memo_libera(g);
}
