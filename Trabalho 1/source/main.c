#include <stdio.h>
#include "indice_primario.h"

int main() {
    FILE* arquivo1 = fopen("arquivos_principais/lista1.txt", "r+");
    FILE* arquivo2 = fopen("arquivos_principais/lista2.txt", "r+");

    gera_indice(arquivo1, "lista1.txt", "indicelista1.ind");
    gera_indice(arquivo2, "lista2.txt", "indicelista2.ind");

    return 0;
}
