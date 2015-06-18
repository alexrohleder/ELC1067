
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

void main(void)
{
	char *arquivo_nome, vertice_chave, vertice_nome, aresta_chave;
	int nvertices, narestas, i = 0;
	grafo_t *g = grafo_cria();

	printf("Digite o nome do arquivo com os dados: ");
	scanf("%s", arquivo_nome);
	FILE *arquivo = fopen(arquivo_nome, "r");
	printf("\n");

	if (arquivo == NULL) {
		printf("Não foi possível ler o arquivo indicado."); return;
	}

	fscanf(arquivo, "%d %d", &nvertices, &narestas);

	while (!feof(arquivo)) {
		if (i < nvertices) {
			fscanf(arquivo, "%s %s", &vertice_chave, &vertice_nome);

			grafo_insere_vertice(g, vertice_cria(vertice_chave, vertice_nome));
		} else {
			fscanf(arquivo, "%s %s", &vertice_chave, &aresta_chave);

			grafo_insere_aresta(g, vertice_chave, aresta_chave);
		}
	}

	grafo_imprime(g);
	//grafo_destroi(g);
	//memo_relatorio();
}
