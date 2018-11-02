#include "indice.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define TAM_REG 65			// Tamanho do registro do arquivo base
#define TAM_CHAVE 30		// Tamanho da chave primaria do arquivo de indices
#define TAM_MATR_NOME 48	// Tamanho do campo matricula + nome no arq. base
#define TAM_DIR 80			// Tamanho da string para nome do diretorio
#define POS_CURSO 52		// Posicao do campo de curso no arquivo base
#define TAM_PULO 10			// Tamanho de string pra ajustar ponteiro de arq.


int gera_ind_prim(FILE* arq_base, char* nome_gerado) {

    char linha_atual[TAM_REG];
    char chave_prim[TAM_CHAVE];
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

    // A cada iteracao, garante-se que se leu uma linha nova de arq_base
    while (fgets(linha_atual, TAM_REG, arq_base) != NULL) {
        
        // Enche string com espacos para evitar sobras quando sobrescrever
        for (int i = 0; i < TAM_CHAVE; i++) {
            chave_prim[i] = ' ';
        }

        conta_char = 0;

        // Le campo desejado salvo em linha_atual[]
        // ate string encher ou campo acabar
        for (int i = 0; i < TAM_MATR_NOME && conta_char < TAM_CHAVE; i++) {	
            if (isspace(linha_atual[i])) {
                continue;
            }
            chave_prim[conta_char] = linha_atual[i];
            conta_char++;
        }

        fprintf(indices, "%s", chave_prim); // Campos matricula e nome concat.
        fprintf(indices, " %d\n", endereco_registro); // NRR em arq_base

        endereco_registro++;
    }

    rewind(arq_base);
    atualiza_cabecalho(nome_gerado, endereco_registro);
    fclose(indices);
    return 0;
}
/*
		Explicacao da Estrutura de Dados (ED) usada nas funcoes gera_ind_sec()
	e escreve_arqs_sec():
	
		Consiste de uma lista de structs do tipo inicia_curso, com uma celula
	para cada curso unico do arquivo base.
		De cada celula de curso, sai uma lista de structs do tipo inicia_chave,
	com uma celula para cada registro (aluno) pertencente aquele curso no
	arquivo base.
		Detalhes das structs no arquivo .h
*/

int gera_ind_sec(FILE* arq_base, FILE* ind_prim, char* nome_gerado) {

    char linha_atual[TAM_REG];
    char ajeita_ponteiro[TAM_PULO];
    char compara_curso[3];

    // Inicia lista de cursos
    inicia_curso *lista_comeco = (inicia_curso*)malloc(sizeof(inicia_curso));
    lista_comeco->prox = NULL;
    lista_comeco->primeiro = NULL;

    // Preenche primeira celula de curso
    fgets(linha_atual, TAM_REG, arq_base);
    lista_comeco->curso[0] = linha_atual[POS_CURSO];
    lista_comeco->curso[1] = linha_atual[POS_CURSO + 1];
    lista_comeco->curso[2] = '\0';

    // Declaracao de ponteiros auxiliares
    inicia_curso *aux = lista_comeco;
    inicia_curso *lista_ultimo = lista_comeco;
    inicia_curso *sentinela_curso;
    inicia_chave *chave_aux;
    inicia_chave *sentinela_chave;

    // Cria e inicia primeira celula de chave
    lista_comeco->primeiro = (inicia_chave*)malloc(sizeof(inicia_chave));
    fgets(lista_comeco->primeiro->chave, TAM_CHAVE, ind_prim);
    lista_comeco->primeiro->prox = NULL;

    // Posiciona ponteiro na prox linha
    fgets(ajeita_ponteiro, TAM_PULO, ind_prim);

    // A cada iteracao, garante-se que se leu uma linha nova de arq_base
    while (fgets(linha_atual, TAM_REG, arq_base) != NULL) {

    	// Inicia string compara_curso com curso do registro lido
        compara_curso[0] = linha_atual[POS_CURSO];
        compara_curso[1] = linha_atual[POS_CURSO + 1];
        compara_curso[2] = '\0';
        aux = lista_comeco;
        
        // Procura curso existente na ED 
        while (aux != NULL && strcmp(compara_curso, aux->curso) != 0) {
            aux = aux->prox;
        }

        // Caso nao exista, cria nova celula de curso
        if (aux == NULL) {
            aux = (inicia_curso*)malloc(sizeof(inicia_curso));
            strcpy(aux->curso, compara_curso);
            aux->prox = NULL;
            aux->primeiro = NULL;
            lista_ultimo->prox = aux;
            lista_ultimo = aux;
        }

        // Cria e inicia nova celula de chave a partir do registro lido 
        chave_aux = aux->primeiro;
        aux->primeiro = (inicia_chave*)malloc(sizeof(inicia_chave));
        fgets(aux->primeiro->chave, TAM_CHAVE, ind_prim);

        // Posiciona ponteiro na prox linha
        fgets(ajeita_ponteiro, TAM_PULO, ind_prim);

        //Encadeia chave nova
        if (chave_aux != NULL) {
            aux->primeiro->prox = chave_aux;
        } else {
            aux->primeiro->prox = NULL;
        }
    }

    escreve_arqs_sec(lista_comeco, nome_gerado);

    // Desaloca ED
    while (lista_comeco != NULL) {
    	while (lista_comeco->primeiro != NULL) {
    		sentinela_chave = lista_comeco->primeiro;
    		lista_comeco->primeiro = lista_comeco->primeiro->prox;
    		free(sentinela_chave);
    	}
    	sentinela_curso = lista_comeco;
    	lista_comeco = lista_comeco->prox;
    	free(sentinela_curso);
    }
    rewind(ind_prim);
    rewind(arq_base);
    return 0;
}


void escreve_arqs_sec(inicia_curso *lista_comeco, char *nome_gerado) {

    char caminho_indice[TAM_DIR] = "arquivos_principais/ind_sec_";
    char caminho_label[TAM_DIR] = "arquivos_principais/label_id_";
    strcat(caminho_indice, nome_gerado);
    strcat(caminho_label, nome_gerado);

    int flag; // Valores: 0 = Primeira insercao de chave; 1 = Caso contrario
    int linha_atual_lista = 0;
    int conta_linhas_ind = 0;

    FILE *ind_sec = fopen(caminho_indice, "w");
    FILE *label_id = fopen(caminho_label, "w");

    inicia_curso *curso_aux = lista_comeco;
    inicia_chave *chave_aux;

    // Para toda celula de curso da ED
    while (curso_aux != NULL) {
        chave_aux = curso_aux->primeiro;
        flag = 0;

        // Para toda celula de chave dessa celula de curso
        while (chave_aux != NULL) {
        	//Cria lista invertida
            if (flag == 0) {
                fprintf(label_id, "%s -1\n", chave_aux->chave);
                flag = 1;
            } else {
                fprintf(label_id, "%s %d\n", chave_aux->chave, linha_atual_lista-1);
            }
            linha_atual_lista++;
            chave_aux = chave_aux->prox;
        }

        // Adiciona registro de indice secundario
        fprintf(ind_sec, "%s %d\n", curso_aux->curso, linha_atual_lista-1);
        conta_linhas_ind++;
        curso_aux = curso_aux->prox;
    }

    atualiza_cabecalho(caminho_indice, conta_linhas_ind);
    fclose(label_id);
    fclose(ind_sec);
    return;
}
