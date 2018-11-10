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
#include <stdlib.h>
#include "indice.h"
#include "ordenacao.h"
#include "pesquisa.h"
#include "manutencao.h"

#define TAM_REG 65			// Tamanho do registro no arquivo base

// Declaracao de funcoes usadas na main
void modifica(lista_NRR *pRetorno);
void atualiza_arquivos();
int menu();


/*
	Funcao main:

		Responsavel por chamar as funcoes atualiza_arquivos e menu, para
	atualizar a base de dados e printar um menu na tela do usuario,
	respectivamente.
		Tambem, a partir do retorno de menu, chama a opcao escolhida pelo
	usuario, que pode ser pesquisa, visualizacao da base de dados ou sair.
*/
int main() {

    int opcao = -1;
    char linha[TAM_REG];

    atualiza_arquivos();
    lista_NRR *aux;
    lista_NRR *sentinela;
		
	do {
		opcao = menu();

		if (opcao >= 1 && opcao <= 3) {
		    lista_NRR *pRetorno = pesquisa(opcao);
		    if (pRetorno->NRR == -1) {
		        printf("Registro nao encontrado!\n");
		        //possível inserção
		    } else {
		        //atualizar ou excluir
		        modifica(pRetorno);
		        aux = pRetorno->prox;
		        sentinela = pRetorno;
		        while (aux != NULL) {
		        	free(sentinela);
		        	sentinela = aux;
		        	aux = aux->prox;
		        }
		        free(sentinela);
		    }
		}

		if (opcao == 4) {
			FILE *arq_interc = fopen("arquivos_principais/interc.txt", "r");
    		printf("\nMATRIC NOME                                     OP  CURSO    TURMA\n");
    		while (fgets(linha, TAM_REG, arq_interc) != NULL) {
    			printf("%s", linha);
    		}
    		fclose(arq_interc);
		}

	} while (opcao != 6);

    return 0;
}


/*
	Funcao atualiza_arquivos:

		Responsavel por gerar arquivos de indice primario e secundario
	atualizados toda vez que o programa executar.
		Arquivos precisam ser fechados para que seus conteudos sejam
	salvos em disco, para entao poderem ser usados pelas outras funcoes
	do programa.
*/
void atualiza_arquivos() {

	// Abre arquivos base e cria arquivo cabecalho
    FILE* lista1 = fopen("arquivos_principais/lista1.txt", "r+");
    FILE* lista2 = fopen("arquivos_principais/lista2.txt", "r+");
    FILE* cabecalho = fopen("arquivos_principais/cabecalho.txt", "w");
    fclose(cabecalho);

    // Cria e abre arquivos de indice primario
    gera_ind_prim(lista1, "indicelista1.ind");
    gera_ind_prim(lista2, "indicelista2.ind");
    FILE* ind_prim1 = fopen("arquivos_principais/indicelista1.ind", "r+");
    FILE* ind_prim2 = fopen("arquivos_principais/indicelista2.ind", "r+");    

    // Cria arquivos de indice secundario
    gera_ind_sec(lista1, ind_prim1, "lista1.ind");
    gera_ind_sec(lista2, ind_prim2, "lista2.ind");

    fclose(ind_prim1);
    fclose(ind_prim2);

    // Ordena arquivos de indice (primario e secundario)
    ordena_arq("indicelista1.ind");
    ordena_arq("indicelista2.ind");
    ordena_arq("ind_sec_lista1.ind");
    ordena_arq("ind_sec_lista2.ind");

    fclose(lista1);
    fclose(lista2);
    
    // Cria arquivo de para intercalacao dos arquivos base 1 e 2
    FILE *arq_interc = intercala("interc.txt");
    fclose(arq_interc);

    return;
}


/*
	Funcao menu:

		Responsavel por mostrar uma indicacao visual ao usuario sobre
	o que se pode fazer no programa.
		Garante que o input dado esta correto.
*/
int menu() {
	int opcao = -1;
	
	do {
		printf("\n\t -ACESSO AO BANCO DE DADOS-\n\n");
        printf("O que deseja fazer?\n\n");
        printf("1. Pesquisar matricula\n2. Pesquisar curso\n3. Pesquisar nome\n");
        printf("4. Mostrar base de dados completa\n5. Inserir registro novo\n");
        printf("6. Sair\n\n");
        scanf("%d", &opcao);

        if (opcao < 1 || opcao > 6) {
            printf("\nEntrada invalida!\n\n");
        }
        if (opcao == 5) {
 			insere_reg(0, 0, 0);
 			atualiza_arquivos();
 			printf("\nRegistro inserido com sucesso!\n\n");      
        }
    } while (opcao < 1 || opcao > 6);

    return opcao;
}

void modifica(lista_NRR *pRetorno) {

	lista_NRR *aux;	
	aux = pRetorno;
	int opcao;
	do {
		printf("\n\t -ACESSO AO BANCO DE DADOS-\n\n");
        printf("Selecione o que deseja fazer:\n\n");
        printf("1. Remover registro\n2. Atualizar registro\n3. Voltar\n\n");
        scanf("%d", &opcao);

        if (opcao < 1 || opcao > 3) {
            printf("\nEntrada invalida!\n\n");
        }

    } while (opcao != 1 && opcao != 2 && opcao != 3);

    if (opcao == 1) {
    	printf("Selecione o registro a ser removido (Numero de indice):\n\n");
    	scanf("%d", &opcao);
    	
    	for (int i = 0; i < opcao-1; i++) {
    		if (aux == NULL) {
    			printf("\nEntrada invalida\n");
    			return;
    		}
    		aux = aux->prox;
    	}

    	remove_reg(aux->tipo, aux->NRR);
    	atualiza_arquivos();
    	printf("Registro removido com sucesso!\n");
    }
    if (opcao == 2) {
    	printf("Selecione o registro a ser atualizado (Numero de indice):\n\n");
    	scanf("%d", &opcao);
    	
    	for (int i = 0; i < opcao-1; i++) {
    		if (aux == NULL) {
    			printf("\nEntrada invalida\n");
    			return;
    		}
    		aux = aux->prox;
    	}

    	insere_reg(1, aux->tipo, aux->NRR);
    	atualiza_arquivos();
    	printf("Registro atualizado com sucesso!\n\n");
    }

	return;
}
