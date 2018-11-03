#include <stdio.h>
#include "indice.h"
#include "ordenacao.h"
#include "pesquisa.h"

#define TAM_REG 65

int main() {

    int opcao = -1, conta_print = 0;
    char registro1[TAM_REG], registro2[TAM_REG];
    char indicacao[TAM_REG+4] = "   MATRIC NOME                                     OP  CURSO    TURMA";
	// Criacao dos arquivos de indice primario e secundario
    FILE* lista1 = fopen("arquivos_principais/lista1.txt", "r+");
    FILE* lista2 = fopen("arquivos_principais/lista2.txt", "r+");
    FILE* cabecalho = fopen("arquivos_principais/cabecalho.txt", "w");
    fclose(cabecalho);

    gera_ind_prim(lista1, "indicelista1.ind");
    gera_ind_prim(lista2, "indicelista2.ind");

    FILE* ind_prim1 = fopen("arquivos_principais/indicelista1.ind", "r+");
    FILE* ind_prim2 = fopen("arquivos_principais/indicelista2.ind", "r+");    

    gera_ind_sec(lista1, ind_prim1, "lista1.ind");
    gera_ind_sec(lista2, ind_prim2, "lista2.ind");

    fclose(ind_prim1);
    fclose(ind_prim2);

    ordena_arq("indicelista1.ind");
    ordena_arq("indicelista2.ind");

    ordena_arq("ind_sec_lista1.ind");
    ordena_arq("ind_sec_lista2.ind");

    fclose(lista1);
    fclose(lista2);
    
    do {
        for(int i = 0; i < TAM_REG; i++) {
            registro1[i] = ' ';
            registro2[i] = ' ';
        }
        registro1[TAM_REG] = '\0';
        registro2[TAM_REG] = '\0';

        printf("Deseja pesquisar de que forma?\n");
        printf("1. matricula\n2. curso\n3. nome\n\n");
        scanf("%d", &opcao);
        if (opcao != 1 && opcao != 2 && opcao != 3) {
            printf("Entrada invalida!\n\n");
        }
    } while (opcao != 1 && opcao != 2 && opcao != 3);
    
    if (pesquisa(opcao, registro1, registro2) != 0) {
        printf("Registro nao encontrado!\n");

    } else {
        printf("\nRegistro(s) encontrado(s):\n%s\n", indicacao);
        if (registro1[0] != '*') {
            conta_print++;
            printf("%d. %s", conta_print, registro1);
        }
        if (registro2[0] != '*') {
            conta_print++;
            printf("%d. %s", conta_print, registro2);
        }
    }

    return 0;
}
