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
