#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "pesquisa.h"

#define BUFFER 100          // Buffer arbitrario
#define POS_QTD_LINHA 49    // Posicao do numero de linhas no cabecalho
#define TAM_REG 65          // Tamanho do registro em arq_base
#define TAM_CHAVE 30        // Tamanho da chave primaria do arquivo de indices

int pesquisa(int opcao, char *reg1, char* reg2) {

    int falha;

    switch (opcao) {
        case 1:
            falha = pesquisa_matricula(reg1, reg2);
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

int pesquisa_matricula(char *reg1, char* reg2) {
    
    FILE *arq_base1 = fopen("arquivos_principais/lista1.txt", "r");
    FILE *arq_base2 = fopen("arquivos_principais/lista2.txt", "r");
    FILE *arq_ind_prim1 = fopen("arquivos_principais/indicelista1.ind", "r");
    FILE *arq_ind_prim2 = fopen("arquivos_principais/indicelista2.ind", "r");
    FILE *arq_cabec = fopen("arquivos_principais/cabecalho.txt", "r");
    int matricula, tam_ind1, tam_ind2, chave_lida, pos_array = -1;
    int NRR = 0, indica_array = -1;
    char linha[BUFFER];

    // Pega chave de busca do usuario
    printf("Entre com a matricula: (6 digitos)\n");
    scanf("%d", &matricula);
    printf("\n");

    // fgets: posiciona ponteiro no campo de qtd_linhas do cabecalho
    // caso null, cabecalho nao iniciado -> sem base de dados
    if (fgets(linha, POS_QTD_LINHA, arq_cabec) == NULL) {
        printf("Nao ha base de dados carregada.\n");
        return 1;
    }

    // Le numero de linhas do arq_ind1 no cabecalho
    fscanf(arq_cabec, "%d", &tam_ind1);

    // Posiciona ponteiro na prox linha
    fgets(linha, BUFFER, arq_cabec);

    // repete para arq_ind2
    if (fgets(linha, POS_QTD_LINHA, arq_cabec) == NULL) {
        printf("Nao ha base de dados carregada.\n");
        return 1;
    }
    fscanf(arq_cabec, "%d", &tam_ind2);

    // Cria arrays para busca
    int chaves_primarias1[tam_ind1];
    int chaves_primarias2[tam_ind2];

    // Le e preenche arrays com chaves primarias
    // para ind_prim_1
    for (int i = 0; i < tam_ind1; i++) {
        // Le valor da chave
        fscanf(arq_ind_prim1, "%d", &chave_lida);
        // Salva no array
        chaves_primarias1[i] = chave_lida;
        // Pula pra prox linha
        fgets(linha, BUFFER, arq_ind_prim1);
    }

    // para ind_prim_2
    for (int i = 0; i < tam_ind2; i++) {
        // Le valor da chave
        fscanf(arq_ind_prim2, "%d", &chave_lida);
        // Salva no array
        chaves_primarias2[i] = chave_lida;
        // Pula pra prox linha
        fgets(linha, BUFFER, arq_ind_prim2);
    }

    // Busca binaria no array1
    pos_array = busca_binaria_int(chaves_primarias1, 0, tam_ind1, matricula);

    if (pos_array >= 0) {
        rewind(arq_ind_prim1);
        // Le arq_ind ate achar registro (usa pos_array pra isso)
        for (int i = 0; i < pos_array; i++) {
            fgets(linha, BUFFER, arq_ind_prim1);
            //printf("linha = |%s|\n", linha);
        }

        // Pega NRR dentro de arq_ind
        fgets(linha, TAM_CHAVE+2, arq_ind_prim1);
        //printf("linha = |%s|\n", linha);
        fscanf(arq_ind_prim1, "%d", &NRR);
        //printf("NRR = %d\n", NRR);

        // Le arq_base (usa NRR) e salva registro em string
        for (int i = 0; i < NRR; i++) {
            fgets(linha, BUFFER, arq_base1);
        }
        fgets(reg1, TAM_REG, arq_base1);
    } else {
        reg1[0] = '*';
    }

    // Busca binaria no array1
    pos_array = busca_binaria_int(chaves_primarias2, 0, tam_ind2, matricula);

    if (pos_array >= 0) {
        rewind(arq_ind_prim2);
        // Le arq_ind ate achar registro (usa pos_array pra isso)
        for (int i = 0; i < pos_array; i++) {
            fgets(linha, BUFFER, arq_ind_prim2);
            //printf("linha = |%s|\n", linha);
        }

        // Pega NRR dentro de arq_ind
        fgets(linha, TAM_CHAVE+2, arq_ind_prim2);
        //printf("linha = |%s|\n", linha);
        fscanf(arq_ind_prim2, "%d", &NRR);
        //printf("NRR = %d\n", NRR);

        // Le arq_base (usa NRR) e salva registro em string
        for (int i = 0; i < NRR; i++) {
            fgets(linha, BUFFER, arq_base2);
        }
        fgets(reg2, TAM_REG, arq_base2);
    } else {
        reg2[0] = '*';
    }

    fclose(arq_base1);
    fclose(arq_base2);
    fclose(arq_ind_prim1);
    fclose(arq_ind_prim2);
    fclose(arq_cabec);

    return 0; 
}

// Retorna posicao do array com a chave, se nao encontrar retorna -1
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
