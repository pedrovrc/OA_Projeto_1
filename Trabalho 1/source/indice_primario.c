#include "indice_primario.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void gera_indice(FILE* referencia, char* nome_arquivo, char* nome_gerado) {
    char linha_atual[70];
    char string[30];
    char caminho_arquivo[40] = "arquivos_principais/";
    const int buffer = 70;
    int tamanho;
    int cont;
    int endereco_registro = 0;

    strcat(caminho_arquivo, nome_gerado);
    FILE* indices = fopen(caminho_arquivo, "w");
    
    while (fgets(linha_atual, buffer, referencia) != NULL) {
        for (int i = 0; i < 30; i++) {
            string[i] = ' ';
        }
        endereco_registro++;        
        cont = 0;
        for (int i = 0; i < 48 && cont < 30; i++) {
            if (isspace(linha_atual[i])) {
                continue;
            }
            string[cont] = linha_atual[i];
            cont++;
        }
        fprintf(indices, "%s %s", string, nome_arquivo);
        fprintf(indices, " %d\n", endereco_registro);
    }

    return;
}
