
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "lista.h"
#include "vertice.h"
#include "memo.h"
#include "grafo.h"

void main(void)
{
	char arquivo_nome[50];
	int nvertices, narestas, i = 0;
	grafo_t *g = grafo_cria();

	FILE *arquivo = fopen("registros.txt", "r+");

	if (arquivo == NULL) {
		printf("Não foi possível ler %s.\n", arquivo_nome); return;
	}

	fscanf(arquivo, "%d %d", &nvertices, &narestas);
	printf("lendo %d vertices e %d arestas...\n", nvertices, narestas);
	printf("vertices: ");

	while (!feof(arquivo))
	{
		char *str0 = memo_aloca(sizeof(char) * 50);
		char *str1 = memo_aloca(sizeof(char) * 50);

		fscanf(arquivo, "%s %s", str0, str1);

		if (i < nvertices) {
			vertice_t *v = vertice_cria(str0, str1);
			printf("%s ", v->chave);
			grafo_insere_vertice(g, v);
		} else {
			grafo_insere_aresta(g, str0, str1);
		}

		i++;
	}

	printf("\n");
	char *v1 = memo_aloca(sizeof(char) * 5);
	char *v2 = memo_aloca(sizeof(char) * 5);
	
	printf("Selecione os vértices para gerar o menor caminho: ");
	scanf("%s %s", v1, v2);

	printf("O menor caminho partindo de %s até %s é:\n", v2, v1);
	grafo_busca_largura(g, grafo_busca_vertice(g, v1));
	grafo_caminho_curto(g, v1, v2);
	printf("\n");

	grafo_destroi(g);
	memo_libera(v1);
	memo_libera(v2);

	memo_relatorio();
}
