#include <stdio.h>
#include "indice.h"

int main() {

	// Criacao dos arquivos de indice primario e secundario
    FILE* lista1 = fopen("arquivos_principais/lista1.txt", "r+");
    FILE* lista2 = fopen("arquivos_principais/lista2.txt", "r+");

    gera_ind_prim(lista1, "indicelista1.ind");
    gera_ind_prim(lista2, "indicelista2.ind");

    FILE* ind_prim1 = fopen("arquivos_principais/indicelista1.ind", "r+");
    FILE* ind_prim2 = fopen("arquivos_principais/indicelista2.ind", "r+");    

    gera_ind_sec(lista1, ind_prim1, "lista1.ind");
    gera_ind_sec(lista2, ind_prim2, "lista2.ind");

    fclose(lista1);
    fclose(lista2);
    fclose(ind_prim1);
    fclose(ind_prim2);

    return 0;
}
