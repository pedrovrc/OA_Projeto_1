#include <stdio.h>
#include <string.h>
#include "manutencao.h"

#define TAM_CAMPO1 50

void atualiza_cabecalho(char *nome_arq, int qtd_linha) {

    char campo1[TAM_CAMPO1];
    char campo_aux[TAM_CAMPO1];
    int tamanho_nome = strlen(nome_arq);
    FILE* cabecalho = fopen("arquivos_principais/cabecalho.txt", "r+");
    int flag = 0;

    for (int i = 0; i < TAM_CAMPO1-1; i++) {
        campo_aux[i] = ' ';
    }
    campo1[TAM_CAMPO1-1] = '\0';

    for (int i = 0; i < tamanho_nome; i++) {
        campo_aux[i] = nome_arq[i];
    }

    while (fgets(campo1, TAM_CAMPO1, cabecalho) != NULL) {
        if (strcmp(campo1, campo_aux) == 0) {
            flag = 1;
            break;
        }
        //ler restante do registro
        fgets(campo1, TAM_CAMPO1, cabecalho);
    }

    if (flag == 0) {
        for (int i = 0; i < TAM_CAMPO1-1; i++) {
            campo1[i] = ' ';
        }
        campo1[TAM_CAMPO1-1] = '\0';

        for (int i = 0; i < tamanho_nome; i++) {
            campo1[i] = nome_arq[i];
        }
        fprintf(cabecalho, "%s", campo1);
    }

    fprintf(cabecalho, "%d\n", qtd_linha);
    fclose(cabecalho);

    return;
}
