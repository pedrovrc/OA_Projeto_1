/*
	Codigo feito por:
		Pedro Victor Rodrigues de Carvalho (17/0113043)
		Victor Eduardo Fernandes Castor	(17/0115127)

	Universidade de Brasília, Segundo semestre de 2018
	Curso: Engenharia de Computacao
	Materia: Organizacao de Arquivos
	Professor: Camilo Dorea
	Projeto 1: Indexacao de registros
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ordenacao.h"

#define BUFFER 40			// Tamanho de buffer arbitrario
#define TAM_DIR 80          // Tamanho da string para nome do diretorio


/*
	Funcao ordena_arq:

		Responsavel por abrir arquivo cujo nome foi passado a funcao, ler
	o conteudo do arquivo, ordenar o conteudo e sobrescrever os dados desor-
	denados com os ordenados.
		Os passos de ler conteudo e ordernar e sobrescrever sao feitos por
	chamadas das funcoes le_strings e ordena, respectivamente.
*/
int ordena_arq(char* nome_gerado) {

    char caminho_arquivo[TAM_DIR] = "arquivos_principais/";
    cell_string *inicio;
    cell_string *aux;

    // Abre arquivo passado para a funcao
    strcat(caminho_arquivo, nome_gerado);
    FILE* arquivo = fopen(caminho_arquivo, "r");

    // Se arquivo nao pode ser aberto, retorna
    if (arquivo == NULL) {
        return 2;
    }

    // Pega lista com registros do arquivo
    inicio = le_strings(arquivo);
    fclose(arquivo);

    // Reabre o arquivo para sobrescrever
    arquivo = fopen(caminho_arquivo, "w");

    // Ordena e sobrescreve
    ordena(inicio, arquivo);
    fclose(arquivo);
    
    // Desalocar
    while (inicio != NULL) {
        aux = inicio->prox;
        free(inicio);
        inicio = aux;
    }

    return 0;
}


/*
	Funcao le_strings:

		Responsavel por ler os conteudos de um arquivo e armazena-los numa
	lista de celulas q contem o registro lido em uma string e um ponteiro
	para a celula seguinte.
*/
cell_string *le_strings(FILE* arquivo) {

    int flag = 0; // 0 -> Primeira insercao na lista; 1 -> Caso contrario
    char linha_atual[BUFFER];
    cell_string *inicio;
    cell_string *ultimo;
    cell_string *aux;

    // Le uma linha completa do arquivo a cada iteracao
    while (fgets(linha_atual, BUFFER, arquivo) != NULL) {
    	// Se for primeira insercao, cria primeira celula da lista
        if (flag == 0) {
            inicio = malloc(sizeof(cell_string));
            aux = inicio;
            inicio->ant = NULL;
            ultimo = inicio;
        } else {
        	// Se nao, cria celula nova
            aux->prox = malloc(sizeof(cell_string));
            aux = aux->prox;
        }
        // Preenche dados da string
        strcpy(aux->string, linha_atual);
        aux->prox = NULL;
        // Se nao for primeira insercao, encadeia
        if (flag != 0) {
            aux->ant = ultimo;
        }
        ultimo = aux;
        flag = 1;
    }

    // Retorna lista criada
    return inicio;
}

/*
	Funcao ordena:

		Utiliza um algoritmo de insertion sort para ordenar a lista fornecida
	e escreve lista ordenada no arquivo fornecido.
		O insertion sort foi escolhido devido a sua boa performance com dados
	semi ordenados, que é o caso dos arquivos de indice primario e secundario.
*/
void ordena(cell_string *inicio, FILE* arquivo) {

    cell_string *aux = inicio;
    cell_string *percorre;
    cell_string *templario = aux->prox;
    char guarda[BUFFER], tras[BUFFER];

/*
    Ponteiros auxiliares:
    -aux: aponta para o ultimo elemento ordenado
    -templario: aponta para o elemento a ser ordenado (a frente de aux)
    -percorre: inicia em aux, mas percorre lista para tras
    	para permitir comparacoes do insertion sort
*/

    // Enquanto ainda existir elementos a se ordenar
    while (templario != NULL) {

    	// Pega valor do elemento a se ordenar
        strcpy(guarda, templario->string);
        percorre = aux;
        // Pega valor do primeiro elemento a comparar
        strcpy(tras, percorre->string);

        // Enquanto templario nao estiver na posicao certa
        while (percorre != NULL && compara_string(guarda, tras) != 1) {
        	// Passa templario para tras, percorre para frente
        	// e repete comparacao
            strcpy(templario->string, percorre->string);
            percorre = percorre->ant;
            templario = templario->ant;
            if (percorre != NULL) 
                strcpy(tras, percorre->string);
        }

        // Achada a posicao certa, salva string no local
        // e repete para proximo elemento
        strcpy(templario->string, guarda);
        aux = aux->prox;
        templario = aux->prox;
    }

    // Escreve lista ordenada no arquivo
    aux = inicio;
    while (aux != NULL) {
        fprintf(arquivo, "%s", aux->string);
        aux = aux->prox;
    }
    fprintf(arquivo, "\n");

    return;
}


/*
	Funcao compara_string:

		Utiliza strcmp e uma comparacao caracter por caracter para retornar qual
	string esta mais acima em ordem alfabetica que a outra.
*/
// Retornos:
// 0 -> strings iguais | 1 -> string1 > string2 alfabeticamente | -1 -> contrário de 1
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
