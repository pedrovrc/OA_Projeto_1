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

typedef struct Lista_NRR{
	int NRR;
	int tipo;
	struct Lista_NRR *prox;
} lista_NRR;

lista_NRR *pesquisa(int opcao);
lista_NRR *pesquisa_matricula();
lista_NRR *pesquisa_nome();
lista_NRR *pesquisa_curso();

lista_NRR *busca_curso(char *chave, int tipo, int tam_ind, FILE *arq_ind_sec, FILE *arq_label_id, FILE *arq_ind_prim);
lista_NRR *busca_nome(char *chave, FILE *arq_base, int tipo);
int busca_binaria_int(int *array, int esquerda, int direita, int chave);

lista_NRR* cria_lista();
int tam_arqs_ind(FILE* arq_cabec, int *tam_ind1, int *tam_ind2);
void le_chaves_primarias(FILE* arq_ind_prim, int tam_ind, int* chaves_primarias);
int acha_matricula(FILE* arq_ind_prim, FILE* arq_base, lista_NRR* inicio, int tipo, int* flag, int pos_array, char* reg);
void printa_lista(FILE* arq_base, lista_NRR* inicio_lista, int *conta);
