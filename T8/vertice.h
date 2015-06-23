#ifndef _VERTICE_H_
#define _VERTICE_H_

#include <stdbool.h>
#include "lista.h"

struct _lista;

typedef struct _vertice {
	char* chave;
	char* nome;
	struct _lista* adjacentes;
} vertice_t;

vertice_t *vertice_cria(char *chave, char *nome);
void vertice_destroi(vertice_t *v);

#endif
