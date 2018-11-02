#include <stdio.h>


typedef struct string {
    char string[40];
    struct string *prox;
    struct string *ant;
} cell_string;

int ordena_arq(char* nome_gerado);

cell_string *le_strings(FILE* arquivo);

void ordena(cell_string *inicio, FILE* arquivo);

int compara_string(char* string1, char* string2);
