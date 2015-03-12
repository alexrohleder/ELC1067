#include <stdio.h>

// c = fgetc(file)
// if (c == "\n")
// feof(file)

void ler_alunos(int * matriculas, char ** nome, int * n) {
	int matricula, i = 0, j = 0;
	char caracter, nome[50];

	FILE * resource = fopen("alunos.txt", "r");

	if (resource == NULL) {
		printf("Erro ao abrir um dos arquivos."); return;
	}

	while (feof(resource) != 0) {
		if (fscanf(resource, "%d", &matricula)) {
			break;
		}

		caracter = fgetc(resource);

		while (caracter != "\n") {
			nome[i] = caracter;
			caracter = fgetc(resource);
			/**/ i++;
		}

		nome[i] = "\n";
		matriculas[j] = matricula;
		strcpy(nomes[j], nome);
		/**/ j++;
	}

	fclose(resource);
	n = linha;
}

float media(int * matricula) {
	int m;
	float n1, n2;

	FILE * resource = fopen("notas.txt", "r");

	if (resource == NULL) {
		printf("Erro ao abrir um dos arquivos."); return;
	}

	while (feof(resource) != 0) {
		if (fscanf(resource, "%d", &m)) {
			break;
		}

		if (m == matricula) {
			if (fscanf(resource, "%f", &n1) && fcanf(resource, "%f", &n2)) {
				return (n1 + n2) / 2;
			}
		}
	}

	return 0.0;
}

void main (int argc, char ** argv) {

	int matriculas[50], matricula, linhas, linha, i;
	char nomes[50][64], nome[64], caracter;

	if (argc > 1) {
		printf("Você deve definir um nome de aluno como parâmetro."); return;
	}

	ler_alunos(&matriculas, &nomes, &linhas);

	for (i = 0; i <= linhas; i++) {
		if (strcmp(nomes[i], nome) >= 0) {
			linha = i; break;
		}
	}

	matricula = matriculas[linha];
	nome = nomes[linha];

	printf("Aluno %s(%d) tem média: %f", nome, matricula, media(matricula));

}
