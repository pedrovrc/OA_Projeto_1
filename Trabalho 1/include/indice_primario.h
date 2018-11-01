#include <stdio.h>

typedef struct Inicia_chave{
	char chave[30];
	struct Inicia_chave *prox;
	int NRR;
} inicia_chave;


typedef struct Inicia_curso{
	char curso[2];
	struct Inicia_curso *prox;
	inicia_chave *primeiro;
} inicia_curso;


int gera_indice_primario(FILE* arq_base, char* nome_gerado);

int gera_indice_secundario(FILE* arq_base, FILE* ind_prim, char* nome_gerado);

void escreve_arq_secundario(inicia_curso *lista_comeco, char *nome_gerado);
