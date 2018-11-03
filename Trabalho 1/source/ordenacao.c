#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ordenacao.h"

#define BUFFER 40
#define TAM_DIR 80          // Tamanho da string para nome do diretorio

int ordena_arq(char* nome_gerado) {
    char caminho_arquivo[TAM_DIR] = "arquivos_principais/";
    cell_string *inicio;
    cell_string *aux;

    strcat(caminho_arquivo, nome_gerado);
    FILE* arquivo = fopen(caminho_arquivo, "r");

    if (arquivo == NULL) {
        return 2;
    }

    inicio = le_strings(arquivo);
    fclose(arquivo);
    arquivo = fopen(caminho_arquivo, "w");
    ordena(inicio, arquivo);
    fclose(arquivo);
    
    //desalocar
    while (inicio != NULL) {
        aux = inicio->prox;
        free(inicio);
        inicio = aux;
    }

    return 0;
}

cell_string *le_strings(FILE* arquivo) {
    int flag = 0;
    char linha_atual[BUFFER];
    cell_string *inicio;
    cell_string *ultimo;
    cell_string *aux;

    while (fgets(linha_atual, BUFFER, arquivo) != NULL) {
        if (flag == 0) {
            inicio = malloc(sizeof(cell_string));
            aux = inicio;
            inicio->ant = NULL;
            ultimo = inicio;
        } else {
            aux->prox = malloc(sizeof(cell_string));
            aux = aux->prox;
        }
        strcpy(aux->string, linha_atual); // inicia string
        aux->prox = NULL;
        if (flag != 0) {
            aux->ant = ultimo;
        }
        ultimo = aux;
        flag = 1;
    }

    return inicio;
}

void ordena(cell_string *inicio, FILE* arquivo) {
    cell_string *aux = inicio;
    cell_string *percorre;
    cell_string *templario = aux->prox;
    char guarda[BUFFER], tras[BUFFER];

    while (templario != NULL) {

        strcpy(guarda, templario->string);
        percorre = aux;
        strcpy(tras, percorre->string);

        while (percorre != NULL && compara_string(guarda, tras) != 1) {
            strcpy(templario->string, percorre->string);
            percorre = percorre->ant;
            templario = templario->ant;
            if (percorre != NULL) 
                strcpy(tras, percorre->string);
        }

        strcpy(templario->string, guarda);
        aux = aux->prox;
        templario = aux->prox;
    }

    aux = inicio;
    while (aux != NULL) {
        fprintf(arquivo, "%s", aux->string);
        aux = aux->prox;
    }
    fprintf(arquivo, "\n");
    return;
}

// 0 -> arquivo iguais | 1 -> string1 > string2 alfabeticamente | -1 -> contrário de 1
int compara_string(char* string1, char* string2) {
    if (strcmp(string1, string2) == 0)
        return 0;

    for (int i = 0; i < BUFFER && string1[i] != '\n' && string2[i] != '\n'; i++) {
        if (string1[i] > string2[i])
            return 1;
        else if (string1[i] < string2[i])
            return -1;
    }
    return 0;
}
