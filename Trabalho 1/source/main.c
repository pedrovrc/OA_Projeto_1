#include <stdio.h>
#include "indice_primario.h"

int main() {
    FILE* lista1 = fopen("arquivos_principais/lista1.txt", "r+");
    FILE* lista2 = fopen("arquivos_principais/lista2.txt", "r+");

    gera_indice_primario(lista1, "indicelista1.ind");
    gera_indice_primario(lista2, "indicelista2.ind");

    FILE* ind_prim1 = fopen("arquivos_principais/indicelista1.ind", "r+");
    FILE* ind_prim2 = fopen("arquivos_principais/indicelista2.ind", "r+");    

    gera_indice_secundario(lista1, ind_prim1, "lista1.ind");
    gera_indice_secundario(lista2, ind_prim2, "lista2.ind");


    return 0;
}
