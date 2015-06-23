
#include <stdlib.h>
#include <stdbool.h>
#include "lista.h"
#include "vertice.h"
#include "memo.h"

vertice_t *vertice_cria(char *chave, char *nome)
{
	vertice_t *n = memo_aloca(sizeof(vertice_t));
	n->adjacentes = lista_cria();
	n->chave = chave;
	n->nome = nome;
	return n;
}
