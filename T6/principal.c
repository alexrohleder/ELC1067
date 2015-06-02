/*
 * principal.c
 * Implementação de árvore de expressões aritméticas.
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
#include <stdio.h>
#include <string.h>

#include "arv.h"
#include "pilha.h"
#include "memo.h"

int main(int argc, char **argv)
{
	int max_expression_size = 120, max_node_number = 10, current_node = 0, i;
	char *tokens = malloc(max_expression_size);
	arv_t *arv[max_node_number];
	op_t op;
	pilha_t *pilha = pilha_cria();

	printf("Digite a expressão polonesa com seus operandos/operadores separados por espaços:\n");
    fgets(tokens, max_expression_size, stdin);

    int tokenslen = strlen(tokens);
    if (tokenslen > 0 && tokens[tokenslen - 1] == '\n') {
        tokens[tokenslen - 1] = '\0';
    }

	while ((tokens = strtok(tokens, " ")) != NULL)
	{

		switch (tokens[0])
		{
			case '+': case '-': case '/': case '*':
				op.tipo = OPERADOR;
				op.u.operador = tokens[0];

				arv[current_node] = arv_cria(op);

				arv_insere_direita(arv[current_node], pilha_remove(pilha));
				arv_insere_esquerda(arv[current_node], pilha_remove(pilha));

				pilha_insere(pilha, arv[current_node]);

				current_node++;
			break;

			default:
				op.tipo = OPERANDO;
				op.u.operando = atof(tokens);

				pilha_insere(pilha, arv_cria(op));
			break;
		}

		tokens = NULL;
	}

	printf("Imprimindo a arvore gerada em:");
	
	printf("\nPré ordem:");
	for (i = 0; i < current_node; i++) {
		arv_imprime_pre_ordem(arv[i]);
		printf(" ");
	}

	printf("\nEm ordem:");
	for (i = 0; i < current_node; i++) {
		arv_imprime_em_ordem(arv[i]);
		printf(" ");
	}

	printf("\nPós ordem:");
	for (i = 0; i < current_node; i++) {
		arv_imprime_pos_ordem(arv[i]);
		printf(" ");
	}

	printf("\n");

	for (i = 0; i < current_node; i++) {
		arv_destroi(arv[i]);
	}

	pilha_destroi(pilha);
	memo_relatorio();

	return 0;
}
