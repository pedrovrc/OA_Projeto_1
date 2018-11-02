#include <stdio.h>

#define TAM_CAMPO1 50

void atualiza_cabecalho(char *nome_arq, int qtd_linha) {
	char campo1[TAM_CAMPO1];

	for (int i = 0; i<TAM_CAMPO1-1; i++) {
		campo1[i] = ' ';
	}

	for (int i = 0; nome_arq[i] != '\n'; i++) {
		campo1[i] = nome_arq[i];
	}

	fprintf("%s %d", campo1, qtd_linha);
	return;
}