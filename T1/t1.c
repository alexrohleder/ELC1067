#include <stdio.h>
#include <string.h>

void main(int argc, char ** argv) {
	int i, j, k, matriculas[50];
	char c, nomes[50][255];
	float notas[50][2], media;

	if (argc < 2) {
		printf("Você deve definir qual nome será buscado."); return;
	}

	FILE * falunos = fopen("alunos.txt", "r");
	FILE * fnotas  = fopen("notas.txt", "r");

	if (falunos == NULL || fnotas == NULL) {
		printf("Erro ao abrir os arquivos necessários."); return;
	}

	i = j = 0;

	while (!feof(falunos) != 0) {
		if (fscanf(falunos, "%d", &matriculas[i]) <= 0) {
			break;
		}

		c = fgetc(falunos);

		while (c != '\n') {
			nomes[i][j] = c;
			c = fgetc(falunos);
			j++;
		}

		nomes[i][j] = '\0';
		j = 0;
		i++;
	}

	i = j = 0;

	while (!feof(fnotas) != 0) {
		if (fscanf(fnotas, "%d %f %f", &matriculas[i], &notas[i][0], &notas[i][1]) <= 0) {
			break;
		}

		i++;
	}

	for (j = 0; j < i; j++) {
		if (strstr(nomes[j], argv[1]) != NULL) {
			printf("%.2f %s\n", (notas[j][0] + notas[j][1]) / 2, nomes[j]);
		}
	}
}

