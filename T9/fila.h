#ifndef _FILA_H_
#define _FILA_H_

#include <stdbool.h>
#include "lista.h"
#include "vertice.h"

typedef struct _fila {
    lista_t *pri;
    lista_t *ult;
} fila_t;

fila_t *fila_cria(void);
fila_t *fila_insere(fila_t *f, vertice_t *v);
vertice_t *fila_remove(fila_t *f);
bool fila_vazia(fila_t *f);
void fila_destroi(fila_t *f);

#endif