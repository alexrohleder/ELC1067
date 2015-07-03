
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "fila.h"
#include "lista.h"
#include "vertice.h"
#include "memo.h"
#include "grafo.h"

grafo_t* grafo_cria(void)
{
	grafo_t *n = memo_aloca(sizeof(grafo_t));
	n->vertices = lista_cria();
	n->nvertices = 0;
	return n;
}

bool grafo_insere_vertice(grafo_t* g, vertice_t* v)
{
	if (v == NULL || v->chave == NULL) {
		return false;
	}

	g->vertices = lista_insere(g->vertices, v);
	g->nvertices++;

	return true;
}

vertice_t* grafo_busca_vertice(grafo_t* g, char* chave)
{
	lista_t *a = g->vertices;

	while (a != NULL) {
		if (strcmp(a->item->chave, chave) == 0) {
			return a->item;
		}

		a = a->prox;
	}

	printf("%s não é um vértice válido.", chave); exit;
}

bool grafo_insere_aresta(grafo_t* g, char* v1, char* v2)
{
	vertice_t* av1 = grafo_busca_vertice(g, v1);
    	vertice_t* av2 = grafo_busca_vertice(g, v2);

    	av1->adjacentes = lista_insere(av1->adjacentes, av2);
    	av2->adjacentes = lista_insere(av2->adjacentes, av1);

    	return true;
}

void grafo_imprime(grafo_t* g)
{
	lista_t *a = g->vertices;

	while (a != NULL) {
		printf("%s -> ", a->item->chave);
		
		while (a->item->adjacentes != NULL && a->item->adjacentes->item != NULL) {
			printf("%s ", a->item->adjacentes->item->chave);
			a->item->adjacentes = a->item->adjacentes->prox;
		}

		printf("\n");
		a = a->prox;
	}
}

void grafo_busca_largura(grafo_t* g, vertice_t* v)
{
	vertice_t *a;
	fila_t* f = fila_insere(fila_cria(), v);
	lista_t* l;

	while (!fila_vazia(f)) {
		a = fila_remove(f);
		
		for (l = a->adjacentes; l != NULL; l = l->prox) {
			if (l->item->cor == BRANCO) {
				l->item->distancia = a->distancia + 1;
				l->item->cor = CINZA;
				l->item->ant = a;

				f = fila_insere(f, l->item);
			}
		}
		a->cor = PRETO;
	}

	fila_destroi(f);
}

void grafo_caminho_curto(grafo_t* g, char* fonte, char* destino){
	vertice_t* f = grafo_busca_vertice(g, fonte);
	vertice_t* d = grafo_busca_vertice(g, destino);

	if (strcmp(f->chave, d->chave) == 0) {
		printf("%s", f->chave);
	} else {
		printf("%s -> ", d->chave);
	}

	if (f->chave != d->chave) {
		if (d->ant == NULL) {
			printf("Não existe caminho de %s a %s\n", f->nome, d->nome);
		} else {
			grafo_caminho_curto(g, f->chave, d->ant->chave);
		}
	}
}

void grafo_destroi(grafo_t* g)
{
	lista_destroi(g->vertices);
	memo_libera(g);
}
