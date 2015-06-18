
#include <stdlib.h>
#include "vertice.h"
#include "memo.h"
#include "lista.h"

vertice_t *vertice_cria(char *chave, char *nome)
{
	vertice_t *v = memo_aloca(sizeof(vertice_t));
	v->chave = chave;
	v->nome = nome;
	v->adjacentes = lista_cria();
	return v;
}

void vertice_destroi(vertice_t *v)
{
	lista_destroi(v->adjacentes);
	memo_libera(v);
}
