
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

	while (!feof(arquivo))
	{
		char *str0 = memo_aloca(sizeof(char) * 50);
		char *str1 = memo_aloca(sizeof(char) * 50);

		printf("lendo registros... ");
		fscanf(arquivo, "%s %s", str0, str1);

		if (i < nvertices)
		{
			vertice_t *v = vertice_cria(str0, str1);
			printf("vertice %s lido... ", v->nome);
			grafo_insere_vertice(g, v);
		}
		else
		{
			printf("aresta entre %s e %s lida... ", str0, str1);
			grafo_insere_aresta(g, str0, str1);
		}

		printf("Ok\n");
		i++;
	}

	printf("imprimindo o grafo gerado:\n");
	grafo_imprime(g);

	
}
