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
#include "carta.h"

#define SOLIT_MAGICO 0x50717
#define DESTRUIDO 0x80000000

#define ERR_PILHA_VAZIA "Você não pode remover uma carta de uma pilha vazia."
#define ERR_IMEDIATO "Você deve colocar uma carta imediatamente inferior e de naipe diferente nesta pilha!"
#define ERR_INSERIR_EM_ASES "Ainda não é possível inserir esta carta a pilha de ases."
#define ERR_JOGADA "Esta jogada não existe."

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
	jogo sol;
	int i;

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
	int i;
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

void
jogo_log(char* message)
{
	printw("\n");
	printw(message);
}

/**
 * Verifica se a carta c1 é imediatamente inferior e caso $mesmo_naipe
 * seja definido como 1, se o naipe de c1 e c2 são iguais, o contrário
 * desta verificação de naipe acontece ao setar $mesmo_naipe a 0.
 *
 * @return boolean
 */

bool
jogo_verifica_imediato(carta c1, carta c2, int mesmo_naipe) {
	naipe c1_naipe = carta_naipe(c1);
	naipe c2_naipe = carta_naipe(c2);

	if (carta_valor(c1) == carta_valor(c2) - 1) {
		if (mesmo_naipe == 0 && c1_naipe != c2_naipe) {
			if ((c1_naipe <= 1 && c2_naipe >= 2) || (c1_naipe >= 2 && c2_naipe <= 1)) {
				return true;
			}
		} else {
			if (mesmo_naipe == 1 && c1_naipe == c2_naipe) {
				return true;
			}
		}
	}

	jogo_log(ERR_IMEDIATO);
	return false;
}

/**
 * Caso a pilha não esteja vazia abre sua última carta.
 *
 * @return void
 */

void
jogo_corrige_pilha(pilha p) {
	if (!pilha_vazia(p)) {
		carta c = pilha_remove_carta(p);
		carta_abre(c);
		pilha_insere_carta(p, c);
	}
}

/**
 * Pede ao usuário que insira um número de 1 a 7 e retorna a lista
 * correspondente a este index.
 *
 * @return pilha
 */

pilha
jogo_seleciona_lista(jogo solit) {
	int p = tela_le(jogo_tela(solit));
	
	if (p < 48 || p > 58) {
		jogo_log(ERR_JOGADA); 
		return NULL;
	}

	return jogo_pilha(solit, p - 49);
}

/**
 * Move uma carta qualquer aos ases, tanto novos ases quanto
 * cartas normais, validando a ordem a pilha no último caso.
 *
 * @return boolean
 */

bool
jogo_carta_para_ases(jogo solit, carta c) {
	pilha p;
	int   i = 0;

	if (carta_valor(c) == 1) {
		do {
			p = jogo_ases(solit, i); i++;
		} while (!pilha_vazia(p));

		pilha_insere_carta(p, c);
		return true;
	} else {
		carta cp;
		
		for (; i < 4; i++) {
			p  = jogo_ases(solit, i);

			if (!pilha_vazia(p)) {
				cp = pilha_remove_carta(p);
					 pilha_insere_carta(p, cp);

				if (jogo_verifica_imediato(cp, c, 1) == true) {
					pilha_insere_carta(p, cp);
					pilha_insere_carta(p, c);
					return true;
				}
			}
		}

		jogo_log(ERR_INSERIR_EM_ASES);
		return false;
	}
}

/**
 * Inicializa o jogo criando o baralho, embaralhando e criando as 7 primeiras
 * pilhas, assim como o descarte.
 *
 * @return void
 */

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
		} while (c == NULL);

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

/**
 * Move a última carta do monte para o descarte
 *
 * @return void
 */

void
jogo_monte_para_descarte(jogo solit) {
	pilha monte = jogo_monte(solit);

	if (pilha_vazia(monte)) {
		/**/ jogo_log(ERR_PILHA_VAZIA);
	} else {
		carta c = pilha_remove_carta(monte);
		carta_abre(c);
	  	pilha_insere_carta(jogo_descartes(solit), c);
	  	jogo_log("Carta movida do monte para o descarte.");
	}
}

/**
 * Move todas as cartas do descarte ao monte
 *
 * @return void
 */

void
jogo_descarte_para_monte(jogo solit) {
	carta c;

	while (!pilha_vazia(jogo_descartes(solit))) {
		c = pilha_remove_carta(jogo_descartes(solit));
		carta_fecha(c);
		pilha_insere_carta(jogo_monte(solit), c);
	}

	jogo_log("Descarte virado no monte");
}

/**
 * Move a última carta do descarte a uma pilha
 *
 * @return void
 */

void
jogo_descarte_para_pilha(jogo solit, pilha p) {
	pilha descarte = jogo_descartes(solit);

	if (pilha_vazia(descarte)) {
		/**/ jogo_log(ERR_PILHA_VAZIA);
	} else {
		carta c = pilha_remove_carta(descarte);

		if (carta_valor(c) == 13 && pilha_vazia(p)) {
			pilha_insere_carta(p, c);
		} else {
			carta t = pilha_remove_carta(p);
				      pilha_insere_carta(p, t);

			if (jogo_verifica_imediato(c, t, 0) == true) {
				    pilha_insere_carta(p, c);
			} else  pilha_insere_carta(descarte, c);
		}

		jogo_log("Carta movida do descarte para a pilha.");
	}
}

/**
 * Move uma carta do descarte para sua respectiva pilha de ases,
 * caso não consiga o descarte permanece no mesmo estado.
 *
 * @return void
 */

void
jogo_descarte_para_ases(jogo solit) {
	pilha descarte = jogo_descartes(solit);

	if (pilha_vazia(descarte)) {
		/**/ jogo_log(ERR_PILHA_VAZIA);
	} else {
		carta c = pilha_remove_carta(descarte);
		int conseguiu_inserir_nos_ases = jogo_carta_para_ases(solit, c);
		if (conseguiu_inserir_nos_ases == false) {
			pilha_insere_carta(descarte, c);
		}

		jogo_log("Carta movida do descarte para os ases.");
	}
}

/**
 * Move a primeira carta de $p1 para $p2, caso seja possível, caso
 * não as pilhas permanecem no mesmo estado.
 *
 * @return void
 */

void
jogo_pilha_para_pilha(pilha p1, pilha p2) {
	if (pilha_vazia(p1)) {
		jogo_log(ERR_PILHA_VAZIA);
	} else {
		carta c1 = pilha_remove_carta(p1);

		if (carta_valor(c1) == 13 && pilha_vazia(p2)) {
			pilha_insere_carta(p2, c1);
		} else {
			carta c2 = pilha_remove_carta(p2);
					   pilha_insere_carta(p2, c2);

		    pilha p3 = pilha_cria();
		    carta cp = c1;

		    pilha_insere_carta(p3, cp);

		    while (!pilha_vazia(p1) && carta_aberta(cp)) {
		    	cp = pilha_remove_carta(p1);
		    	pilha_insere_carta(p3,  cp);
		    }

		    if (!carta_aberta(cp)) {
		    	pilha_insere_carta(p1, pilha_remove_carta(p3));
		    	cp = pilha_remove_carta(p3);
		    		 pilha_insere_carta(p3, cp);
		    }

		    bool para_segunda_pilha = jogo_verifica_imediato(cp, c2, 0);

		    while (!pilha_vazia(p3)) {
		    	pilha_insere_carta(para_segunda_pilha == true ? p2 : p1, pilha_remove_carta(p3));
		    }

		    pilha_destroi(p3);
		    jogo_corrige_pilha(p1);
		}

		jogo_log("Carta movida de uma pilha a outra.");
	}
}

/**
 * Manda a última carta de uma pilha a sua respectiva pilha de ases,
 * caso não consiga a pilha permanece no mesmo estado.
 *
 * @return void
 */

void
jogo_pilha_para_ases(jogo solit, pilha p) {
	if (pilha_vazia(p)) {
		/**/ jogo_log(ERR_PILHA_VAZIA);
	} else {
		carta c = pilha_remove_carta(p);
		int   conseguiu_inserir_nos_ases = jogo_carta_para_ases(solit, c);

		if (conseguiu_inserir_nos_ases == true) {
			   jogo_corrige_pilha(p);
		} else pilha_insere_carta(p, c);

		jogo_log("Carta movida da pilha para os ases.");
	}
}

/**
 * Insere a última carta de uma pilha no descarte.
 *
 * @return void
 */

void
jogo_pilha_para_descarte(jogo solit, pilha p) {
	if (pilha_vazia(p)) {
		/**/ jogo_log(ERR_PILHA_VAZIA);
	} else {
		carta c = pilha_remove_carta(p);
	  			  pilha_insere_carta(jogo_descartes(solit), c);
	  	jogo_corrige_pilha(p);
	  	jogo_log("Carta movida de uma pilha para o descarte.");
	}
}

/**
 * Lê um comando do usuário e procura por sua função correspondente.
 *
 * @return void
 */

int
jogo_comando(jogo solit) {
	pilha l1 = NULL;
	pilha l2 = NULL;
	int   c  = tela_le(jogo_tela(solit));

	switch (c) {
		// Move do monte para o descarte
		case ' '://c
			jogo_monte_para_descarte(solit);

			break;
		case 'b'://c
		   	jogo_descarte_para_monte(solit);

			break;
		// Move do descarte para uma pilha
		case 'q'://c
			l1 = jogo_seleciona_lista(solit);
			
			if (l1 != NULL) {
				jogo_descarte_para_pilha(solit, l1);
			}

			break;
		// Move do descarte para os ases
		case 'w'://c
			jogo_descarte_para_ases(solit);

			break;
		// Move de uma pilha a outra
		case 'e':
			l1 = jogo_seleciona_lista(solit);
			l2 = jogo_seleciona_lista(solit);

			if (l1 != NULL && l2 != NULL) {
				jogo_pilha_para_pilha(l1, l2);
			}

			break;
		// Move de uma pilha aos ases
		case 'r'://c
			l1 = jogo_seleciona_lista(solit);

			if (l1 != NULL) {
				jogo_pilha_para_ases(solit, l1);
			}

			break;
		// Move de uma pilha ao descarte
		case 't'://c
			l1 = jogo_seleciona_lista(solit);

			if (l1 != NULL) {
				jogo_pilha_para_descarte(solit, l1);
			}

			break;
		default:
			jogo_log(ERR_JOGADA);
	}

	return c;
}
