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

void atualiza_cabecalho(char *nome_arq, int qtd_linha);
void insere_reg(int opcao, int tipo, int NRR);
void verifica_entrada(char *item, int tam_esperado);
void remove_reg(int tipo, int NRR);
