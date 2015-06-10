
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#include "arv.h"
#include "memo.h"

arv_t* arv_cria(op_t op)
{
	arv_t *n = (arv_t*) memo_aloca(sizeof(arv_t));
	n->dado = op;

	return n;
}

arv_t* arv_insere_esquerda(arv_t* arv, arv_t* filho)
{
	arv->esq = filho;

	return arv;
}

arv_t* arv_insere_direita(arv_t* arv, arv_t* filho)
{
	arv->dir = filho;

	return arv;
}

void arv_imprime(arv_t *arv)
{
	if (arv->dado.tipo == OPERADOR) {
		printf(" %c ", arv->dado.u.operador);
	} else {
		printf(" %.2f ", arv->dado.u.operando);
	}
}

void arv_imprime_pre_ordem(arv_t* arv)
{
	if (arv != NULL) {
		arv_imprime(arv);
		arv_imprime_pre_ordem(arv->esq);
		arv_imprime_pre_ordem(arv->dir);
	}
}

void arv_imprime_em_ordem(arv_t* arv)
{
	if (arv != NULL) {
		arv_imprime_em_ordem(arv->esq);
		arv_imprime(arv);
		arv_imprime_em_ordem(arv->dir);
	}
}

void arv_imprime_pos_ordem(arv_t* arv)
{
	if (arv != NULL) {
		arv_imprime_pos_ordem(arv->esq);
		arv_imprime_pos_ordem(arv->dir);
		arv_imprime(arv);
	}
}

float arv_calc_resultado(arv_t* arv)
{
	float esq, dir;

	if (arv->dado.tipo == OPERADOR) {
		esq = arv_calc_resultado(arv->esq);
		dir = arv_calc_resultado(arv->dir);

		switch (arv->dado.u.operador) {
			case '+':
				return esq + dir;
			case '-':
				return esq - dir;
			case '/':
				return esq / dir;
			case '*':
				return esq * dir;
			default:
				printf("Operador não suportado encontrado."); exit(-1);
		}
	}

	return arv->dado.u.operando;
}

void arv_imprime_resultado(arv_t* arv)
{
	printf("%.2f", arv_calc_resultado(arv));
}

void arv_destroi(arv_t* arv)
{
	if (arv != NULL) {
		arv_destroi(arv->esq);
		arv_destroi(arv->dir);

		if (arv != NULL) {
			memo_libera(arv);
		}
	}
}
