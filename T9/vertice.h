#ifndef _VERTICE_H_
#define _VERTICE_H_

#include <stdbool.h>
#include "lista.h"

struct _lista;

typedef enum {
    BRANCO,
    CINZA,
    PRETO
} cor_t;

typedef struct _vertice {
	char* chave;
	char* nome;
    int distancia;
    cor_t cor;
	struct _lista* adjacentes;
    struct _vertice* ant;
} vertice_t;

vertice_t *vertice_cria(char *chave, char *nome);
void vertice_destroi(vertice_t *v);

#endif
