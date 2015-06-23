
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "lista.h"
#include "vertice.h"
#include "memo.h"

lista_t *lista_cria(void)
{
	lista_t *n = memo_aloca(sizeof(lista_t));
	n->item = NULL;
	n->prox = NULL;
	return n;
}

lista_t *lista_insere(lista_t *l, vertice_t *v)
{
	lista_t *n = lista_cria();
	n->item = v;

	if (l->item == NULL) {
		return n;
	}

	lista_t *a = l;

	while (a->prox != NULL) a = a->prox;

	a->prox = n;
	
	return l;
}

void lista_destroi(lista_t *l)
{
	
}

