/*
 * lista.h
 * Funções de TAD lista duplamente encadeada.
 *
 * The MIT License (MIT)
 * 
 * Copyright (c) 2014, 2015 João V. F. Lima, UFSM
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <stdlib.h>
#include <stdbool.h>

#include "lista.h"
#include "memo.h"

lista_t* lista_inicia(void)
{
	lista_t* l = (lista_t*) memo_aloca(sizeof(lista_t));

	l->proximo = NULL;
	l->anterior = NULL;

	l->valor = (char*) memo_aloca(sizeof(char));
	*l->valor = '\0';

	return l;
}

lista_t* lista_nesimo(lista_t* l, int n)
{
	if (lista_valida(l)) {
		lista_t* a = l;
		int i = 0;

		while (a->proximo != NULL && i < n) {
			a = a->proximo; i++;
		}

		   return a;
	} else return NULL;
}

lista_t* lista_adiciona(lista_t* l)
{
	int i      = lista_tamanho(l);
	lista_t* a = lista_nesimo(l, i);
	lista_t* n = lista_inicia();

	n->anterior = a;
	a->proximo  = n;

	return n;
}

lista_t* lista_remove(lista_t* l, int n)
{
	lista_t* u = lista_nesimo(l, n);

	if (u != NULL) {
		lista_t* a = u->anterior;
		lista_t* p = u->proximo;

		a->proximo = p;
		p->anterior = a;

		memo_libera(u);

		   return l;
	} else return u;
}

void lista_libera(lista_t* l)
{
	if (lista_valida(l)) {
		lista_t* p;

		do {
			p = l->proximo;
			
			memo_libera(l->valor);
			memo_libera(l);
		} while (p != NULL);
	}
}

bool lista_valida(lista_t* l)
{
	return l != NULL;
}

int lista_tamanho(lista_t* l)
{
	if (lista_valida(l)) {
		lista_t* a = l;
		int i = 0;

		while (a->proximo != NULL) {
			a = a->proximo; i++;
		}

		   return i;
	} else return 0;
}

char* lista_valor(lista_t* l, int n)
{
	if (lista_valida(l)) {
		lista_t* a = lista_nesimo(l, n);
		return a->valor;
	} else return NULL ;
}
