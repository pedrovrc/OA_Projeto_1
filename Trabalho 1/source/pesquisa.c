#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "pesquisa.h"

#define BUFFER 100
#define POS_QTD_LINHA 49
#define TAM_REG 65
#define TAM_CHAVE 30        // Tamanho da chave primaria do arquivo de indices

int pesquisa(int opcao, char *string) {

    int falha;

    switch (opcao) {
        case 1:
            falha = pesquisa_matricula(string);
            break;
        case 2:
            //falha = pesquisa_curso(string);
            break;
        case 3:
            //falha = pesquisa_nome(string);
            break;
    }
    return falha;
}

int pesquisa_matricula(char *string) {
    
    FILE *arq_base1 = fopen("arquivos_principais/lista1.txt", "r");
    FILE *arq_base2 = fopen("arquivos_principais/lista2.txt", "r");
    FILE *arq_ind_prim1 = fopen("arquivos_principais/indicelista1.ind", "r");
    FILE *arq_ind_prim2 = fopen("arquivos_principais/indicelista2.ind", "r");
    FILE *arq_cabec = fopen("arquivos_principais/cabecalho.txt", "r");
    int matricula, tam_ind1, tam_ind2, chave_lida, pos_array = -1;
    int NRR = 0, indica_array;
    char linha[BUFFER];

    printf("Entre com a matricula: (6 digitos)\n");
    scanf("%d", &matricula);

    // Pega dados de arquivos de indices primarios
    if (fgets(linha, POS_QTD_LINHA, arq_cabec) == NULL) {
        printf("Nao ha base de dados carregada.\n");
        return 1;
    }

    // Le numero de linhas do arq1
    fscanf(arq_cabec, "%d", &tam_ind1);

    // Posiciona na prox linha
    fgets(linha, BUFFER, arq_cabec);

    // Pega dados de arquivos de indices primarios
    if (fgets(linha, POS_QTD_LINHA, arq_cabec) == NULL) {
        printf("Nao ha base de dados carregada.\n");
        return 1;
    }

    // Le numero de linhas do arq2
    fscanf(arq_cabec, "%d", &tam_ind2);

    int chaves_primarias1[tam_ind1];
    int chaves_primarias2[tam_ind2];

    //Le e preenche estruturas com chaves primarias
    // ind_prim_1
    //printf("chaves_primarias1:\n");
    for (int i = 0; i < tam_ind1; i++) {
        // Le valor da chave
        fscanf(arq_ind_prim1, "%d", &chave_lida);
        // Salva na estrutura
        chaves_primarias1[i] = chave_lida;
        //printf("%d ", chaves_primarias1[i]);
        // Pula pra prox linha
        fgets(linha, BUFFER, arq_ind_prim1);
    }
    //printf("\n\n");

    // ind_prim_2
    //printf("chaves_primarias2:\n");
    for (int i = 0; i < tam_ind2; i++) {
        // Le valor da chave
        fscanf(arq_ind_prim2, "%d", &chave_lida);
        // Salva na estrutura
        chaves_primarias2[i] = chave_lida;
        //printf("%d ", chaves_primarias2[i]);
        // Pula pra prox linha
        fgets(linha, BUFFER, arq_ind_prim2);
    }
    //printf("\n\n");

    // Busca binaria nos arrays
    pos_array = busca_binaria_int(chaves_primarias1, 0, tam_ind1, matricula);

    if (pos_array < 0) {    // Se nao estiver no primeiro, procura no segundo
        pos_array = busca_binaria_int(chaves_primarias2, 0, tam_ind2, matricula);
    } else {
        indica_array = 1;
    }

    if (pos_array < 0) {    // Se nao estiver em nenhum dos dois, nao existe
        return 1;
    } else {
        indica_array = 2;
    }

    printf("\npos_array = %d\n", pos_array);
    rewind(arq_ind_prim1);
    rewind(arq_ind_prim2);

    if (indica_array == 1) {    // Usa arq_ind_prim1
        // Le arq_ind ate achar registro (usar pos_array pra isso)
        for (int i = 0; i < pos_array; i++) {
            fgets(linha, BUFFER, arq_ind_prim1);
            printf("linha = |%s|\n", linha);
        }

        // Pegar NRR
        fgets(linha, TAM_CHAVE, arq_ind_prim1);
        printf("linha = |%s|\n", linha);
        fscanf(arq_ind_prim1, "%d", &NRR);
        printf("NRR = %d\n", NRR);

        // Ler arq_base (usar NRR) e salvar registro registro
        for (int i = 0; i < NRR; i++) {
            fgets(linha, BUFFER, arq_base1);
        }

        fgets(string, TAM_REG, arq_base1);
    } else {    // Usa arq_ind_prim2
        // Le arq_ind ate achar registro (usar pos_array pra isso)
        for (int i = 0; i < pos_array; i++) {
            fgets(linha, BUFFER, arq_ind_prim2);
            printf("linha = |%s|\n", linha);
        }

        // Pegar NRR
        fgets(linha, TAM_CHAVE, arq_ind_prim2);
        printf("linha = |%s|\n", linha);
        fscanf(arq_ind_prim2, "%d", &NRR);
        printf("NRR = %d\n", NRR);

        // Ler arq_base (usar NRR) e salvar registro registro
        for (int i = 0; i < NRR; i++) {
            fgets(linha, BUFFER, arq_base2);
        }

        fgets(string, TAM_REG, arq_base2);
    }

    fclose(arq_base1);
    fclose(arq_base2);
    fclose(arq_ind_prim1);
    fclose(arq_ind_prim2);
    fclose(arq_cabec);

    return 0; 
}

int busca_binaria_int(int *array, int esquerda, int direita, int chave) {
    int meio;

    if (direita >= esquerda) {  // Se array existe
        meio = esquerda + (direita - esquerda)/2;

        if (chave == array[meio]) {
            return meio;

        } else if (chave < array[meio]) {
            return busca_binaria_int(array, esquerda, meio-1, chave);

        } else if (chave > array[meio]) {
            return busca_binaria_int(array, meio+1, direita, chave);
        }
    }
    return -1;
}
