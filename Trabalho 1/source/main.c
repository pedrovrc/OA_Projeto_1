#include <stdio.h>
#include "indice.h"
#include "ordenacao.h"
#include "pesquisa.h"

#define TAM_REG 65

void atualiza_arquivos();
int menu();

int main() {

    int opcao = -1;
    char linha[TAM_REG];

    atualiza_arquivos();
		
	do {
		opcao = menu();

		if (opcao != 4 && opcao != 5) {
		    lista_NRR *pRetorno= pesquisa(opcao);

		    if (pRetorno->NRR == -1) {
		        printf("Registro nao encontrado!\n");
		        //possível inserção
		    } else {
		        //atualizar ou excluir
		    }
		}

		if (opcao == 4) {
			FILE *arq_interc = fopen("arquivos_principais/interc.txt", "r");
    		printf("MATRIC NOME                                     OP  CURSO    TURMA\n");
    		while (fgets(linha, TAM_REG, arq_interc) != NULL) {
    			printf("%s", linha);
    		}
    		fclose(arq_interc);
		}

	} while (opcao != 5);

    return 0;
}

void atualiza_arquivos() {
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
    
    FILE *arq_interc = intercalacao("interc.txt");
    fclose(arq_interc);

    return;
}

int menu() {
	int opcao = -1;
	
	do {
        printf("\nO que deseja fazer?\n\n");
        printf("1. Pesquisar matricula\n2. Pesquisar curso\n3. Pesquisar nome\n");
        printf("4. Printar base de dados\n5. Sair\n\n");
        scanf("%d", &opcao);
        if (opcao != 1 && opcao != 2 && opcao != 3 && opcao != 4 && opcao != 5) {
            printf("Entrada invalida!\n\n");
        }
    } while (opcao != 1 && opcao != 2 && opcao != 3 && opcao != 4 && opcao != 5);

    return opcao;
}
