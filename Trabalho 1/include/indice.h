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

typedef struct Inicia_chave{
	char chave[30];
	struct Inicia_chave *prox;
} inicia_chave;

typedef struct Inicia_curso{
	char curso[3];
	struct Inicia_curso *prox;
	inicia_chave *primeiro;
} inicia_curso;

int gera_ind_prim(FILE* arq_base, char* nome_gerado);
int gera_ind_sec(FILE* arq_base, FILE* ind_prim, char* nome_gerado);
void escreve_arqs_sec(inicia_curso *lista_comeco, char *nome_gerado);
FILE *intercala(char *nome_arq);
