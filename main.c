#include <stdio.h>

void main (void) {
	int matricula;
	char nome[32];
	FILE *arq;

	printf("digite a matricula:");
	scanf("%d", &matricula);

	printf("digite o nome:");
	scanf("%s", nome);

	arq = fopen("saida.txt", "w");
	
	if (arq == NULL) {
		printf("Erro ao abrir arquivo.");
	} else {
		fprintf(arq, "2014%d %s\n", matricula, nome);
	}
	
	fclose(arq);
}
