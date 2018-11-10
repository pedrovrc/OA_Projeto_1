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
#include "pesquisa.h"

#define TAM_CAMPO1 50			// Tamanho do campo 1 no arquivo cabecalho
#define BUFFER 100

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

void insere_reg(int opcao, int tipo, int NRR) {

    int i, tam_ind1, tam_ind2, tamanho_nome, start, tam_string;
    char nome[41], curso[4], turma, linha[BUFFER], lixo[BUFFER];
    char matricula[6], OP[2];

    FILE *arq_base1 = fopen("arquivos_principais/lista1.txt", "r+");
    FILE *arq_base2 = fopen("arquivos_principais/lista2.txt", "r+");
    FILE *arq_cabec = fopen("arquivos_principais/cabecalho.txt", "r");

    printf("Entre com a nova matricula: (6 dígitos)\n");
    verifica_entrada(matricula, 6);
    printf("Entre com o nome: (Max 39 caracteres)\n");
    scanf("%c", lixo);
    scanf("%[^\n]", nome);
    printf("Entre com OP: (2 dígitos)\n");
    verifica_entrada(OP, 2);
    printf("Entre com as iniciais do curso: (2 digitos)\n");
    verifica_entrada(curso, 2);
    if (opcao == 0) {
        printf("Entre com a turma: (A ou B)\n");
        scanf("%c", lixo);
        scanf("%c", &turma);
        scanf("%[^\n]", lixo);
    } else {
        if (tipo == 1) {
            turma = 'A';
        } else {
            turma = 'B';
        }
    }

    if (opcao == 0) {
        tam_arqs_ind(arq_cabec, &tam_ind1, &tam_ind2);
    } else {
        tam_ind1 = NRR;
        tam_ind2 = NRR;
    }

    start = strlen(nome);
    for (int i = start; i < 40; i++) {
        nome[i] = ' '; 
    }
    nome[40] = '\0';

    if (turma != 'A') {
        for (i = 0; i < tam_ind2 && linha[0] != '*'; i++) {
            fgets(linha, BUFFER, arq_base2);
        }
        if (linha[0] == '*') {
            rewind(arq_base2);
            for (int j = 0; j < i-1; j++) {
                fgets(linha, BUFFER, arq_base2);   
            }
            opcao = 1;
        }
        fprintf(arq_base2, "%s %s %s  %s       %c", matricula, nome, OP, curso, turma);
        if (opcao == 0) {
            fprintf(arq_base2, "\n");
        }
    } else {
        for (i = 0; i < tam_ind1 && linha[0] != '*'; i++) {
            fgets(linha, BUFFER, arq_base1);
        }
        if (linha[0] == '*') {
            rewind(arq_base1);
            for (int j = 0; j < i-1; j++) {
                fgets(linha, BUFFER, arq_base1);   
            }
            opcao = 1;
        }
        fprintf(arq_base1, "%s %s %s  %s       %c", matricula, nome, OP, curso, turma);
        if (opcao == 0) {
            fprintf(arq_base1, "\n");
        }
    }

    fclose(arq_base1);
    fclose(arq_base2);
    fclose(arq_cabec);
    
    return;
}

void verifica_entrada(char *item, int tam_esperado) {

    int tam_string;

    do {
        scanf("%s", item);
        tam_string = strlen(item);
        if (tam_string != tam_esperado) {
            printf("Entrada Invalida! Tente novamente: \n");
        }
    } while (tam_string != tam_esperado);

    return;
}

void remove_reg(int tipo, int NRR) {

    char linha[BUFFER];
    FILE *arq_base;

    if (tipo == 1) {
        arq_base = fopen("arquivos_principais/lista1.txt", "r+");
    } else {
        arq_base = fopen("arquivos_principais/lista2.txt", "r+");
    }

    for (int i = 0; i < NRR; i++) {
        fgets(linha, BUFFER, arq_base);
    }
    fprintf(arq_base, "*** ");

    fclose(arq_base);
}
