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
int busca_sequencial(char *chave, FILE *arq_base);
int busca_binaria_int(int *array, int esquerda, int direita, int chave);
