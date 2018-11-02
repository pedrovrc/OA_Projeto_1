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
