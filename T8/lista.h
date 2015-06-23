#ifndef _LISTA_H_
#define _LISTA_H_

#include <stdbool.h>
#include "vertice.h"

struct _vertice;

typedef struct _lista {
	struct _vertice *item;
	struct _lista *prox;
} lista_t;

lista_t *lista_cria(void);
lista_t *lista_insere(lista_t *l, vertice_t *v);
void lista_destroi(lista_t *l);

#endif
