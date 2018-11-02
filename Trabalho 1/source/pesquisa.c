#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int pesquisa(int opcao, char *string) {

	int falha;

	switch (opcao) {
		case 1:
			falha = pesquisa_matricula(string);
			break;
		case 2:
			falha = pesquisa_curso(string);
			break;
		case 3:
			falha = pesquisa_nome(string);
			break;
	}
	return falha;
}

int pesquisa_matricula(char *string) {

	/*
	FILE *arq_base = fopen("arquivos_principais/lista1.txt");
	FILE *arq_ind = fopen("arquivos_principais/indicelista1.ind");
	int matricula;

	printf("Entre com a matricula: (6 digitos)\n");
	scanf("%d", matricula);
	*/

	return 0; 
}
