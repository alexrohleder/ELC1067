#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int MAX_REG_SIZE = 50;
const int STRING_SIZE = 256;

void ler_alunos(int* matriculas, char** nomes) {
	int i, j;
	char c;

	FILE *resource = fopen("alunos.txt", "r");

	if (resource == NULL) {
		printf("Erro ao abrir os arquivos necessários.\n"); return;
	}

	i = j = 0;

	while (!feof(resource)) {
		if (fscanf(resource, "%d", &matriculas[i]) <= 0) {
			break;
		}

		nomes[i] = (char *) malloc(STRING_SIZE * sizeof(char));
		c = fgetc(resource);

		while (c != '\n') {
			nomes[i][j] = c;
			c = fgetc(resource);
			j++;
		}

		nomes[i][j] = '\0';
		j = 0;
		i++;
	}

	fclose(resource);
}

void ler_notas(int *matriculas, float *n1, float *n2) {
	int i, matricula;

	FILE *resource = fopen("notas.txt", "r");

	if (resource == NULL) {
		printf("Erro ao abrir os arquivos necessários.\n"); return;
	}

	while (!feof(resource) != 0) {
		if (fscanf(resource, "%d", &matricula) <= 0) {
			break;
		}

		for (i = 0; i < MAX_REG_SIZE; i++) {
			if (matriculas[i] == matricula) {
				break;
			}
		}

		fscanf(resource, "%f %f", &n1[i], &n2[i]);
		i = 0;
	}

	fclose(resource);
}

void main(int argc, char ** argv) {
	int i, j, k, *matriculas;
	char c, **nomes;
	float *n1, *n2;

	if (argc < 2) {
		printf("Você deve definir qual nome será buscado.\n"); return;
	}

	matriculas = (int*) malloc(MAX_REG_SIZE * sizeof(int));
	nomes = (char**) malloc(MAX_REG_SIZE * sizeof(char*));
	n1 = (float*) malloc(MAX_REG_SIZE * sizeof(float));
	n2 = (float*) malloc(MAX_REG_SIZE * sizeof(float));

	ler_alunos(matriculas, nomes);
	ler_notas(matriculas, n1, n2);

	for (i = 0; i < MAX_REG_SIZE; i++) {
		if (strstr(nomes[i], argv[1]) != NULL) {
			printf("%.2f %s\n", (n1[i] + n2[i]) / 2, nomes[i]);
		}
	}

	free(matriculas);
	for (i = 0; i < MAX_REG_SIZE; i++) free(nomes[i]);
	free(nomes);
	free(n1);
	free(n2);
}

