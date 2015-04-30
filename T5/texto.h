/*
 * texto.h
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
#ifndef _TEXTO_H_
#define _TEXTO_H_

#include "tela.h"
#include "lista.h"

typedef struct {
	char *nome;      // nome do arquivo sendo editado.
	int lincur;      // linha onde esta o cursor (comeca em 0).
	int colcur;      // coluna onde esta o cursoe na linha acima.
	lista_t* linhas; // lista de linhas do arquivo.
	tela_t tela;     // tela do editor.
} texto_t;


texto_t* texto_inicia(void);
tela_t*  texto_tela(texto_t* txt);
void texto_destroi(texto_t* txt);
void texto_desenha_tela(texto_t *txt);
void texto_atualiza_tela(texto_t *txt);
bool texto_processa_comandos(texto_t* txt);
void texto_le_arquivo(texto_t *txt, char *nome);
void texto_move_esq(texto_t *txt);
void texto_move_dir(texto_t *txt);
void texto_move_baixo(texto_t *txt);
void texto_move_cima(texto_t *txt);
void texto_insere_char(texto_t *txt, char c);
void texto_remove_char(texto_t *txt);

#endif /* _TEXTO_H_ */
