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
#ifndef _LISTA_H_
#define _LISTA_H_

#include <stdlib.h>
#include "memo.h"

typedef struct _lista {
	char* valor;
	struct _lista* proximo;
	struct _lista* anterior;
} lista_t;

typedef struct _lista_map {
	struct _lista* primeiro;
	struct _lista* ultimo;
} lista_map_t;

lista_t* lista_inicia(void);
lista_t* lista_nesimo(lista_t* l, int n);
lista_t* lista_adiciona(lista_t* l);
lista_t* lista_remove(lista_t* l, int n);
void lista_libera(lista_t* l);
int lista_tamanho(lista_t* l);
char* lista_valor(lista_t* l, int n);
bool lista_valida(lista_t* l);

#endif /* _LISTA_H_ */
