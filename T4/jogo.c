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
	jogo  sol;
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

/**
 * -------------------------------------------------------------------------
 */

int
jogo_verifica_imediato(carta c1, carta c2, int mesmo_naipe) {
	if (carta_valor(c1) == carta_valor(c2) - 1) {
		if (mesmo_naipe == 0 && carta_naipe(c1) != carta_naipe(c2)) {
			return 1;
		}
	}

	return 0;
}

void
jogo_corrige_pilha(pilha p) {
	if (!pilha_vazia(p)) {
		pilha_insere_carta(p, 
			carta_abre(
				pilha_remove_carta(p)
			)
		);
	}
}

void
jogo_carta_para_ases(jogo solit, carta c) {
	pilha p;
	int   i = 0;

	if (carta_valor(c) == 1) {
		do {
			p = jogo_ases(solit, i); i++;
		} while (p == NULL);

		pilha_insere_carta(p, c);
	} else {
		carta cp;
		int    b;
		
		do {
			p  = jogo_ases(solit, i); i++;
			cp = pilha_remove_carta(p);
			b  = jogo_verifica_imediato(c, cp, 1);
		} while (i < 4 && (p == NULL || b != 1));

		if (b != 1) {
			printw("\nAinda não é possível inserir esta carta a pilha de ases.\n");
		} else {
			pilha_insere_carta(p, cp);
			pilha_insere_carta(p, c);
		}
	}
}

void
jogo_inicia(jogo solit)
{
	srand(time(NULL));

	vetor_t* cartas = vetor_cria();
	vetor_t* cartas_embaralhadas = vetor_cria();

	int i, j;
	carta  c;

	for (i = 0; i < 4; i++) {
		for (j = 1; j <= 13; j++) {
			vetor_insere_carta(cartas, (i * 13) + j - 1, carta_cria(j, i));
		}
	}

	for (i = 0; i < 52; i++) {
		do {
			j = ((int) rand()) % vetor_numelem(cartas);
			c = vetor_remove_carta(cartas, j);
		} while(c == NULL);

		vetor_insere_carta(cartas_embaralhadas, i, c);
	}

	for (i = 1; i <= 7; i++) {
		for (j = 1; j <= i; j++) {
			c = vetor_remove_carta(cartas_embaralhadas, 0);
			pilha_insere_carta(jogo_pilha(solit, i - 1), c);
		}

		carta_abre(c);
	}

	for (i = 0; i < 52 - 28; i++) { 
		pilha_insere_carta(jogo_monte(solit), vetor_remove_carta(cartas_embaralhadas, 0));
	}
}

void
jogo_monte_para_descarte(jogo solit) {
	carta c = pilha_remove_carta(jogo_monte(solit));
			  pilha_insere_carta(jogo_descartes(solit), carta_abre(c));
}

void
jogo_descarte_para_monte(jogo solit) {
	carta c;
	while (!pilha_vazia(jogo_descartes(solit))) {
		c = pilha_remove_carta(jogo_descartes(solit));
			pilha_insere_carta(jogo_monte(solit), carta_abre(c));
	}
}

void
jogo_descarte_para_pilha(jogo solit, pilha p) {
	carta c = pilha_remove_carta(jogo_descartes(solit));
	// Se a carta for um rei só pode ser inserida em uma pilha vazia.
	if (c == 13 && pilha_vazia(p)) {
		pilha_insere_carta(p, c);
	} else {
		carta t = pilha_remove_carta(p);
		// Caso a carta seja imediatamente inferior e de naipe diferente
		if (jogo_verifica_imediato(c, t, 0) == 1) {
			pilha_insere_carta(p, t);
			pilha_insere_carta(p, c);
		} else {
			pilha_insere_carta(p, t);
			printw("\nVocê deve colocar uma carta imediatamente inferior e de naipe diferente neste monte!\n");
		}
	}
}

void
jogo_descarte_para_ases(jogo solit) {
	carta c = pilha_remove_carta(jogo_descartes(solit));
	jogo_carta_para_ases(solit, c);
}

void
jogo_pilha_para_pilha(pilha p1, pilha p2) {
	carta c1 = pilha_remove_carta(p1);

	if (carta_valor(c1) == 13 && pilha_vazia(p2)) {
		pilha_insere_carta(p2, c1);
	} else {
		carta c2 = pilha_remove_carta(p2);

		if (jogo_verifica_imediato(c1, c2, 0) == 1) {
			pilha_insere_carta(p2, c2);
			pilha_insere_carta(p2, c1);
			jogo_corrige_pilha(p1); // Abre a última carta de p1
		} else {
			pilha_insere_carta(p2, c2);
			pilha_insere_carta(p1, c1);
			printw("\nVocê deve colocar uma carta imediatamente inferior e de naipe diferente nesta pilha!\n");
		}
	}
}

void
jogo_pilha_para_ases(jogo solit, pilha p) {
	carta c = pilha_remove_carta(p);
	jogo_carta_para_ases(solit, c);
	jogo_corrige_pilha(p);
}

void
jogo_pilha_para_descarte(jogo solit, pilha p) {
	carta c = pilha_remove_carta(p);
			  pilha_insere_carta(jogo_descartes(solit), c);
  	jogo_corrige_pilha(p);
}

void
jogo_comando(jogo solit) {
	switch (tela_le(jogo_tela(solit))) {
		case ' ':
			if (pilha_vazia(jogo_monte(solit))) {
				   jogo_monte_para_descarte(solit);
			} else jogo_descarte_para_monte(solit);

			break;
		case 'q':
			jogo_descarte_para_pilha(solit, 
				jogo_pilha(solit, tela_le(jogo_tela(solit)) - 49) // Início dos números em ascii + 1
			);

			break;
		case 'w':
			jogo_descarte_para_ases(solit);

			break;
		case 'e':
			jogo_pilha_para_pilha(
				jogo_pilha(solit, tela_le(jogo_tela(solit)) - 49), // Lê a pilha de saída
				jogo_pilha(solit, tela_le(jogo_tela(solit)) - 49)  // Lê a pilha de entrada
			);

			break;
		case 'r':
			jogo_pilha_para_ases(solit,
				jogo_pilha(tela_le(jogo_tela(solit)) - 49)
			);

			break;
		case 't':
			jogo_pilha_para_descarte(solit,
				jogo_pilha(tela_le(jogo_tela(solit)) - 49)
			);
	}
}
