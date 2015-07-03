
#include <stdbool.h>
#include "fila.h"
#include "lista.h"
#include "vertice.h"
#include "memo.h"

fila_t *fila_cria(void)
{
    fila_t *n = memo_aloca(sizeof(fila_t));
    n->pri = NULL;
    n->ult = NULL;
    return n;
}

fila_t *fila_insere(fila_t *f, vertice_t *v)
{
    lista_t *l = lista_cria();
    l->item = v;

    if (f->ult == NULL) {
        f->pri = l;
    } else {
        f->ult->prox = l;
    }

    f->ult = l;
    return f;
}

vertice_t *fila_remove(fila_t *f)
{
    lista_t *a = f->pri;
    vertice_t *v = a->item;
    f->pri = f->pri->prox;

    if (f->pri == NULL) {
        f->ult = NULL;
    }

    memo_libera(a);
    return v;
}

bool fila_vazia(fila_t *f)
{
    return f->pri == NULL;
}

void fila_destroi(fila_t *f)
{
    memo_libera(f);
}
