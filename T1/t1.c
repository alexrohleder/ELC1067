#include <stdio.h>

void main (int argc, char ** argv) {

	int matricula;
	char nome[64];
	float n1 , n2;

	FILE * alunos, notas;

	if (argc > 1) {
		printf("Você deve definir um nome de aluno como parâmetro."); return;
	}

	alunos = fopen("alunos.txt", "r");
	notas = fopen("notas.txt", "r");

	if (alunos == NULL || notas == NULL) {
		printf("Erro ao abrir um dos arquivos."); return;
	}

	

	fclose(alunos);
	fclose(notas);
}
