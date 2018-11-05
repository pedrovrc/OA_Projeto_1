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
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "indice.h"
#include "manutencao.h"
#include "pesquisa.h"

#define TAM_REG 65			// Tamanho do registro do arquivo base
#define TAM_CHAVE 30		// Tamanho da chave primaria do arquivo de indices
#define TAM_MATR_NOME 48	// Tamanho do campo matricula + nome no arq. base
#define TAM_DIR 80			// Tamanho da string para nome do diretorio
#define POS_CURSO 52		// Posicao do campo de curso no arquivo base
#define TAM_PULO 10			// Tamanho de string pra ajustar ponteiro de arq.
#define BUFFER 100			// Tamanho de buffer arbitrario


/*
	Funcao gera_ind_prim:

		Responsavel por criar arquivo para indices primarios e preencher
	seus dados com base em um arquivo de base de dados.
		O arquivo gerado possui um campo de chave primaria e outro de
	numeros que correspondem a NRR do arquivo base.
*/
int gera_ind_prim(FILE* arq_base, char* nome_gerado) {

    char linha_atual[TAM_REG];
    char chave_prim[TAM_CHAVE];
    char caminho_arquivo[TAM_DIR] = "arquivos_principais/";
    int conta_char;
    int endereco_registro = 0;

    // Se arquivo base nao existe, retorna sem criar nada
    if (arq_base == NULL) {
        return 1;
    }

    // Gera arquivo para indices primarios
    strcat(caminho_arquivo, nome_gerado);
    FILE* indices = fopen(caminho_arquivo, "w");

    // Se nao foi possivel criar o arquivo, retorna
    if (indices == NULL) {
        return 2;
    }

    // A cada iteracao, le-se uma linha nova de arq_base
    while (fgets(linha_atual, TAM_REG, arq_base) != NULL) {
        
        // Enche string com espacos para evitar sobras quando sobrescrever
        for (int i = 0; i < TAM_CHAVE; i++) {
            chave_prim[i] = ' ';
        }

        conta_char = 0;

        // Le campo desejado salvo em linha_atual[]
        // ate string encher ou campo acabar
        for (int i = 0; i < TAM_MATR_NOME && conta_char < TAM_CHAVE; i++) {	
            // Ignora espacos
            if (isspace(linha_atual[i])) {
                continue;
            }
            chave_prim[conta_char] = linha_atual[i];
            conta_char++;
        }

        fprintf(indices, "%s", chave_prim); // Campos matricula e nome concat.
        fprintf(indices, " %d\n", endereco_registro); // NRR em arq_base

        endereco_registro++;
    }

    rewind(arq_base);
    atualiza_cabecalho(nome_gerado, endereco_registro);
    fclose(indices);
    return 0;
}


/*
		Explicacao da Estrutura de Dados (ED) usada nas funcoes gera_ind_sec
	e escreve_arqs_sec:
	
		Consiste de uma lista de structs do tipo inicia_curso, com uma celula
	para cada curso unico do arquivo base.
		De cada celula de curso, sai uma lista de structs do tipo inicia_chave,
	com uma celula para cada registro (aluno) pertencente aquele curso no
	arquivo base.
		Detalhes das structs no arquivo indice.h
*/


/*
	Funcao gera_ind_sec:

		Responsavel por ler os arquivos base e indice primario para
	gerar dois arquivos: um arquivo de indice secundario e outro
	de listas invertidas, chamado de label_id.
		A funcao le o arquivo base e salva os dados uma ED. O preen-
	chimento dos arquivos gerados e feito pela funcao escreve_arqs_sec.
*/
int gera_ind_sec(FILE* arq_base, FILE* ind_prim, char* nome_gerado) {

    char linha_atual[TAM_REG];
    char ajeita_ponteiro[TAM_PULO];
    char compara_curso[3];

    // Inicia lista de cursos
    inicia_curso *lista_comeco = (inicia_curso*)malloc(sizeof(inicia_curso));
    lista_comeco->prox = NULL;
    lista_comeco->primeiro = NULL;

    // Preenche primeira celula de curso
    fgets(linha_atual, TAM_REG, arq_base);
    lista_comeco->curso[0] = linha_atual[POS_CURSO];
    lista_comeco->curso[1] = linha_atual[POS_CURSO + 1];
    lista_comeco->curso[2] = '\0';

    // Declaracao de ponteiros auxiliares
    inicia_curso *aux = lista_comeco;
    inicia_curso *lista_ultimo = lista_comeco;
    inicia_curso *sentinela_curso;
    inicia_chave *chave_aux;
    inicia_chave *sentinela_chave;

    // Cria e inicia primeira celula de chave
    lista_comeco->primeiro = (inicia_chave*)malloc(sizeof(inicia_chave));
    fgets(lista_comeco->primeiro->chave, TAM_CHAVE, ind_prim);
    lista_comeco->primeiro->prox = NULL;

    // Posiciona ponteiro na prox linha
    fgets(ajeita_ponteiro, TAM_PULO, ind_prim);

    // A cada iteracao, le-se uma linha nova de arq_base
    while (fgets(linha_atual, TAM_REG, arq_base) != NULL) {

    	// Inicia string compara_curso com curso do registro lido
        compara_curso[0] = linha_atual[POS_CURSO];
        compara_curso[1] = linha_atual[POS_CURSO + 1];
        compara_curso[2] = '\0';
        aux = lista_comeco;
        
        // Procura curso existente na ED 
        while (aux != NULL && strcmp(compara_curso, aux->curso) != 0) {
            aux = aux->prox;
        }

        // Caso nao exista, cria nova celula de curso
        if (aux == NULL) {
            aux = (inicia_curso*)malloc(sizeof(inicia_curso));
            strcpy(aux->curso, compara_curso);
            aux->prox = NULL;
            aux->primeiro = NULL;
            lista_ultimo->prox = aux;
            lista_ultimo = aux;
        }

        // Cria e inicia nova celula de chave a partir do registro lido 
        chave_aux = aux->primeiro;
        aux->primeiro = (inicia_chave*)malloc(sizeof(inicia_chave));
        fgets(aux->primeiro->chave, TAM_CHAVE, ind_prim);

        // Posiciona ponteiro na prox linha
        fgets(ajeita_ponteiro, TAM_PULO, ind_prim);

        //Encadeia chave nova
        if (chave_aux != NULL) {
            aux->primeiro->prox = chave_aux;
        } else {
            aux->primeiro->prox = NULL;
        }
    }

    escreve_arqs_sec(lista_comeco, nome_gerado);

    // Desaloca ED
    while (lista_comeco != NULL) {
    	while (lista_comeco->primeiro != NULL) {
    		sentinela_chave = lista_comeco->primeiro;
    		lista_comeco->primeiro = lista_comeco->primeiro->prox;
    		free(sentinela_chave);
    	}
    	sentinela_curso = lista_comeco;
    	lista_comeco = lista_comeco->prox;
    	free(sentinela_curso);
    }
    rewind(ind_prim);
    rewind(arq_base);
    return 0;
}


/*
	Funcao escreve_arqs_sec:

		Cria arquivo de listas invertidas e inicia seus dados lendo
	a ED passada como parametro, que contem curso e registros associados.
*/
void escreve_arqs_sec(inicia_curso *lista_comeco, char *nome_gerado) {

    char prefixo_indice[TAM_DIR] = "ind_sec_";
    char prefixo_label[TAM_DIR] = "label_id_";
    strcat(prefixo_label, nome_gerado);
    strcat(prefixo_indice, nome_gerado);
    char caminho_indice[TAM_DIR] = "arquivos_principais/";
    char caminho_label[TAM_DIR] = "arquivos_principais/";
    strcat(caminho_indice, prefixo_indice);
    strcat(caminho_label, prefixo_label);

    int flag; // Valores: 0 = Primeira insercao de chave; 1 = Caso contrario
    int linha_atual_lista = 0;
    int conta_linhas_ind = 0;

    FILE *ind_sec = fopen(caminho_indice, "w");
    FILE *label_id = fopen(caminho_label, "w");

    inicia_curso *curso_aux = lista_comeco;
    inicia_chave *chave_aux;

    // Para toda celula de curso da ED
    while (curso_aux != NULL) {
        chave_aux = curso_aux->primeiro;
        flag = 0;

        // Para toda celula de chave dessa celula de curso
        while (chave_aux != NULL) {
        	//Cria lista invertida
            if (flag == 0) {
                fprintf(label_id, "%s -1\n", chave_aux->chave);
                flag = 1;
            } else {
                fprintf(label_id, "%s %d\n", chave_aux->chave, linha_atual_lista-1);
            }
            linha_atual_lista++;
            chave_aux = chave_aux->prox;
        }

        // Adiciona registro de indice secundario
        fprintf(ind_sec, "%s %d\n", curso_aux->curso, linha_atual_lista-1);
        conta_linhas_ind++;
        curso_aux = curso_aux->prox;
    }

    atualiza_cabecalho(prefixo_indice, conta_linhas_ind);
    fclose(label_id);
    fclose(ind_sec);
    return;
}


/*
	Funcao intercala:

		Le os dois arquivos de indice primario linha a linha comparando suas
	chaves entre si, para decidir qual registro inserir primeiro no arquivo
	intercalado.
		Produz o arquivo intercalado ordenado pela chave primaria.
*/
FILE *intercala(char *nome_arq) {

    int tam_ind1, tam_ind2, flag = 0, perdeu = 0;
    int chave1, chave2, NRR1, NRR2, contador1 = 0, contador2 = 0;
    char caminho[TAM_DIR] = "arquivos_principais/";
    char linha[BUFFER];
    char registro1[TAM_REG], registro2[TAM_REG];
    strcat(caminho, nome_arq);

    // Abre arquivos para leitura
    FILE *arq_interc = fopen(caminho, "w");
    FILE *arq_base1 = fopen("arquivos_principais/lista1.txt", "r");
    FILE *arq_base2 = fopen("arquivos_principais/lista2.txt", "r");
    FILE *arq_ind1 = fopen("arquivos_principais/indicelista1.ind", "r");
    FILE *arq_ind2 = fopen("arquivos_principais/indicelista2.ind", "r");
    FILE *arq_cabec = fopen("arquivos_principais/cabecalho.txt", "r");

    // Pega tamanho dos indices primarios para limites do while
    tam_arqs_ind(arq_cabec, &tam_ind1, &tam_ind2);

    // Enquanto arqs. ind. prim. nao foram completamente lidos
    while (contador1 < tam_ind1 && contador2 < tam_ind2) {
    	// Se ainda tem dados para ler e
    	// se precisar ler novamente
        if (contador1 < tam_ind1 && perdeu != 1) {
            // le arq_ind1
            fscanf(arq_ind1, "%d", &chave1);
            fgets(linha, TAM_CHAVE-4, arq_ind1);
            fscanf(arq_ind1, "%d", &NRR1);
            fgets(linha, BUFFER, arq_ind1);
        } else {
        	// Se precisou ser lido, mas nao ha mais o que ler
            if (perdeu != 1) {
            	// Flag indica para ler somente arq_ind2
                flag = 1;
            }
        }

        // Se ainda tem dados para ler e
    	// se precisar ler novamente
        if (contador2 < tam_ind2 && perdeu != 2) {
            // le arq_ind2
            fscanf(arq_ind2, "%d", &chave2);
            fgets(linha, TAM_CHAVE-4, arq_ind2);
            fscanf(arq_ind2, "%d", &NRR2);
            fgets(linha, BUFFER, arq_ind2);
        } else {
        	// Se precisou ser lido, mas nao ha mais o que ler
            if (perdeu != 2) {
            	// Flag indica para ler somente arq_ind1
                flag = 2;
            }
        }

        // Utiliza a flag para discriminar os casos
        if (flag == 0) {
            // compara 2 registros
            if (chave1 <= chave2) {
                for (int i = 0; i < NRR1; i++) {
                    fgets(linha, BUFFER, arq_base1);
                }
                fgets(registro1, TAM_REG, arq_base1);
                rewind(arq_base1);
                perdeu = 2;
            }
            if (chave1 >= chave2) {
                for (int i = 0; i < NRR2; i++) {
                    fgets(linha, BUFFER, arq_base2);
                }
                fgets(registro2, TAM_REG, arq_base2);
                rewind(arq_base2);
                perdeu = 1;
            }
            if (chave1 == chave2) {
                perdeu = 0;
            }
        } else if ( flag == 1) {
            // le e insere reg do arq_ind2
            for (int i = 0; i < NRR2; i++) {
                fgets(linha, BUFFER, arq_base2);
            }
            fgets(registro2, TAM_REG, arq_base2);
            rewind(arq_base2);
        } else {
            // le e insere reg do arq_ind1
            for (int i = 0; i < NRR1; i++) {
                fgets(linha, BUFFER, arq_base1);
            }
            fgets(registro1, TAM_REG, arq_base1);
            rewind(arq_base1);
        }

        if (perdeu == 2 || perdeu == 0) {
            fprintf(arq_interc, "%s", registro1);
            contador1++;
        }
        if (perdeu == 1 || perdeu == 0) {
            fprintf(arq_interc, "%s", registro2);
            contador2++;
        }
    }

    fclose(arq_base1);
    fclose(arq_base2);
    fclose(arq_ind1);
    fclose(arq_ind2);
    fclose(arq_cabec);

    // Retorna ponteiro pronto para uso
    rewind(arq_interc);
    return arq_interc;
}
