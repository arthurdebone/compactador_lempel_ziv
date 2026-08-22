///FAESA
///
///SISTEMAS DE COMUNICA��O 2
///ARTHUR CARLOS CALVI DE BONE
///
///CODIFICA��O LEMPEL-ZIV

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 256
#define TAM_STRING 256

///********************ESTRUTURAS********************

typedef struct noDicionario  ///estrutura para o dicionario de simbolos
{
    char simbolo[TAM_STRING];///simbolo do dicionario
    int indice;
    struct noDicionario *prox;///ponteiro para proximo simbolo
}NO;

///********************PROTOTIPO FUN��ES********************

NO * criaSimbolo();
int existeDicionario (NO * dicionario, char *simbolo);
NO* inserirNoDicionario(NO *dicionario, const char *simbolo, int *proximoIndice);
NO* destroiDicionario (NO *dicionario);
void compactar(char* nomeArquivo);
void imprimirCodigoCompresao(char* codigoComrpimido, char* nomeArquivo);
char* buscaSimboloNoDicionario (NO *dicionario, char *codigo);
int existeDicionarioCodigo (NO * dicionario, char *codigo);
void descompactar(char* nomeArquivo);
void imprimirDicionario (NO * dicionario);

///********************MAIN********************
int main()
{
    int op=0;///OP (Opera��o do MENU).
    char arq[MAX];///arq (nome do arquivo a ser compactado)
    do///MENU
    {
        printf ("\n");
        printf ("1- COMPACTAR ARQUIVO\n");
        printf ("2- DESCOMPACTAR ARQUIVO\n");
        printf ("3- Sair\n");
        printf ("Opcao: ");
        scanf ("%d", &op);
        printf ("\n");
        if (op <= 0 || op >3)///Se numero de opera��o n�o contem no menu, retorna erro.
        {
            printf("Erro!! Numeracao nao consta no menu.\n\n");
            op = -1;
        }
        else
        switch(op)
        {
            case 1:
                printf ("\nDigite o nome do arquivo a ser compactado: ");
                scanf ("%s", arq);
                compactar(arq);
                break;
            case 2:
                printf ("\nDigite o nome do arquivo a ser descompactado: ");
                scanf ("%s", arq);
                descompactar(arq);
                break;
        }
    }while (op != 3);
    return 0;
}

///********************FUN��ES********************

/// A FUN��O criaSimbolo() RETORNA UMA C�LULA ALOCADA NA MEM�RIA COM PONTEIRO PARA NULL;
NO * criaSimbolo()
{
    NO *no = (NO *)malloc(sizeof(NO));///ALOCA UM N�
    if (no) {
        no->simbolo[0] = '\0';
        no->indice = 0;
        no->prox = NULL;
    }
    return no;
}


NO * destroiDicionario (NO *dicionario)///Destroi uma lista
{
    if (dicionario) {
        destroiDicionario(dicionario->prox);
        free(dicionario);
    }
    return NULL;
}

int buscarIndice(NO *dicionario, const char *simbolo) {
    NO *aux = dicionario;
    while (aux != NULL) {
        if (strcmp(aux->simbolo, simbolo) == 0)
            return aux->indice;
        aux = aux->prox;
    }
    return -1;
}

char* buscarSimbolo(NO *dicionario, int indice) {
    NO *aux = dicionario;
    while (aux != NULL) {
        if (aux->indice == indice)
            return aux->simbolo;
        aux = aux->prox;
    }
    return NULL;
}


///RECEBE O DICIONARIO E O S�MBOLO, SE O SIMBOLO EST� NO DICION�RIO RETONA 1, SE N�O ESTIER RETORNA 0
int existeDicionario (NO * dicionario, char *simbolo)///Verifica se um s�mbolo est� no dicionario
{
    NO *aux = dicionario; ///Auxiliar para percorrer o dicionario
    int esta=0; ///variavel que informara se est� no dicionario
    while (aux!=NULL && !esta) ///Percorre a lista at� chegar ao final dela ou encontrar o simbolo
    {
        if(strcmp(aux->simbolo, simbolo) == 0)
            esta = 1;
        aux = aux->prox;
    }
    return esta;
}

///RECEBE O DICIONARIO E O S�MBOLO E INSERE O SIMBOLO NO DICION�RIO E CALCULA O SEU C�DIGO.
NO* inserirNoDicionario(NO *dicionario, const char *simbolo, int *proximoIndice) {
    NO *novo = criaSimbolo();
    strcpy(novo->simbolo, simbolo);
    novo->indice = *proximoIndice;
    (*proximoIndice)++;

    if (dicionario == NULL) {
        return novo;
    }

    NO *aux = dicionario;
    while (aux->prox != NULL) {
        aux = aux->prox;
    }
    aux->prox = novo;
    return dicionario;
}

/// A FUN��O compactar(char* nomeArquivo) RECEBER� UM ARQUIVO A SER IDO E COMPACTADO E AO FINAL IR� SALVAR O ARQUIVO COMPRIMIDO
void compactar(char* nomeArquivo)
{   
    ///ABERTURA DO ARQUIVO
    FILE *entrada = fopen(nomeArquivo, "rb");///ABRE UM ARQUIVO BIN�RIO PARA LEITURA.
    if(!entrada){///se o nome do arquivo n�o existe, retorna erro
        printf("\nErro ao abrir o arquivo para compactação!\n");
        return;
    }

    ///ABERTURA ARQUIVO PARA ESCRITA
    char nomeSaida[MAX]; ///Pr�ximas linhas servem para gerar o nome do arquivo com final .lz
    snprintf(nomeSaida, sizeof(nomeSaida), "%s.lz", nomeArquivo);
    FILE *saida = fopen(nomeSaida, "wb");
    if (!saida) {
        printf("\nErro ao criar o arquivo compactado!\n");
        fclose(entrada);
        return;
    }

    NO *dicionario = NULL;
    int proximoIndice = 1;

    // Adiciona entrada vazia
    dicionario = inserirNoDicionario(dicionario, "", &proximoIndice);

    char w[TAM_STRING] = "";
    int c;

    while((c = fgetc(entrada)) != EOF)///GARANTE A LEITURA DO ARQUIVO AT� O FINAL
    {
        /*SITUA��ES
        -O PRIMEIRO DADO N�O EXISTE NO DICION�RIO
            - INCLUI O S�MBOLO NO DICION�IO COM UM C�DIGO
            - FAZ O C�DIGO COMPACTADO CONCATENANDO CODIGO DE VAZIO COM O C�DIGO DO DADO.
            - ATUALIZA O TAMANHO DO C�DIGO DO DICION�RIO
        - O PRIMEIRO DADO EXISTE O DICION�RIO
            - N�O ACABOU O ARQUIVO
                - IR PEGANDO PROX DADO AT� N�O ACHAR NO DICION�RIO
                - INCLUIR O DADO NO DICION�RIO
                - FAZ O C�DIGO COMPACTADO CONCATENANDO CODIGO DO DICION�RIO COM O C�DIGO DO DADO.
                - ATUALIZA O TAMANHO DO C�DIGO DO DICION�RIO
            - ACABOU O ARQUIVO
                - SE AUX N�O EST� NO DICION�RIO, INCLUIR E FAZER IGUAL SITUA��O 1
                - SE AUX EST� NO DICION�RIO, PEGAR C�DIGO NO DICION�RIO
                - ATUALIZA O TAMANHO DO C�DIGO DO DICION�RIO
        */

        char wc[TAM_STRING];
        snprintf(wc, sizeof(wc), "%s%c", w, (char)c);

        if (buscarIndice(dicionario, wc) != -1) {
            strcpy(w, wc);
        } else {
            int idx = buscarIndice(dicionario, w);
            fwrite(&idx, sizeof(int), 1, saida);
            unsigned char charOut = (unsigned char)c;
            fwrite(&charOut, sizeof(unsigned char), 1, saida);

            dicionario = inserirNoDicionario(dicionario, wc, &proximoIndice);
            w[0] = '\0';
        }
    }
    if (strlen(w) > 0) {
        int idx = buscarIndice(dicionario, w);
        fwrite(&idx, sizeof(int), 1, saida);
        unsigned char charOut = 0;
        fwrite(&charOut, sizeof(unsigned char), 1, saida);
    }

    fclose(entrada);
    fclose(saida);
    destroiDicionario(dicionario);
    printf("\nArquivo compactado com sucesso: %s\n", nomeSaida);
}

//****************DESCOMPACTAR**************./lempel_ziv

/// A FUN��O descompactar(char* nomeArquivo) RECEBER� UM ARQUIVO A SER LIDO E DESCOMPACTADO E AO FINAL IR� SALVAR O ARQUIVO DESCOMPRIMIDO
void descompactar(char* nomeArquivo)
{
    ///ABERTURA DO ARQUIVO
    FILE *entrada = fopen(nomeArquivo, "rb");///ABRE UM ARQUIVO BIN�RIO PARA LEITURA.
    if (!entrada) {///se o nome do arquivo n�o existe, retorna erro
        printf("\nErro ao abrir o arquivo compactado!\n");
        return;
    }

    char nomeSaida[MAX];
    strncpy(nomeSaida, nomeArquivo, sizeof(nomeSaida));
    int tam = strlen(nomeSaida);
    if (tam > 3 && strcmp(&nomeSaida[tam - 3], ".lz") == 0) {
        nomeSaida[tam - 3] = '\0';
    } else {
        strcat(nomeSaida, ".out");
    }

    ///ABERTURA ARQUIVO PARA ESCRITA
    FILE *saida = fopen(nomeSaida, "wb");
    if (!saida) {
        printf("\nErro ao criar o arquivo descompactado!\n");
        fclose(entrada);
        return;
    }

    NO *dicionario = NULL;
    int proximoIndice = 1;

    dicionario = inserirNoDicionario(dicionario, "", &proximoIndice);

    int idx;
    unsigned char c;

    while(fread(&idx, sizeof(int), 1, entrada) == 1 &&
           fread(&c, sizeof(unsigned char), 1, entrada) == 1)///GARANTE A LEITURA DO ARQUIVO AT� O FINAL
    {
        /*SITUA��ES
        -O PRIMEIRO DADO N�O EXISTE NO DICION�RIO
            - INCLUI O S�MBOLO NO DICION�IO COM UM C�DIGO
            - FAZ O C�DIGO COMPACTADO CONCATENANDO CODIGO DE VAZIO COM O C�DIGO DO DADO.
            - ATUALIZA O TAMANHO DO C�DIGO DO DICION�RIO
        - O PRIMEIRO DADO EXISTE O DICION�RIO
            - N�O ACABOU O ARQUIVO
                - IR PEGANDO PROX DADO AT� N�O ACHAR NO DICION�RIO
                - INCLUIR O DADO NO DICION�RIO
                - FAZ O C�DIGO COMPACTADO CONCATENANDO CODIGO DO DICION�RIO COM O C�DIGO DO DADO.
                - ATUALIZA O TAMANHO DO C�DIGO DO DICION�RIO
            - ACABOU O ARQUIVO
                - SE AUX N�O EST� NO DICION�RIO, INCLUIR E FAZER IGUAL SITUA��O 1
                - SE AUX EST� NO DICION�RIO, PEGAR C�DIGO NO DICION�RIO
                - ATUALIZA O TAMANHO DO C�DIGO DO DICION�RIO
        */
        char *prefixo = buscarSimbolo(dicionario, idx);
        if (!prefixo) prefixo = "";

        char entradaNova[TAM_STRING];
        if (c != 0) {
            snprintf(entradaNova, sizeof(entradaNova), "%s%c", prefixo, c);
            fputs(entradaNova, saida);
            dicionario = inserirNoDicionario(dicionario, entradaNova, &proximoIndice);
        } else {
            fputs(prefixo, saida);
        }
    }
    //imprimirDicionario (dicionario);

    fclose(entrada);
    fclose(saida);
    destroiDicionario(dicionario); ///DESALOCAR DICION�RIO
    printf("\nArquivo descompactado com sucesso: %s\n", nomeSaida);
    
}

