/*
 * jogo.c TAD que implementa o jogo de cartas "solitaire".
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

#include <assert.h>
#include <time.h>

#include "jogo.h"
#include "vetor.h"
#include "memo.h"

#define SOLIT_MAGICO 0x50717
#define DESTRUIDO 0x80000000

bool 
jogo_valido(jogo sol)
{
	if (sol == NULL || sol->magico != SOLIT_MAGICO) {
		return false;
	}
	return true;
}

jogo 
jogo_cria(void)
{
	jogo		sol;
	int		i;

	sol = (jogo) memo_aloca(sizeof(jogo_t));
	assert(sol != NULL);
	sol->magico = SOLIT_MAGICO;

	sol->monte = pilha_cria();
	sol->descartes = pilha_cria();
	for (i = 0; i < 4; i++) {
		sol->ases[i] = pilha_cria();
	}
	for (i = 0; i < 7; i++) {
		sol->pilhas[i] = pilha_cria();
	}
	sol->tela = tela_cria();

	return sol;
}

void
jogo_inicia(jogo solit)
{
	srand(time(NULL));

	vetor_t* cartas = vetor_cria();
	vetor_t* cartas_embaralhadas = vetor_cria();

	int i, j;
	carta  c;

	// Criando todas as 13 cartas dos 4 naipes, 52 interações
	for (i = 0; i < 4; i++) {
		for (j = 1; j <= 13; j++) {
			vetor_insere_carta(cartas, (i * 13) + j - 1, carta_cria(j, i));
		}
	}

	// Embaralhando...
	for (i = 0; i < 52; i++) {
		do {
			j = ((int) rand()) % vetor_numelem(cartas);
			c = vetor_remove_carta(cartas, j);
		} while(c == NULL);

		vetor_insere_carta(cartas_embaralhadas, i, c);
	}

	// Adiciona 28 cartas as 7 pilhas
	for (i = 1; i <= 7; i++) {
		for (j = 1; j <= i; j++) {
			c = vetor_remove_carta(cartas_embaralhadas, 0);
			pilha_insere_carta(jogo_pilha(solit, i - 1), c);
		}

		carta_abre(c); // Abre a carta do topo
	}

	// Cartas que restaram das pilhas adicionadas ao monte
	for (i = 0; i < 52 - 28; i++) { 
		pilha_insere_carta(jogo_monte(solit), vetor_remove_carta(cartas_embaralhadas, 0));
	}

}

void
jogo_comando(jogo solit) {
	switch (tela_le(jogo_tela(solit))) {
		
	}
}

static void 
jogo_destroi_pilha(pilha p)
{
	while (!pilha_vazia(p)) {
		carta_destroi(pilha_remove_carta(p));
	}
	pilha_destroi(p);
}

void 
jogo_destroi(jogo sol)
{
	int		i;
	assert(jogo_valido(sol));
	jogo_destroi_pilha(sol->monte);
	jogo_destroi_pilha(sol->descartes);
	for (i = 0; i < 4; i++) {
		jogo_destroi_pilha(sol->ases[i]);
	}
	for (i = 0; i < 7; i++) {
		jogo_destroi_pilha(sol->pilhas[i]);
	}
	tela_destroi(sol->tela);
	sol->magico |= DESTRUIDO;
	memo_libera(sol);
}

/* acessores */
tela 
jogo_tela(jogo sol)
{
	assert(jogo_valido(sol));
	return sol->tela;
}

pilha 
jogo_monte(jogo sol)
{
	assert(jogo_valido(sol));
	return sol->monte;
}

pilha 
jogo_descartes(jogo sol)
{
	assert(jogo_valido(sol));
	return sol->descartes;
}

pilha 
jogo_ases(jogo sol, int i)
{
	assert(jogo_valido(sol));
	assert(i >= 0 && i < 4);
	return sol->ases[i];
}

pilha 
jogo_pilha(jogo sol, int i)
{
	assert(jogo_valido(sol));
	assert(i >= 0 && i < 7);
	return sol->pilhas[i];
}
