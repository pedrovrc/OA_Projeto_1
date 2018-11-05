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
#include "manutencao.h"

#define TAM_CAMPO1 50			// Tamanho do campo 1 no arquivo cabecalho


/*
	Funcao atualiza_cabecalho:

		Sempre que chamada, essa funcao abrira o arquivo cabecalho
	e ira procurar o nome do arquivo passado a ela em cabecalho.
	Caso encontre, somente atualiza o numero de linhas desse arquivo.
	Caso nao encontre, cria novo registro para esse arquivo.
*/
void atualiza_cabecalho(char *nome_arq, int qtd_linha) {

    char campo1[TAM_CAMPO1];
    char campo_aux[TAM_CAMPO1];
    int tamanho_nome = strlen(nome_arq);
    FILE* cabecalho = fopen("arquivos_principais/cabecalho.txt", "r+");
    int flag = 0;

    // Preenche string auxiliar com espacos em branco
    for (int i = 0; i < TAM_CAMPO1-1; i++) {
        campo_aux[i] = ' ';
    }
    campo1[TAM_CAMPO1-1] = '\0';

    // Coloca nome do arquivo passado na string auxiliar
    for (int i = 0; i < tamanho_nome; i++) {
        campo_aux[i] = nome_arq[i];
    }

    // Procura por registro do arquivo ja existente em cabecalho.txt
    while (fgets(campo1, TAM_CAMPO1, cabecalho) != NULL) {
        if (strcmp(campo1, campo_aux) == 0) {
            flag = 1;
            break;
        }
        //ler restante do registro
        fgets(campo1, TAM_CAMPO1, cabecalho);
    }

   	// Se nao encontrou, cria novo registro ao final
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

    // Atualiza numero de linhas do arquivo
    fprintf(cabecalho, "%d\n", qtd_linha);
    fclose(cabecalho);

    return;
}
