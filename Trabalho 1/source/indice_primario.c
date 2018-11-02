#include "indice_primario.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define TAM_REG 70			// Tamanho do registro do arquivo base
#define TAM_CHAVE 30		// Tamanho da chave primaria do arquivo de indices
#define TAM_MATR_NOME 48	// Tamanho do campo matricula + nome
#define TAM_DIR 80			// Tamanho da string para nome do diretorio
#define POS_CURSO 52		// Posicao do campo de curso

int gera_indice_primario(FILE* arq_base, char* nome_gerado) {
	// Funcao recebe arquivo base de registros e nome para arquivo de indices

    char linha_atual[TAM_REG];	// String para armazenar linha lida de arq_base
    char chave_prim[TAM_CHAVE];	// String para concatenar matricula + nome
    							// Arq. de indices sera criado na pasta abaixo
    char caminho_arquivo[TAM_DIR] = "arquivos_principais/";
    int conta_char;				// Contador de caracteres inseridos em string[]
    int endereco_registro = 0;	// Contador de linhas lidas de arq_base

    // Se arquivo base nao existe, nao se cria arquivo de indices
    if (arq_base == NULL) {
        return 1;
    }

    // Concatena strings de diretorio e nome de arquivo fornecido pelo usuario
    strcat(caminho_arquivo, nome_gerado);
    // Cria arquivo com nome fornecido no diretorio definido anteriormente
    FILE* indices = fopen(caminho_arquivo, "w");
    // IMPORTANTE: Caso caminho seja invalido, nao se cria arquivo
    // Nesse caso, indices recebe NULL

    // Se caminho do arquivo de indices for invalido, funcao retorna
    if (indices == NULL) {
        return 2;
    }

    // A cada iteracao, garante-se que se leu uma linha nova de arq_base
    while (fgets(linha_atual, TAM_REG, arq_base) != NULL) {
        
        // Enche string com espacos para evitar sobras quando sobrescrever
        for (int i = 0; i < TAM_CHAVE; i++) {
            chave_prim[i] = ' ';
        }

        // Reseta contador de caracteres na string
        conta_char = 0;

        // Le campo desejado salvo em linha_atual[]
        // ate string encher ou campo acabar
        for (int i = 0; i < TAM_MATR_NOME && conta_char < TAM_CHAVE; i++) {	
            
            // Ignora espacos no campo desejado (nao se adiciona a string)
            if (isspace(linha_atual[i])) {
                continue;
            }

            // String recebe caracteres validos do campo
            chave_prim[conta_char] = linha_atual[i];
            // Incrementa +1 caracter entrado na string
            conta_char++;
        }

        // Salva no arquivo o registro transformado para indice
        fprintf(indices, "%s", chave_prim); // Campos matricula e nome concatenados
        fprintf(indices, " %d\n", endereco_registro); // NRR em arq_base
        
        // Incrementa toda vez que se adiciona novo indice (NRR)
        endereco_registro++;
    }

    // Reseta o ponteiro do arquivo base
    rewind(arq_base);

    fclose(indices);
    return 0;
}



int gera_indice_secundario(FILE* arq_base, FILE* ind_prim, char* nome_gerado) {

    char linha_atual[TAM_REG];
    //char recebe_chave[TAM_CHAVE];
    int conta_char;
    int endereco_registro = 0;
    char compara_curso[2];

    // Inicia lista de cursos
    inicia_curso *lista_comeco = (inicia_curso*)malloc(sizeof(inicia_curso));
    lista_comeco->prox = NULL;
    lista_comeco->primeiro = NULL;

    // Ponteiros auxiliares
    inicia_curso *aux = lista_comeco;
    inicia_curso *lista_ultimo = lista_comeco;
    inicia_chave *chave_aux;

    // Preenche primeira celula de curso
    fgets(linha_atual, TAM_REG, arq_base);
    lista_comeco->curso[0] = linha_atual[POS_CURSO];
    lista_comeco->curso[1] = linha_atual[POS_CURSO + 1];

    // Cria e preenche primeira celula de chave
    lista_comeco->primeiro = (inicia_chave*)malloc(sizeof(inicia_chave));
    fgets(lista_comeco->primeiro->chave, TAM_CHAVE, ind_prim); // Pega chave primaria
    lista_comeco->primeiro->NRR = endereco_registro;
    lista_comeco->primeiro->prox = NULL;
    endereco_registro++;

    while (fgets(linha_atual, TAM_REG, arq_base) != NULL) {

        compara_curso[0] = linha_atual[POS_CURSO];
        compara_curso[1] = linha_atual[POS_CURSO + 1];
        aux = lista_comeco;
        
        //Para até o final da lista ou até encontrar um curso existente 
        while (aux != NULL && strcmp(compara_curso, aux->curso) != 0) {
            aux = aux->prox;
        }

        //Se chegou ao final cria-se uma nova célula de curso
        if (aux == NULL) {
            aux = (inicia_curso*)malloc(sizeof(inicia_curso));
            strcpy(aux->curso, compara_curso);
            aux->prox = NULL;
            aux->primeiro = NULL;
            lista_ultimo->prox = aux;
            lista_ultimo = aux;
        }

        //Cria-se nova chave a partir do registro lido 
        chave_aux = aux->primeiro;
        aux->primeiro = (inicia_chave*)malloc(sizeof(inicia_chave));
        fgets(aux->primeiro->chave, TAM_CHAVE, ind_prim); // Pega chave primaria
        aux->primeiro->NRR = endereco_registro;

        //Encadeia
        if (chave_aux != NULL) {
            aux->primeiro->prox = chave_aux;
        } else {
            aux->primeiro->prox = NULL;
        }

        endereco_registro++;
    }

    escreve_arq_secundario(lista_comeco, nome_gerado);

    //desalocar
    return 0;
}

void escreve_arq_secundario(inicia_curso *lista_comeco, char *nome_gerado) {

    char caminho_indice[TAM_DIR] = "arquivos_principais/ind_sec_";
    char caminho_label[TAM_DIR] = "arquivos_principais/label_id_";
    strcat(caminho_indice, nome_gerado);
    strcat(caminho_label, nome_gerado);
    int flag; //0 = Primeira inserção de chave. 1 = cc.
    int linha_atual = 0;

    FILE *ind_sec = fopen(caminho_indice, "w");
    FILE *label_id = fopen(caminho_label, "w");

    inicia_curso *curso_aux = lista_comeco;
    inicia_chave *chave_aux;

    while (curso_aux != NULL) {
        chave_aux = curso_aux->primeiro;
        flag = 0;
        while (chave_aux != NULL) {
            if (flag == 0) {
                fprintf(label_id, "%s -1\n", chave_aux->chave);
                flag = 1;
            } else {
                fprintf(label_id, "%s %d\n", chave_aux->chave, linha_atual-1);
            }
            linha_atual++;
            chave_aux = chave_aux->prox;
        }
        fprintf(ind_sec, "%s %d\n", curso_aux->curso, linha_atual-1);
        curso_aux = curso_aux->prox;
    }

    fclose(label_id);
    fclose(ind_sec);
    return;
}
