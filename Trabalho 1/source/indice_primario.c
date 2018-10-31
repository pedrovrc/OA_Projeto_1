#include "indice_primario.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define TAM_REG 70
#define TAM_CHAVE 30
#define TAM_MATR_NOME 48
#define TAM_DIR 80

int gera_indice_primario(FILE* arq_base, char* nome_gerado) {
    char linha_atual[TAM_REG];
    char string[TAM_CHAVE];
    char caminho_arquivo[TAM_DIR] = "arquivos_principais/";
    int conta_char;
    int endereco_registro = 0;

    if (arq_base == NULL) {
        return 1;
    }    

    strcat(caminho_arquivo, nome_gerado);
    FILE* indices = fopen(caminho_arquivo, "w");

    if (indices == NULL) {
        return 2;
    }

    while (fgets(linha_atual, TAM_REG, arq_base) != NULL) {
        for (int i = 0; i < TAM_CHAVE; i++) {
            string[i] = ' ';
        }
        conta_char = 0;
        for (int i = 0; i < TAM_MATR_NOME && conta_char < TAM_CHAVE; i++) {
            if (isspace(linha_atual[i])) {
                continue;
            }
            string[conta_char] = linha_atual[i];
            conta_char++;
        }
        fprintf(indices, "%s", string);
        fprintf(indices, " %d\n", endereco_registro);
        endereco_registro++;
    }

    rewind(arq_base);
    return 0;
}


int gera_indice_secundario(FILE* arq_base, FILE* ind_prim, char* nome_gerado){

    char linha_atual[TAM_REG];
    int conta_char;

    inicia_curso *cabeca = (inicia_curso*)malloc(sizeof(inicia_curso));
    cabeca->primeiro = NULL;
    cabeca->prox = NULL;
    
    while (fgets(linha_atual, TAM_REG, arq_base) != NULL) {
        for (int i = 0; i < TAM_CHAVE; i++) {
            string[i] = ' ';
        }
        conta_char = 0;
        for (int i = 0; i < TAM_MATR_NOME && conta_char < TAM_CHAVE; i++) {
            if (isspace(linha_atual[i])) {
                continue;
            }
            string[conta_char] = linha_atual[i];
            conta_char++;
        }
    }

    return;
}
