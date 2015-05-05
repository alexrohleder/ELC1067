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
	
	tela_inicializa(txt->tela, txt_tam, "Trabalho T5");
	tela_limpa(txt->tela);

	txt->lin1   = 0;
	txt->col1   = 0;
	txt->nlin   = 0;
	txt->lincur = 0;
	txt->colcur = 0;
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
	lista_libera(txt->linhas);
	tela_limpa(txt->tela);
	tela_finaliza(txt->tela);
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
	return txt->tela;
}

/**
 * Desenha o cursor na tela, respeitando as proporções desta.
 *
 * @param  text_t
 * @return void
 */
void texto_desenha_cursor_tela(texto_t* txt)
{
	// cor do cursor, padrão: branco(1, 1, 1).
	cor_t cor = {1.0, 1.0, 1.0}; 
	// tamanho do texto.
	tamanho_t t; 
	// pontos iniciais e finais da escrita
	ponto_t p1, p2;
	// lista atual
	lista_t* l = lista_nesimo(txt->linhas, txt->lincur);
	// tamanho da linha atual.
	int tam = lista_tamanho(l); 
	// string com o tamanho das letras até o cursor.
	char* substr; 

	// Setando dinâmicamente o \0 no final da sar *dest, const tring
	// Copiando toda a string anterior ao cursor
	memset(substr, '\0', tam);
	strncpy(substr, l->valor, txt->colcur * sizeof(char));
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
 * Pega uma substring de uma linha, a substring corresponde ao
 * texto da linha a partir da primeira coluna.
 *
 * @param  string
 * @param  integer
 * @return string
 */
char* texto_corrige(char* str, int pos)
{
	int len = strlen(str);
	char* buffer = "";

	if (pos < len) {
		strncpy(buffer, str + pos, len - pos);
	}

	return buffer;
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
	tela_limpa(txt->tela);
	tela_cor(txt->tela, cor);

	// Pegando o número de linhas para desenhar.
	// Toda linha vai começar no p.X = 1.
	j = lista_tamanho(txt->linhas);
	p.x = 1;

	// Varrendo linha por linha e as desenhando.
	// p.y será o tamanho da linha multiplicado pelo número dela.
	for (i = 0; i < j; i++) {
		aux = lista_valor(txt->linhas, i);

		// Caso o ponteiro já tenha ultrapassado o limite da tela
		// move o texto de forma correspondente ao ponteiro.
		aux = texto_corrige(aux, txt->col1);

		t   = tela_tamanho_texto(aux);
		p.y = (i - 1) * t.alt + 1;
		tela_texto(txt->tela, p, aux);
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
	tela_mostra(txt->tela);
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
	tela_limpa(txt->tela);
	tela_texto(txt->tela, p, c);
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
	char*  n, o, a;
	int i, j, k;

	texto_escreve_tela(txt, "Qual o nome do arquivo?");	

	// Le o nome do arquivo e tenta abri-lo
	// caso não exista o arquivo a permissão
	// "a" garante que ele seja criado.
	scanf("%s", n);
	FILE *f = fopen(n, "w");

	i = lista_tamanho(txt->linhas);
	j = 0;

	// Fazendo a alocação inicial do output.
	// A cada nova linha este sera realocado.
	o = (char*) memo_aloca(1);

	// Gerando o output com a concatenação
	// do texto de todas as linhas.
	do {
		a = lista_valor(txt->linhas, j);

		// Realocando o tamanho do output para comportar a nova linha
		// mais uma posição para o \n...
		o = (char*) memo_realoca(o, sizeof(o) + sizeof(a) + 1);

		// Utilizando o strcat para concatenar o output atual
		// com o valor da linha.
		o = strcat(o, a);
		o = strcat(o, "\n");
	} while (j < i);

	// Removendo o último \n e encerrando a string.
	o[strlen(o) - 1] = '\0';

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
	if (f != NULL) {
		printf("Não foi possível abrir o arquivo \"%s\".", n);
	} else {
		while (feof(f) != 0) {
			c = fgetc(c);

			if (c == '\n') {
				lista_adiciona(txt->linhas);
			} else texto_insere_char(txt, c);
		}

		// move o cursor para o início do texto.
		txt->colcur = 0;
		txt->lincur = 0;
	}
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
			int i, n = sizeof(txt->linhas->valor);

			// A inserção deve ser feita na linha anterior,
			// para seguir a interface de texto.h decrementei
			// antes do loop o número de linhas
			txt->lincur--;

			// Movendo todo o texto da linha removida
			// para a linha anterior.
			for (i = 0; i < n; i++) {
				// Manipulando a linha excluida atravéz do ponteiro
				// próximo, para seguir a interface de texto.h
				texto_insere_char(txt, txt->linhas->proxima->valor[i]);
			}

			// Remove a lista que sobrou
			lista_remove(txt->linhas, txt->lincur + 1);
		}
	} else {
		lista_t* l = lista_nesimo(txt->linhas, txt->lincur);

		txt->colcur--;

		// Removendo o caracter desejado
		// @see http://stackoverflow.com/questions/5457608/how-to-remove-a-character-from-a-string-in-c
		memmove(&l->valor, &l->valor[txt->colcur + 1], strlen(l->valor) - txt->colcur);

		// liberar a ultima posição.
	}
}

/**
 * Adiciona um caracter a linha atual.
 *
 * @param  texto_t, int
 * @return void
 */
void texto_insere_char(texto_t txt, int c)
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
		if (txt->colcur == txt->col1) {
			txt->col1--;
		}
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
		if (txt->lincur == txt->lin1) {
			txt->lin1--;
		}
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
