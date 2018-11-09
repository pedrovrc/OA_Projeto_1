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
#include <string.h>
#include <stdlib.h>
#include "pesquisa.h"

#define BUFFER 100          // Tamanho de buffer arbitrario
#define POS_QTD_LINHA 49    // Posicao do numero de linhas no cabecalho
#define TAM_REG 65          // Tamanho do registro em arq_base
#define TAM_CHAVE 30        // Tamanho da chave primaria do arquivo de indices


/*
	Funcao pesquisa:

		Responsavel por receber a opcao de pesquisa do usuario e chamar
	a funcao de pesquisa correspondente.
		Retorna lista de NRRs para permitir alteracoes como remocao ou
	atualizacao de registro apos uma pesquisa bem sucedida, e insercao
	para pesquisas sem sucesso.
*/
lista_NRR *pesquisa(int opcao) {

    lista_NRR *retorno;

    switch (opcao) {
        case 1:
            retorno = pesquisa_matricula();
            break;
        case 2:
            retorno = pesquisa_curso();
            break;
        case 3:
            retorno = pesquisa_nome();
            break;
    }
    return retorno;
}


/*
	Funcao pesquisa_matricula:

		Responsavel por pegar a chave de busca do usuario (matricula), buscar
	chave em estruturas de dados que contem as chaves de indice primario e
	mostrar na tela os registros encontrados na busca (1 reg por arquivo).
*/
lista_NRR *pesquisa_matricula() {
    
    FILE *arq_base1 = fopen("arquivos_principais/lista1.txt", "r");
    FILE *arq_base2 = fopen("arquivos_principais/lista2.txt", "r");
    FILE *arq_ind_prim1 = fopen("arquivos_principais/indicelista1.ind", "r");
    FILE *arq_ind_prim2 = fopen("arquivos_principais/indicelista2.ind", "r");
    FILE *arq_cabec = fopen("arquivos_principais/cabecalho.txt", "r");

    int matricula, tam_ind1, tam_ind2, pos_array = -1;
    char reg1[TAM_REG], reg2[TAM_REG];
    int conta_print = 0, flag = 0;
    lista_NRR *inicio_lista = cria_lista();

    // Pega chave de busca do usuario
    printf("Entre com a matricula: (6 digitos)\n");
    scanf("%d", &matricula);
    printf("\n");

    // Checa se existe base de dados e recupera tamanho dos arquivos
    if (tam_arqs_ind(arq_cabec, &tam_ind1, &tam_ind2) == -1) {
        return inicio_lista;
    }

    // Cria arrays para busca
    int chaves_primarias1[tam_ind1];
    int chaves_primarias2[tam_ind2];
    // Inicia dados dos arrays
    le_chaves_primarias(arq_ind_prim1, tam_ind1, chaves_primarias1);
    le_chaves_primarias(arq_ind_prim2, tam_ind2, chaves_primarias2);

    // Busca binaria no array1
    pos_array = busca_binaria_int(chaves_primarias1, 0, tam_ind1, matricula);
    if (pos_array >= 0) {
        printf("   MATRIC NOME                                     OP  CURSO    TURMA\n");
    }

    // Se encontrar chave de busca na lista, mostra registro na tela
    if (acha_matricula(arq_ind_prim1, arq_base1, inicio_lista, 1, &flag, pos_array, reg1) == 0) {
        conta_print++;
        printf("%d. %s", conta_print, reg1);
    }

    // Busca binaria no array2
    pos_array = busca_binaria_int(chaves_primarias2, 0, tam_ind2, matricula);

    // Se encontrar chave de busca na lista, mostra registro na tela
    if (acha_matricula(arq_ind_prim2, arq_base2, inicio_lista, 2, &flag, pos_array, reg2) == 0) {
        if (flag == 2) {
            printf("   MATRIC NOME                                     OP  CURSO    TURMA\n");
        }
        conta_print++;
        printf("%d. %s", conta_print, reg2);
    }

    fclose(arq_base1);
    fclose(arq_base2);
    fclose(arq_ind_prim1);
    fclose(arq_ind_prim2);
    fclose(arq_cabec);

    return inicio_lista; 
}


/*
	Funcao pesquisa_nome:

		Responsavel por pegar a chave de busca (nome) e chamar as funcoes que
	buscam a chave nos arquivos base e que mostram os registros encontrados
	na tela. Ao final, conecta as duas listas de resultados de busca e retorna
	uma unica lista conectada com todos os resultados.
*/
lista_NRR *pesquisa_nome() {

    FILE *arq_base1 = fopen("arquivos_principais/lista1.txt", "r");
    FILE *arq_base2 = fopen("arquivos_principais/lista2.txt", "r");

    char nome[TAM_REG], lixo;
    int conta_print = 0;
    lista_NRR *aux;

    // Pega chave de busca do usuario
    printf("Entre com o nome: (Primeiras letras maiusculas)\n");
    fflush(stdin);
    scanf("%c", &lixo);
    scanf("%[^\n]", nome);
    printf("\n");

    // Cria listas com resultados da busca por nome
    lista_NRR *lista1 = busca_nome(nome, arq_base1, 1);
    lista_NRR *lista2 = busca_nome(nome, arq_base2, 2);

    // Mostra resultados da busca na tela
    printa_lista(arq_base1, lista1, &conta_print);
    printa_lista(arq_base2, lista2, &conta_print);

    // Conecta as duas listas uma a outra
    aux = lista1;
    while (aux->prox != NULL) {
    	aux = aux->prox;
    }
    aux->prox = lista2;

    fclose(arq_base1);
    fclose(arq_base2);

    // Retorna lista unica com todos os registros encontrados
    return lista1;
}


/*
	Funcao pesquisa_curso:

		Responsavel por pegar chave de busca do usuario (curso), chamar as
	funcoes de busca por curso para criar as listas de resultado e de mostrar
	resultados na tela de acordo com suas validades. Retorna uma unica lista
	com todos os resultados da busca.
*/
lista_NRR *pesquisa_curso() {

    FILE *arq_base1 = fopen("arquivos_principais/lista1.txt", "r");
    FILE *arq_base2 = fopen("arquivos_principais/lista2.txt", "r");
    FILE *arq_ind_prim1 = fopen("arquivos_principais/indicelista1.ind", "r");
    FILE *arq_ind_prim2 = fopen("arquivos_principais/indicelista2.ind", "r");
    FILE *arq_ind_sec1 = fopen("arquivos_principais/ind_sec_lista1.ind", "r");
    FILE *arq_ind_sec2 = fopen("arquivos_principais/ind_sec_lista2.ind", "r");
    FILE *arq_label_id1 = fopen("arquivos_principais/label_id_lista1.ind", "r");
    FILE *arq_label_id2 = fopen("arquivos_principais/label_id_lista2.ind", "r");
    FILE *arq_cabec = fopen("arquivos_principais/cabecalho.txt", "r");

    char lixo, curso[2];
    int conta_print = 0, tam_ind1, tam_ind2;
    lista_NRR *inicio_lista1;
    lista_NRR *inicio_lista2;
    lista_NRR *aux;

    // Pega chave de busca do usuario
    printf("Entre com o curso: (Letras Maiusculas, 2 letras)\n");
    scanf("%c", &lixo);
    scanf("%s", curso);
    printf("\n");

    // Checa se existe base de dados e recupera tamanho dos arquivos
    if (tam_arqs_ind(arq_cabec, &tam_ind1, &tam_ind2) == -1) {
        inicio_lista1 = cria_lista();
        return inicio_lista1;
    }

    // Cria listas com resultados da busca por curso
    inicio_lista1 = busca_curso(curso, 1, tam_ind1, arq_ind_sec1, arq_label_id1, arq_ind_prim1);
    inicio_lista2 = busca_curso(curso, 2, tam_ind2, arq_ind_sec2, arq_label_id2, arq_ind_prim2);

    // Caso somente lista2 com elementos validos
    if (inicio_lista1->NRR == -1 && inicio_lista2->NRR != -1) {

    	// Mostra somente lista2 na tela
        printa_lista(arq_base2, inicio_lista2, &conta_print);
        return inicio_lista2;

    // Caso somente lista1 com elementos validos
    } else if (inicio_lista1->NRR != -1 && inicio_lista2->NRR == -1) {

    	// Mostra somente lista1 na tela
        printa_lista(arq_base1, inicio_lista1, &conta_print);
        return inicio_lista1;

    // Caso ambas sem elementos validos
    } else if (inicio_lista1->NRR == -1 && inicio_lista2->NRR == -1) {

    	// Retorna lista vazia
        return inicio_lista1;

    // Caso ambas com elementos validos
    } else {

    	// Mostra lista1 e depois lista2 na tela
        printa_lista(arq_base1, inicio_lista1, &conta_print);
        printa_lista(arq_base2, inicio_lista2, &conta_print);

        // Conecta listas em uma unica
        aux = inicio_lista1;
        while (aux->prox != NULL) {
            aux = aux->prox;
        }
        aux->prox = inicio_lista2;

        // Retorna unica lista com todos os resultados da busca
        return inicio_lista1;
    }
}


/*
	Funcao busca_binaria_int:

		Busca binaria em array de inteiros. Retorna posicao do elemento caso
	encontre, caso contrario retorna -1.
*/
// Retorna posicao do array com a chave, se nao encontrar retorna -1
int busca_binaria_int(int *array, int esquerda, int direita, int chave) {
    int meio;

    if (direita >= esquerda) {
        meio = esquerda + (direita - esquerda)/2;

        if (chave == array[meio]) {
            return meio;

        } else if (chave < array[meio]) {
            return busca_binaria_int(array, esquerda, meio-1, chave);

        } else if (chave > array[meio]) {
            return busca_binaria_int(array, meio+1, direita, chave);
        }
    }
    return -1;
}


/*
	Funcao busca_nome:

		Responsavel por ler o arquivo base e comparar chave do usuario (nome)
	com cada registro do arquivo. A cada registro que bater com a chave,
	adiciona um elemento a lista de resultados da busca.
*/
// Retorna a NRR se encontrar, se nao encontrar, retorna -1
lista_NRR *busca_nome(char *chave, FILE *arq_base, int tipo) {

    int pula = 8, confirma = -1, NRR = -1;
    int tamanho_chave;
    char lixo[BUFFER];
    lista_NRR *cabeca = cria_lista();
    lista_NRR *ultimo = cabeca;

    tamanho_chave = strlen(chave);
    char recebe_campo[tamanho_chave + 1];

    // A cada iteracao se pula o numero de matricula do registro
    while (fgets(lixo, pula, arq_base) != NULL) {

    	// Le o nome do registro
        fgets(recebe_campo, tamanho_chave + 1, arq_base);

        // Pula para prox linha
        fgets(lixo, BUFFER, arq_base);

        // Compara se encontrou chave
        confirma = strcmp(chave, recebe_campo);
        NRR++;
        if (confirma == 0) {

        	// Se nao for primeira insercao
	        if (cabeca->NRR != -1) {
	        	// Cria nova celula e inicia os valores
	        	ultimo->prox = (lista_NRR*)malloc(sizeof(lista_NRR));
	        	ultimo->prox->NRR = NRR;
	        	ultimo->prox->prox = NULL;
	        	ultimo->prox->tipo = tipo;
	        	ultimo = ultimo->prox;

	        // Se for primeira insercao
        	} else {
        		// Somente inicia dados da celula
	        	cabeca->NRR = NRR;
	        	cabeca->prox = NULL;
	        	cabeca->tipo = tipo;
        	}
        }
    }

    rewind(arq_base);

    return cabeca;
}


/*
	Funcao busca_curso:

		Responsavel por encontrar chave de busca (curso) no arquivo de indice
	secundario e pegar a referencia para a lista invertida. Apos isso, le a
	lista invertida e acessa o arquivo de indice primario para pegar a NRR
	do registro na lista. A cada NRR pega, adiciona-se uma celula na lista de
	retorno.
*/
lista_NRR *busca_curso(char *chave, int tipo, int tam_ind, FILE *arq_ind_sec, FILE *arq_label_id, FILE *arq_ind_prim) {

    lista_NRR *cabeca;
    lista_NRR *ultimo;
    int tam_curso = 2, ref_label = -1, matricula, flag = 0, chave_lida;
    int NRR, ref_ind_prim;
    char linha[BUFFER], le_curso[tam_curso + 1];
    char chave_prim[TAM_CHAVE + 2];

    cabeca = cria_lista();
    ultimo = cabeca;

    // Cria arrays para busca
    int chaves_primarias[tam_ind];
    le_chaves_primarias(arq_ind_prim, tam_ind, chaves_primarias);

    // Le arq. ind. sec. para encontrar referencia da
    // lista invertida de acordo com chave de busca
    while (fgets(le_curso, tam_curso + 1, arq_ind_sec) != NULL) {

        if (strcmp(chave, le_curso) == 0) {

        	// Pega referencia para ler lista invertida
            fscanf(arq_ind_sec, "%d", &ref_label);
            break;
        }
        fgets(linha, BUFFER, arq_ind_sec);
    }

    // Se nao existir curso, retorna lista vazia
    if (ref_label == -1) {
        return cabeca;
    }

    // Enquanto ainda existirem elementos em lista invertida
    while (ref_label != -1) {

    	// Pula para registro da referencia na lista
        rewind(arq_ind_prim);
        for (int i = 0; i < ref_label; i++) {
            fgets(linha, BUFFER, arq_label_id);
        }

        // Armazena matricula
        fscanf(arq_label_id, "%d", &matricula);

        // Pula nome no campo
        fgets(chave_prim, TAM_CHAVE-6, arq_label_id);

        // Armazena referencia pra prox elemento da lista invertida
        fscanf(arq_label_id, "%d", &ref_label);

        // busca binaria encontra posicao da chave no array de ind_prim
        ref_ind_prim = busca_binaria_int(chaves_primarias, 0, tam_ind, matricula);

        // Pula as linhas no arq_ind_prim
        for (int i = 0; i < ref_ind_prim; i++) {
            fgets(linha, BUFFER, arq_ind_prim);
        }

        // Pula chave_prim pra chegar à NRR
        fgets(chave_prim, TAM_CHAVE + 2, arq_ind_prim);

        // Pega NRR
        fscanf(arq_ind_prim, "%d", &NRR);

        // Se for primeira insercao
        if (flag == 0) {

        	// Somente inicia celula
            cabeca->NRR = NRR;
            cabeca->tipo = tipo;
            flag = 1;

        // Se nao
        } else {

        	// Cria e inicia nova celula
            ultimo->prox = (lista_NRR*)malloc(sizeof(lista_NRR));
            ultimo = ultimo->prox;
            ultimo->NRR = NRR;
            ultimo->tipo = tipo;
            ultimo->prox = NULL;
        }
        rewind(arq_label_id);
    }

    return cabeca;
}


/*
	Funcao cria_lista:

		Inicia uma lista de resultados considerada vazia.
*/
// Retorna ponteiro para lista com celula iniciada neutra
lista_NRR* cria_lista() {

    lista_NRR *ponteiro = (lista_NRR*)malloc(sizeof(lista_NRR));
    ponteiro->prox = NULL;
    ponteiro->NRR = -1;
    ponteiro->tipo = 0;
    return ponteiro;
}


/*
	Funcao tam_arqs_ind:

		Checa arquivo de cabecalho para retornar tamanho dos arquivos de
	indice primario 1 e 2.
*/
// Retorna 0 para sucesso e -1 para base de dados inexistente
int tam_arqs_ind(FILE* arq_cabec, int *tam_ind1, int *tam_ind2) {
    char linha[BUFFER];

    // fgets: posiciona ponteiro no campo de qtd_linhas do cabecalho
    // caso null, cabecalho nao iniciado -> sem base de dados
    if (fgets(linha, POS_QTD_LINHA, arq_cabec) == NULL) {
        printf("Nao ha base de dados carregada.\n");
        return -1;
    }

    // Le numero de linhas do arq_ind1 no cabecalho
    fscanf(arq_cabec, "%d", tam_ind1);
    // Posiciona ponteiro na prox linha
    fgets(linha, BUFFER, arq_cabec);

    // fgets: posiciona ponteiro no campo de qtd_linhas do cabecalho
    // caso null, cabecalho nao iniciado -> sem base de dados
    if (fgets(linha, POS_QTD_LINHA, arq_cabec) == NULL) {
        printf("Nao ha base de dados carregada.\n");
        return -1;
    }

    // Le numero de linhas do arq_ind2 no cabecalho
    fscanf(arq_cabec, "%d", tam_ind2);

    return 0;
}


/*
	Funcao le_chaves_primarias:

		Le todas as matriculas de um arquivo de indice primario e
	armazena registros num array de inteiros.
*/
// Inicia valores do vetor de chaves primarias (matriculas)
void le_chaves_primarias(FILE* arq_ind_prim, int tam_ind, int* chaves_primarias) {

    char linha[BUFFER];
    int chave_lida;

    // Le e preenche arrays com chaves primarias
    for (int i = 0; i < tam_ind; i++) {
        // Le valor da chave
        fscanf(arq_ind_prim, "%d", &chave_lida);
        // Salva no array
        chaves_primarias[i] = chave_lida;
        // Pula pra prox linha
        fgets(linha, BUFFER, arq_ind_prim);
    }
    return;
}


/*
	Funcao acha_matricula:

		Recupera o registro do arquivo base com base em uma matricula forneida
	e modifica lista de retorno de acordo com a flag fornecida.
*/
// Retorna 0 para sucesso e 1 para fracasso 
int acha_matricula(FILE* arq_ind_prim, FILE* arq_base, lista_NRR* inicio, int tipo, int* flag, int pos_array, char* reg) {
    int NRR;
    char linha[BUFFER];

    // Se pos_array for valido
    if (pos_array >= 0) {
        rewind(arq_ind_prim);
        // Le arq_ind ate achar registro (usa pos_array pra isso)
        for (int i = 0; i < pos_array; i++) {
            fgets(linha, BUFFER, arq_ind_prim);
        }

        // Pega NRR dentro de arq_ind
        fgets(linha, TAM_CHAVE+2, arq_ind_prim);
        fscanf(arq_ind_prim, "%d", &NRR);
        
        // Encadeia registro na lista de acordo com a flag
        if (tipo == 1) {
            inicio->NRR = NRR;
            *flag = 1;
        } else if (tipo == 2 && *flag == 1) {
            inicio->prox = (lista_NRR*)malloc(sizeof(lista_NRR));
            inicio->prox->NRR = NRR;
            inicio->prox->prox = NULL;
        } else {
            inicio->NRR = NRR;
            *flag = 2;
        }
        inicio->tipo = tipo;
        
        // Le arq_base (usa NRR) e salva registro em string
        for (int i = 0; i < NRR; i++) {
            fgets(linha, BUFFER, arq_base);
        }
        fgets(reg, TAM_REG, arq_base);
        return 0;
    } else {
        return 1;
    }
}

/*
	Funcao printa_lista:

		Mostra na tela todos os registros contidos numa lista de resultados.
*/
void printa_lista(FILE* arq_base, lista_NRR* inicio_lista, int *conta) {
    lista_NRR* aux;
    char linha[BUFFER], reg[TAM_REG];

    if (inicio_lista->NRR == -1) {
    	return;
    }

    if (*conta == 0) {
        printf("   MATRIC NOME                                     OP  CURSO    TURMA\n");
    }
    aux = inicio_lista;
    while (aux != NULL) {
  		// Pula para o registro na NRR
        for (int i = 0; i < aux->NRR; i++) {
            fgets(linha, BUFFER, arq_base);
        }

        // Le registro de arq_base
        fgets(reg, TAM_REG, arq_base);
        *conta = *conta + 1;

        // Mostra registro de arq_base na tela
        printf("%d. %s", *conta, reg);
        aux = aux->prox;
        rewind(arq_base);
    }
    return;
}
