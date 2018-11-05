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

typedef struct string {
    char string[40];
    struct string *prox;
    struct string *ant;
} cell_string;

int ordena_arq(char* nome_gerado);
cell_string *le_strings(FILE* arquivo);
void ordena(cell_string *inicio, FILE* arquivo);
int compara_string(char* string1, char* string2);
