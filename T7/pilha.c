
#include <stdio.h>
#include <stdlib.h>
#include "memo.h"
#include "pilha.h"

pilha_t * pilha_cria(void)
{
    pilha_t* p = memo_aloca(sizeof(pilha_t));
    p->topo = NULL;

    return p;
}

void pilha_destroi(pilha_t* p)
{
	lista_t* l = p->topo;
	
	while(l != NULL){
		memo_libera(l);
		l = l->prox;
	}
	 
	memo_libera(p);
}

bool pilha_vazia(pilha_t* p)
{
    return p == NULL;
}

void pilha_insere(pilha_t* p, arv_t* arv)
{
	lista_t* n = (lista_t*) memo_aloca(sizeof(lista_t));
	
	n->arv = arv;
	n->prox = p->topo;
	
	p->topo = n;
}

arv_t* pilha_remove(pilha_t* p)
{
    arv_t* arv;
    lista_t* a;

    a = p->topo;
    p->topo = a->prox;
    arv = a->arv;

    memo_libera(a);
    return arv;
}
