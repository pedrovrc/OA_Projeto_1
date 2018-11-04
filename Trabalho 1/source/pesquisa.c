#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "pesquisa.h"

#define BUFFER 100          // Buffer arbitrario
#define POS_QTD_LINHA 49    // Posicao do numero de linhas no cabecalho
#define TAM_REG 65          // Tamanho do registro em arq_base
#define TAM_CHAVE 30        // Tamanho da chave primaria do arquivo de indices

lista_NRR *pesquisa(int opcao) {

    lista_NRR *retorno;

    switch (opcao) {
        case 1:
            retorno = pesquisa_matricula();
            break;
        case 2:
            retorno = pesquisa_curso();
            break;
        case 3:
            retorno = pesquisa_nome();
            break;
    }
    return retorno;
}

lista_NRR *pesquisa_matricula() {
    
    FILE *arq_base1 = fopen("arquivos_principais/lista1.txt", "r");
    FILE *arq_base2 = fopen("arquivos_principais/lista2.txt", "r");
    FILE *arq_ind_prim1 = fopen("arquivos_principais/indicelista1.ind", "r");
    FILE *arq_ind_prim2 = fopen("arquivos_principais/indicelista2.ind", "r");
    FILE *arq_cabec = fopen("arquivos_principais/cabecalho.txt", "r");
    char indicacao[TAM_REG+4] = "   MATRIC NOME                                     OP  CURSO    TURMA";
    int matricula, tam_ind1, tam_ind2, chave_lida, pos_array = -1;
    int NRR = 0, indica_array = -1;
    char linha[BUFFER], reg1[TAM_REG], reg2[TAM_REG];
    int conta_print = 0;

    lista_NRR *inicio_lista = (lista_NRR*)malloc(sizeof(lista_NRR));
    inicio_lista->prox = NULL;
    inicio_lista->NRR = -1;
    inicio_lista->tipo = 0;

    // Pega chave de busca do usuario
    printf("Entre com a matricula: (6 digitos)\n");
    scanf("%d", &matricula);
    printf("\n");

    // fgets: posiciona ponteiro no campo de qtd_linhas do cabecalho
    // caso null, cabecalho nao iniciado -> sem base de dados
    if (fgets(linha, POS_QTD_LINHA, arq_cabec) == NULL) {
        printf("Nao ha base de dados carregada.\n");
        return inicio_lista;
    }

    // Le numero de linhas do arq_ind1 no cabecalho
    fscanf(arq_cabec, "%d", &tam_ind1);
    // Posiciona ponteiro na prox linha
    fgets(linha, BUFFER, arq_cabec);

    // repete para arq_ind2
    if (fgets(linha, POS_QTD_LINHA, arq_cabec) == NULL) {
        printf("Nao ha base de dados carregada.\n");
        return inicio_lista;
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
        
        printf("%s\n", indicacao);
        inicio_lista->NRR = NRR;
        inicio_lista->tipo = 1;
        
        // Le arq_base (usa NRR) e salva registro em string
        for (int i = 0; i < NRR; i++) {
            fgets(linha, BUFFER, arq_base1);
        }
        fgets(reg1, TAM_REG, arq_base1);
        conta_print++;
        printf("%d. %s", conta_print, reg1);
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

        if (NRR != -1) {
            inicio_lista->prox = (lista_NRR*)malloc(sizeof(lista_NRR));
            inicio_lista->prox->NRR = NRR;
            inicio_lista->prox->prox = NULL;
        } else {
            printf("%s\n", indicacao);
            inicio_lista->NRR = NRR;
        }
        inicio_lista->tipo = 2;

        // Le arq_base (usa NRR) e salva registro em string
        for (int i = 0; i < NRR; i++) {
            fgets(linha, BUFFER, arq_base2);
        }
        fgets(reg2, TAM_REG, arq_base2);
        conta_print++;
        printf("%d. %s", conta_print, reg2);
    }

    fclose(arq_base1);
    fclose(arq_base2);
    fclose(arq_ind_prim1);
    fclose(arq_ind_prim2);
    fclose(arq_cabec);

    return inicio_lista; 
}

lista_NRR *pesquisa_nome() {

    FILE *arq_base1 = fopen("arquivos_principais/lista1.txt", "r");
    FILE *arq_base2 = fopen("arquivos_principais/lista2.txt", "r");
    char indicacao[TAM_REG+4] = "   MATRIC NOME                                     OP  CURSO    TURMA";
    char nome[TAM_REG], linha[BUFFER], reg[TAM_REG];
    int NRR1 = -1, NRR2 = -1;
    int conta_print = 0;
    char lixo;

    lista_NRR *inicio_lista = (lista_NRR*)malloc(sizeof(lista_NRR));
    inicio_lista->prox = NULL;
    inicio_lista->NRR = -1;
    inicio_lista->tipo = 0;

    // Pega chave de busca do usuario
    printf("Entre com o nome: (Primeiras letras maiusculas)\n");
    fflush(stdin);
    scanf("%c", &lixo);
    scanf("%[^\n]", nome);

    NRR1 = busca_sequencial(nome, arq_base1);
    inicio_lista->NRR = NRR1;
    inicio_lista->tipo = 1;

    NRR2 = busca_sequencial(nome, arq_base2);
    if (NRR1 == -1) {
        inicio_lista->NRR = NRR2;
    } else {
        inicio_lista->prox = (lista_NRR*)malloc(sizeof(lista_NRR));
        inicio_lista->prox->NRR = NRR2;
        inicio_lista->prox->prox = NULL;
    }
    inicio_lista->tipo = 2;

    if (NRR1 != -1) {
        for (int i = 0; i < NRR1; i++) {
            fgets(linha, BUFFER, arq_base1);
        }
        fgets(reg, TAM_REG, arq_base1);
        conta_print++;
        printf("%d. %s", conta_print, reg);
    }

    if (NRR2 != -1) {
        for (int i = 0; i < NRR2; i++) {
            fgets(linha, BUFFER, arq_base2);
        }
        fgets(reg, TAM_REG, arq_base2);
        conta_print++;
        printf("%d. %s", conta_print, reg);
    }

    fclose(arq_base1);
    fclose(arq_base2);

    return inicio_lista;
}

lista_NRR *pesquisa_curso() {
    FILE *arq_base1 = fopen("arquivos_principais/lista1.txt", "r");
    FILE *arq_base2 = fopen("arquivos_principais/lista2.txt", "r");
    FILE *arq_ind_prim1 = fopen("arquivos_principais/indicelista1.ind", "r");
    FILE *arq_ind_prim2 = fopen("arquivos_principais/indicelista2.ind", "r");
    FILE *arq_ind_sec1 = fopen("arquivos_principais/ind_sec_lista1.ind", "r");
    FILE *arq_ind_sec2 = fopen("arquivos_principais/ind_sec_lista2.ind", "r");
    FILE *arq_label_id1 = fopen("arquivos_principais/label_id_lista1.ind", "r");
    FILE *arq_label_id2 = fopen("arquivos_principais/label_id_lista2.ind", "r");
    FILE *arq_cabec = fopen("arquivos_principais/cabecalho.txt", "r");
    char indicacao[TAM_REG+4] = "   MATRIC NOME                                     OP  CURSO    TURMA";
    char linha[BUFFER];
    char lixo, curso[2], reg[TAM_REG];
    int conta_print = 0, tam_ind1, tam_ind2;

    lista_NRR *inicio_lista1;
    lista_NRR *inicio_lista2;
    lista_NRR *aux;

    // Pega chave de busca do usuario
    printf("Entre com o curso: (Letras Maiusculas, 2 letras)\n");
    scanf("%c", &lixo);
    scanf("%s", curso);
    
    // fgets: posiciona ponteiro no campo de qtd_linhas do cabecalho
    // caso null, cabecalho nao iniciado -> sem base de dados
    if (fgets(linha, POS_QTD_LINHA, arq_cabec) == NULL) {
        printf("Nao ha base de dados carregada.\n");
        //NAO ESQUECER DE MALLOC
        return inicio_lista1;
    }

    // Le numero de linhas do arq_ind1 no cabecalho
    fscanf(arq_cabec, "%d", &tam_ind1);
    // Posiciona ponteiro na prox linha
    fgets(linha, BUFFER, arq_cabec);

    // repete para arq_ind2
    if (fgets(linha, POS_QTD_LINHA, arq_cabec) == NULL) {
        printf("Nao ha base de dados carregada.\n");
        //NAO ESQUECER DE MALLOC
        return inicio_lista1;
    }
    fscanf(arq_cabec, "%d", &tam_ind2);

    inicio_lista1 = busca_curso(curso, 1, tam_ind1, arq_ind_sec1, arq_label_id1, arq_ind_prim1);
    inicio_lista2 = busca_curso(curso, 2, tam_ind2, arq_ind_sec2, arq_label_id2, arq_ind_prim2);

    if (inicio_lista1->NRR == -1 && inicio_lista2->NRR != -1) {
        
        printf("%s\n", indicacao);
        aux = inicio_lista2;
        while (aux != NULL) {
            for (int i = 0; i < aux->NRR; i++) {
                fgets(linha, BUFFER, arq_base2);
            }
            fgets(reg, TAM_REG, arq_base2);
            conta_print++;
            printf("%d. %s", conta_print, reg);
            aux = aux->prox;
            rewind(arq_base2);
        }
        return inicio_lista2;

    } else if (inicio_lista1->NRR != -1 && inicio_lista2->NRR == -1) {
        
        printf("%s\n", indicacao);
        aux = inicio_lista1;
        while (aux != NULL) {
            for (int i = 0; i < aux->NRR; i++) {
                fgets(linha, BUFFER, arq_base1);
            }
            fgets(reg, TAM_REG, arq_base1);
            conta_print++;
            printf("%d. %s", conta_print, reg);
            aux = aux->prox;
            rewind(arq_base1);
        }
        return inicio_lista1;

    } else if (inicio_lista1->NRR == -1 && inicio_lista2->NRR == -1) {
        return inicio_lista1;

    } else {

        printf("%s\n", indicacao);
        aux = inicio_lista1;
        while (aux != NULL) {
            for (int i = 0; i < aux->NRR; i++) {
                fgets(linha, BUFFER, arq_base1);
            }
            fgets(reg, TAM_REG, arq_base1);
            conta_print++;
            printf("%d. %s", conta_print, reg);
            aux = aux->prox;
            rewind(arq_base1);
        }

        aux = inicio_lista2;
        while (aux != NULL) {
            for (int i = 0; i < aux->NRR; i++) {
                fgets(linha, BUFFER, arq_base2);
            }
            fgets(reg, TAM_REG, arq_base2);
            conta_print++;
            printf("%d. %s", conta_print, reg);
            aux = aux->prox;
            rewind(arq_base2);
        }

        aux = inicio_lista1;
        while (aux->prox != NULL) {
            aux = aux->prox;
        }
        aux->prox = inicio_lista2;

        return inicio_lista1;
    }
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

int busca_sequencial(char *chave, FILE *arq_base) {

    int pula = 8, confirma = -1, NRR = -1;
    int tamanho_chave;
    char lixo[BUFFER];

    tamanho_chave = strlen(chave);
    char recebe_campo[tamanho_chave + 1];

    while (fgets(lixo, pula, arq_base) != NULL && confirma != 0) {
        fgets(recebe_campo, tamanho_chave + 1, arq_base);
        fgets(lixo, BUFFER, arq_base);
        confirma = strcmp(chave, recebe_campo);
        NRR++;
    }

    rewind(arq_base);
    if (confirma != 0){
        return -1;       
    } else {
        return NRR;
    }
}

lista_NRR *busca_curso(char *chave, int tipo, int tam_ind, FILE *arq_ind_sec, FILE *arq_label_id, FILE *arq_ind_prim) {

    lista_NRR *cabeca;
    lista_NRR *ultimo;
    int tam_curso = 2, ref_label = -1, matricula, flag = 0, chave_lida;
    int NRR, ref_ind_prim;
    char linha[BUFFER], le_curso[tam_curso + 1];
    char chave_prim[TAM_CHAVE + 2];

    cabeca = (lista_NRR*)malloc(sizeof(lista_NRR));
    cabeca->prox = NULL;
    cabeca->NRR = -1;
    cabeca->tipo = 0;
    ultimo = cabeca;

    // Cria arrays para busca
    int chaves_primarias[tam_ind];

    // Le e preenche arrays com chaves primarias
    for (int i = 0; i < tam_ind; i++) {
        // Le valor da chave
        fscanf(arq_ind_prim, "%d", &chave_lida);
        // Salva no array
        chaves_primarias[i] = chave_lida;
        // Pula pra prox linha
        fgets(linha, BUFFER, arq_ind_prim);
    }

    while (fgets(le_curso, tam_curso + 1, arq_ind_sec) != NULL) {
        if (strcmp(chave, le_curso) == 0) {
            fscanf(arq_ind_sec, "%d", &ref_label);
            break;
        }
        fgets(linha, BUFFER, arq_ind_sec);
    }

    if (ref_label == -1) {
        return cabeca;
    }

    while (ref_label != -1) {
        rewind(arq_ind_prim);
        for (int i = 0; i < ref_label; i++) {
            fgets(linha, BUFFER, arq_label_id);
        }
        // armazena matricula
        fscanf(arq_label_id, "%d", &matricula);
        // pula nome no campo
        fgets(chave_prim, TAM_CHAVE-6, arq_label_id);
        // armazena referencia pra prox elemento da lista invertida
        fscanf(arq_label_id, "%d", &ref_label);
        fgets(linha, BUFFER, arq_label_id);

        // usa busca binaria pra encontrar posicao da matricula no array de ind_prim (criar esse array)
        ref_ind_prim = busca_binaria_int(chaves_primarias, 0, tam_ind, matricula);

        // pula as linhas no arq_ind_prim
        for (int i = 0; i < ref_ind_prim; i++) {
            fgets(linha, BUFFER, arq_ind_prim);
        }
        // pula chave_prim pra chegar à NRR
        fgets(chave_prim, TAM_CHAVE + 2, arq_ind_prim);
        // pega NRR
        fscanf(arq_ind_prim, "%d", &NRR);
        if (flag == 0) {
            cabeca->NRR = NRR;
            cabeca->tipo = tipo;
            flag = 1;
        } else {
            ultimo->prox = (lista_NRR*)malloc(sizeof(lista_NRR));
            ultimo = ultimo->prox;
            ultimo->NRR = NRR;
            ultimo->tipo = tipo;
            ultimo->prox = NULL;
        }
        rewind(arq_label_id);
    }

    return cabeca;
}
