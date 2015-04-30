/*
 * texto.c
 * Funções para implementação de editor de texto.
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

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "texto.h"
#include "tela.h"
#include "memo.h"
#include "lista.h"

/**
 * Inicializa o texto, criando sua estrutura e seus valores padrões,
 * tais como linhas e suas respectivas listas.
 *
 * @param  void
 * @return text_t*
 */
texto_t* texto_inicia(void)
{
	texto_t*  txt = (texto_t*) memo_aloca(sizeof(texto_t));
	tamanho_t txt_tam = {600, 400};
	
	tela_inicializa(&txt->tela, txt_tam, "Trabalho T5");
	tela_limpa(&txt->tela);

	txt->nlin   = 0;
	txt->lincur = 0;
	txt->colcur = 0;
	txt->lin1   = 0;
	txt->col1   = 0;
	txt->linhas = lista_inicia();

	return txt;
}

/**
 * Finaliza e libera a memória do texto.
 *
 * @param  text_t*
 * @return void
 */
void texto_destroi(texto_t* txt)
{
	tela_limpa(&txt->tela);
	tela_finaliza(&txt->tela);
	lista_libera(&txt->linhas);
	memo_libera(txt);
}

/**
 * Valida e retorna a tela do texto.
 *
 * @param  texto_t*
 * @return tela_t*
 */
tela_t* texto_tela(texto_t* txt)
{
	assert( txt != NULL );
	return &txt->tela;
}

/**
 * Desenha o cursor na tela, respeitando as proporções desta.
 *
 * @param  text_t
 * @return void
 */
void texto_desenha_cursor_tela(texto_t* txt)
{
	cor_t cor = {1.0, 1.0, 1.0}; // branco.
	tamanho_t t; // tamanho do texto.
	ponto_t p1, p2; // pontos iniciais e finais da escrita
	int tam = lista_tamanho(txt->linhas[txt->lincur]); // tamanho da linha atual.
	char* substr[tam]; // string com o tamanho das letras até o cursor.

	memset(substr, '\0', tam);
	strncpy(substr, txt->content, txt->colcur * sizeof(char));
	t = tela_tamanho_texto(txt->tela, substr);

	// Para desenhar o cursor será feita uma linha de p1 até p2
	// p1 representa o ponto superior da linha.
	// p2 o ponto inferior.
	p1.y = txt->lincur * t.alt;
	p2.y = p1.y + t.alt;
	p1.x = p2.x = t.larg + 1;

	tela_cor(&txt->tela, cor);
	tela_linha(&txt->tela, p1, p2);
}

/**
 * Desenha todas as linhas e o cursor do editor.
 *
 * @param  text_t
 * @return void
 */
void texto_desenha_tela(texto_t* txt)
{
	cor_t cor = {1.0, 1.0, 1.0};
	tamanho_t t;
	ponto_t p;
	char* aux;
	int i, j;
	
	// Limpando a tela a cada reescrita.
	// e definindo que a cor a ser usada.
	tela_limpa(&txt->tela);
	tela_cor(&txt->tela, cor);

	// Pegando o número de linhas para desenhar.
	// Toda linha vai começar no p.X = 1.
	j = lista_tamanho(txt->linhas);
	p.x = 1;

	// Varrendo linha por linha e as desenhando.
	// p.y será o tamanho da linha multiplicado pelo número dela.
	for (i = 0; i < j; i++) {
		aux = lista_valor(txt->linhas, i);
		t = tela_tamanho_texto(aux);
		p.y = (i - 1) * t.alt + 1;
		tela_texto(&txt->tela, p, aux);
	}

	texto_desenha_cursor_tela(txt);
}

/**
 * Redesenha a tela a cada 30ms.
 *
 * @param  text_t
 * @return void
 */
void texto_atualiza_tela(texto_t* txt)
{
	texto_desenha_tela(txt);
	tela_mostra(texto_tela(txt));
	tela_espera(30);
}

/**
 * Apaga toda a tela e escreve um texto logo
 * no início dela.
 *
 * @param  text_t, char*
 * @return void
 */
void texto_escreve_tela(text_t* txt, char* c)
{
	ponto_t p;
	tamanho_t t;

	// criando um ponto que represente o tamanho
	// de uma linha.
	t = tela_tamanho_texto(c);
	p.x = 1;
	p.y = t.alt + 1;

	// Limpa a tela e manda uma mensagem requisitando
	// que um nome para o arquivo seja definido.
	tela_limpa(&txt->tela);
	tela_texto(&txt->tela, p, c);
}

/**
 * Salva o texto editado em um novo ou existente arquivo.
 * Caso o arquivo já exista o conteúdo será adicionado ao
 * seu final.
 *
 * @param  text_t
 * @return void
 */
void texto_comando_salvar(texto_t* txt)
{
	char* n,  o;
	int t, i, j;

	texto_escreve_tela(txt, "Qual o nome do arquivo?");	

	// Le o nome do arquivo e tenta abri-lo
	// caso não exista o arquivo a permissão
	// "a" garante que ele seja criado.
	scanf("%s", n);
	FILE *f = fopen(n, "ab+");

	// Gerando o output com a concatenação
	// do texto de todas as linhas.
	i = lista_tamanho(txt->linhas);
	for (j = 0; j < i; j++) {
		o = strcat(o, lista_valor(txt->linhas, j));
		o = strcat(o, "\n");
	}

	// Gravando e fechando o arquivo final.
	// note que se o arquivo já existir a
	// escrita será feita como uma concatenação
	// com o valor atual.
	fprintf(f, "%s", o);
	fclose(f);
}

/**
 * Abre um arquivo e gera todas suas linhas prontas para edição.
 *
 * @param  text_t
 * @return void
 */
void texto_comando_editar(texto_t* txt)
{
	char* n, c;

	texto_escreve_tela(txt, "Qual o nome do arquivo?");

	// Requisita o nome do arquivo e tenta abri-lo 
	// apenas para leitura.
	scanf("%s", n);
	FILE *f = fopen(n, "r");

	// caso não o encontre ou não tenha permissão
	// para leitura, apenas segue com a execução 
	// mostrando um erro no terminal.
	if (f == NULL) {
		printf("Não foi possível abrir o arquivo \"%s\".", n); return;
	}

	while (feof(f) != 0) {
		c = fgetc(c);

		if (c == '\n') {
			lista_adiciona(txt->linhas); continue;
		}

		texto_insere_char(txt, c);
	}

	// move o cursor para o início do texto.
	txt->colcur = 0;
	txt->lincur = 0;
}

/**
 * Cria uma nova lista representante de uma linha.
 *
 * @param  text_t
 * @return void
 */
void texto_nova_linha(texto_t* txt)
{
	lista_adiciona(txt->linhas);
	txt->lincur++;
	txt->colcur = 0;
}

/**
 * Apaga o último caracter, caso esteja no inicio da linha,
 * apaga a linha.
 *
 * @param  text_t
 * @return void
 */
void texto_remove_char(texto_t* txt)
{
	if (txt->colcur == 0) {
		if (txt->lincur > 0) {
			lista_remove(txt->linhas, txt->lincur);
			txt->lincur--;
		}
	} else {
		lista_t* l = lista_nesimo(txt->linhas, txt->lincur);
		int i = strlen(l->valor); // último caracter, \0
		l->valor[i - 1] = 0; // Penúltimo caracter vira o \0
		txt->colcur--;
	}
}

/**
 * Adiciona um caracter a linha atual.
 *
 * @param  texto_t, int
 * @return void
 */
void texto_insere_char(texto_t* txt, int c)
{
	char* caracter;

	sprintf(caracter, "%s", c);

	txt->linhas->valor = strcat(txt->linhas->valor, caracter);
}

/**
 * Acompanha a digitação do usuário atrás de combinações
 * pré-definidas e executa o comando correspondente a estas.
 *
 * @param  text_t
 * @return bool
 */
bool texto_processa_comandos(texto_t* txt)
{
	int tecla = tela_tecla(texto_tela(txt));
	int modificador = tela_tecla_modificador(texto_tela(txt));
	
	if (modificador & ALLEGRO_KEYMOD_CTRL) {
		switch (tecla) {
			// CTRL + Q: Fecha o editor de texto.
			case ALLEGRO_KEY_Q:
				return false;

			// CTRL + S: Salva em um arquivo todo o texto atual.
			case ALLEGRO_KEY_S:
				texto_comando_salvar(txt); break;

			// CTRL + E: Abre um arquivo no editor de texto.
			case ALLEGRO_KEY_E:
				texto_comando_editar(txt); break;
		}
	}

	switch (tecla) {
		// Move o cursor pra esquerda.
		case ALLEGRO_KEY_LEFT:
			texto_move_esq(txt); break;

		// Move o cursor pra direita.
		case ALLEGRO_KEY_RIGHT: 
			texto_move_dir(txt); break;

		// Move o cursor pra cima.
		case ALLEGRO_KEY_UP:
			texto_move_cima(txt); break;

		// Move o cursor pra baixo.
		case ALLEGRO_KEY_DOWN:
			texto_move_baixo(txt); break;

		// Remove o último caracter.
		case ALLEGRO_KEY_BACKSPACE: 
			texto_remove_char(txt); break;

		// Quebra a linha.
		case ALLEGRO_KEY_ENTER:
		case ALLEGRO_KEY_PAD_ENTER: 
			texto_nova_linha(txt); break;
	}

	texto_insere_char(txt, tecla);

	return true;
}

/**
 * Move o cursor para esquerda, apenas se não estivar na estrema esquerda.
 *
 * @param  text_t
 * @return void
 */
void texto_move_esq(texto_t* txt)
{
	if (txt->colcur > 0) {
		txt->colcur--;
	}
}

/**
 * Move o cursor para a direita, apenas se não estiver no final da linha.
 *
 * @param  text_t
 * @return void
 */
void texto_move_dir(texto_t* txt)
{
	if (txt->colcur < strlen(lista_valor(txt->linhas, txt->lincur))) {
		txt->colcur++;
	}
}

/**
 * Move o cursor para cima, apenas se não estiver na primeira linha
 *
 * @param  text_t
 * @return void
 */
void texto_move_cima(texto_t* txt)
{
	if (txt->lincur > 0) {
		txt->lincur--;
	}
}

/**
 * Move o cursor para baixo apenas se não estiver na última linha.
 *
 * @param  text_t
 * @return void
 */
void texto_move_baixo(texto_t* txt)
{
	if (txt->lincur < lista_tamanho(txt->linhas)) {
		txt->lincur++;
	}
}
