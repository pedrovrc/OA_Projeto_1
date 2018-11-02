#include <stdio.h>
#include "indice.h"
#include "ordenacao.h"

int main() {

    int opcao = -1;
    char string[65];
    cell_string *estrutura;
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

    do {
        printf("Deseja pesquisar de que forma?\n");
        printf("1. matricula\n2. curso\n3. nome\n\n");
        scanf("%d", opcao);
        if (opcao != 1 && opcao != 2 && opcao != 3) {
            printf("Entrada invalida!\n\n");
        }
    } while (opcao != 1 && opcao != 2 && opcao != 3);
    
    if (pesquisa(opcao, string) != 0) {
        printf("Registro nao encontrado!\n");
    } else {
        printf("%s\n", string);
    }

    return 0;
}

/*
int main() {
    FILE* strings = fopen("strings.txt", "r");
    cell_string *estrutura;

    if (strings == NULL) {
        printf("ERRO AO CARREGAR ARQUIVO\n");
        return 1;
    }

    estrutura = le_strings(strings);
    ordena(estrutura);

    fclose(strings);
    return 0;
}
*/