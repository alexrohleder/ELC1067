#include <stdio.h>
#include <string.h>

void main(int argc, char ** argv) {
	int i, j, matriculas[50];
	char c, nome[50][255];
	float notas[2], media;

	if (argc < 2) {
		printf("Você deve definir qual nome será buscado."); return;
	}

	FILE * alunos = fopen("alunos.txt", "r");
	FILE * notas  = fopen("notas.txt", "r");

	if (alunos == NULL || notas == NULL) {
		printf("Erro ao abrir os arquivos necessários."); return;
	}

	i = j = 0;

	while (!feof(alunos) != 0) {
		if (fscanf(alunos, "%d", &matriculas[i])) {
			break;
		}

		c = fgetc(alunos);

		while (c != "\n") {
			nome[i][j] = c;
			c = fgetc(alunos);
			j++;
		}

		
	}
}

