/*
 * principal.c Esse programa testa as TADs implementadas para um jogo
 * solitário.
 * 
 * The MIT License (MIT)
 * 
 * Copyright (c) 2014, 2015 João V. Lima, UFSM 2005       Benhur Stein, UFSM
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "memo.h"
#include "vetor.h"
#include "jogo.h"

int 
main(int argc, char **argv)
{
	jogo		solit;
	solit = jogo_cria();

	srand(time(NULL));

	vetor_t* cartas = vetor_cria();
	vetor_t* cartas_embaralhadas = vetor_cria();

	int i, j;
	carta  c;

	// Criando todas as 13 cartas dos 4 naipes
	for (i = 0; i < 4; i++) {
		for (j = 1; j <= 13; j++) {
			vetor_insere_carta(cartas, (i * 13) + j - 1, carta_cria(i, j));
		}
	}

	// Embaralhando...
	for (i = 0; i < vetor_numelem(cartas); i++) {
		j = ((int) rand()) % vetor_numelem(cartas);
		c = vetor_remove_carta(cartas, j);
		vetor_insere_carta(cartas_embaralhadas, i, c);
	}

	// Adiciona 28 cartas as 7 pilhas
	for (i = 1; i <= 7; i++) {
		for (j = 1; j <= i; j++) {
			c = vetor_remove_carta(cartas_embaralhadas, 0);

			if (j == i) {
				carta_abre(c);
			}

			pilha_insere_carta(jogo_pilha(solit, i - 1), c);
		}
	}

	// Cartas que restaram das pilhas adicionadas ao monte
	for (i = 0; i < 52 - 28; i++) { 
		pilha_insere_carta(jogo_monte(solit), vetor_remove_carta(cartas_embaralhadas, 0));
	}

	jogo_desenha(solit);
	while (!pilha_vazia(jogo_monte(solit))) {
		carta		c;
		tela_le(jogo_tela(solit));

		c = pilha_remove_carta(jogo_monte(solit));
		carta_abre(c);
		pilha_insere_carta(jogo_pilha(solit, 3), c);

		jogo_desenha(solit);
	}
	tela_le(jogo_tela(solit));
	jogo_destroi(solit);

	/* relatório de memória */
	memo_relatorio();
	return 0;
}
