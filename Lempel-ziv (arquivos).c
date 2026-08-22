///FAESA
///
///SISTEMAS DE COMUNICAÇÃO 2
///ARTHUR CARLOS CALVI DE BONE
///
///CODIFICAÇÃO LEMPEL-ZIV

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 50
#define TAM_STRING 50

///********************ESTRUTURAS********************

typedef struct noDicionario  ///estrutura para o dicionario de simbolos
{
    char simbolo[TAM_STRING], codigo[TAM_STRING];///simbolo do dicionario
    struct noDicionario *prox;///ponteiro para proximo simbolo
}NO;

///********************PROTOTIPO FUNÇÕES********************

NO * criaSimbolo();
int existeDicionario (NO * dicionario, char *simbolo);
NO* inserirNoDicionario(NO *dicionario, char *simbolo);
char* converterBinario(int decimal);
NO* destroiDicionario (NO *dicionario);
void inverteString(char *string);
void compactar(char* nomeArquivo);
NO* igualaCasasBinarias (NO *dicionario);
void imprimirCodigoCompresao(char* codigoComrpimido, char* nomeArquivo);
char* buscaSimboloNoDicionario (NO *dicionario, char *codigo);
int existeDicionarioCodigo (NO * dicionario, char *codigo);
NO* igualaCasasBinariasCodigo (NO *dicionario, int **bit);
void descompactar(char* nomeArquivo);
void imprimirDicionario (NO * dicionario);
void criaByte(FILE *saida, char* comprimido, char * byte, int fimArquivo);
void completaZeros (char* byte);

///********************MAIN********************
int main()
{
    int op=0;///OP (Operação do MENU).
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
        if (op <= 0 || op >3)///Se numero de operação não contem no menu, retorna erro.
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

///********************FUNÇÕES********************

/// A FUNÇÃO criaSimbolo() RETORNA UMA CÉLULA ALOCADA NA MEMÓRIA COM PONTEIRO PARA NULL;
NO * criaSimbolo()
{
    NO *no = (NO *)malloc(sizeof(NO));///ALOCA UM NÓ
    no->prox = NULL;
    return no;
}

///PASSA PELO DICIONARIO E IGUALA O TAMANHO DE BITS DOS SIMBOLOS COM O SIMBOLO DE MAIOR CODIGO BINARIO
NO* igualaCasasBinarias (NO *dicionario)
{
    NO *aux = dicionario;///aux aponta pro inicio da lista
    int tam, tamAux, i, j;///tam(qtd de bits do maior codigo no momento), tamAux(qtd de bits do codigo de aux)
    char auxCod[TAM_STRING];

    while(aux->prox != NULL)///Percorre até o final do dicionário
        aux = aux->prox;
    tam = strlen(aux->codigo);///tamanho de quantos bits tem o maior simbolo
    aux = dicionario;///aux aponta para o inicio do dicionario
    while (aux != NULL)///Iguala os simbolos do dicinario com a qtd binaria do maior simbolo no momento
    {
        strcpy(auxCod,aux->codigo);
        if (strlen(auxCod) == tam)///se o cógido já tem a qtd de bits do maior, encerra.
            break;
        tamAux = strlen(auxCod);///Verifica a qtd de bits do codigo de aux
        for(i=tamAux;i>=1;i--)
            auxCod[i] = auxCod[i-1];
        auxCod[tamAux+1] = '\0';
        auxCod[0] = '0';
        strcpy(aux->codigo,auxCod);
        aux = aux->prox;
    }
    return dicionario;
}

NO * destroiDicionario (NO *dicionario)///Destroi uma lista
{
    if (dicionario)///se a lista tem elementos
    {
        dicionario->prox = destroiDicionario(dicionario->prox);///chama a função para a proxima celula da lista
        free(dicionario);///desaloca
    }
    dicionario = NULL;
    return dicionario;
}

char* buscaCodigoNoDicionario (NO *dicionario, char *simbolo)///Busca no dicionario o código referente ao símbolo passado
{
    NO *aux = dicionario->prox;

    while(aux != NULL)
    {
        if (strcmp(aux->simbolo,simbolo) == 0)
            return aux->codigo;
        aux = aux->prox;
    }
    return NULL;
}

///RECEBE O DICIONARIO E O SÍMBOLO, SE O SIMBOLO ESTÁ NO DICIONÁRIO RETONA 1, SE NÃO ESTIER RETORNA 0
int existeDicionario (NO * dicionario, char *simbolo)///Verifica se um símbolo está no dicionario
{
    NO *aux = dicionario; ///Auxiliar para percorrer o dicionario
    int esta=0; ///variavel que informara se está no dicionario
    while (aux!=NULL && !esta) ///Percorre a lista até chegar ao final dela ou encontrar o simbolo
    {
        if(strcmp(aux->simbolo, simbolo) == 0)
            esta = 1;
        aux = aux->prox;
    }
    return esta;
}

///RECEBE O DICIONARIO E O SÍMBOLO E INSERE O SIMBOLO NO DICIONÁRIO E CALCULA O SEU CÓDIGO.
NO* inserirNoDicionario(NO *dicionario, char *simbolo)///Inserir novo símbolo no dicionário
{
    NO *aux, *novo;///aux (para alocar no dicionário), aux2 (percorre a lista)
    novo = criaSimbolo();
    aux = dicionario;
    int cont=1;///contador para saber a posição no dicionario e assim gerar o seu código referente ao simbolo inserido.

    strcpy(novo->simbolo, simbolo);///Copia o símbolo para o nó que será inserido no dicionário
    while(aux->prox != NULL)///leva aux2 até o fim da lista
    {
        aux = aux->prox;
        cont++;
    }
    strcpy(novo->codigo,converterBinario(cont));///Coloca o codigo na celula aux
    aux->prox = novo;///insere no dicionário
    return dicionario;
}

///RECEBE UM VALOR EM DECIMAL E DEVOLVE UMA STRING REPRESENTANDO EM BINÁRIO
char* converterBinario(int decimal)///Converte o numero decimal em binario
{
    char binario[TAM_STRING];
    int indice = 0, quociente, resto;

    do
    {
        quociente = decimal/2;
        resto = decimal%2;
        binario[indice] = resto+48;///48 - zero em ascii
        indice++;
        decimal = quociente;
    }while(quociente > 0);
    binario[indice] = '\0';
    inverteString(binario);
    return binario;
}

///RECEBE UMA STRING E INVERTE ELA
void inverteString(char *string)
{
   int  i, j;
   char temp[TAM_STRING];

   for(i = 0, j = strlen(string)-1; j >= 0; ++i, --j)
   {
       temp[i] = string[j];
       temp[i+1] = '0';
   }
   strcpy(string, temp);
   string[i] = '\0';
}

///IMPRIMIR DICIONARIO
void imprimirDicionario (NO * dicionario)
{
    NO *aux = dicionario;
    printf ("\nIMPRIMINDO DICIONARIO");
    printf ("\nVAZIO | %s", aux->codigo);
    aux = aux->prox;
    while (aux !=NULL)
    {
        printf ("\n%s | %s",aux->simbolo, aux->codigo);
        aux = aux->prox;
    }
}

///Função para criar uma string com 8 caracteres
void criaByte(FILE *saida, char* comprimido, char* byte, int fimArquivo)
{
    int i = strlen(byte), j=0;
    printf ("%s|%s - ", comprimido, byte);
    for (i,j;j < strlen(comprimido);i++, j++)///Passa os caracteres de comprmento para formr 1 byte
    {
        byte[i] = comprimido[j];
        if (strlen(byte) == 8)///Se a sting tiver 1 byte, imprime e zera ela
        {
            fprintf(saida,"%s", byte);
            byte[0] = '\0';
            i=0;
        }

    }
    byte[strlen(byte)] = '\0';
    ///Se o arquivo acabou e ainda tem bits a serem impressos, complementa com zeros à esquerda e imprime
    if (fimArquivo)
    {
        completaZeros(byte);
        fprintf(saida,"%s", byte);
    }

}

///Completa de zeros o byte
void completaZeros (char* byte)
{
    int i=0, j = (8-strlen(byte));
    char aux[8];
    strcpy(aux, byte);
    for (i;i<j;i++)///Coloca zeros
    {
        byte[i] = '0';
        byte[i+1] = '\0';
    }
    for (i= strlen(byte), j=0;i<8;i++, j++)///coloca a info
    {
        byte[i] = aux[j];
    }
}

/// A FUNÇÃO compactar(char* nomeArquivo) RECEBERÁ UM ARQUIVO A SER IDO E COMPACTADO E AO FINAL IRÁ SALVAR O ARQUIVO COMPRIMIDO
void compactar(char* nomeArquivo)
{
    FILE *entrada;
    NO *dicionario = NULL, *vazio;
    char simbolo[TAM_STRING], aux[TAM_STRING], comprimido[TAM_STRING], byte[8];///SIMBOLO[VAI RECEBENDO OS SIMBOLOS DO ARQUIVO], AUX[RECEBE A VARIÁVEL SIMBOLO MENOS O ULTIMO PEGO], AUX2[RECEBE O ULTIMO SIMBOLO DO ARQUIVO]
    char aux2;
    int  indice=0;

    ///COLOCANDO O SIMBOLO VAZIO NO DICIONÁRIO
    vazio = criaSimbolo();
    strcpy(vazio->codigo,"0");
    dicionario = vazio;

    ///ABERTURA DO ARQUIVO
    entrada = fopen(nomeArquivo, "rb");///ABRE UM ARQUIVO BINÁRIO PARA LEITURA.
    if (!entrada)///se o nome do arquivo não existe, retorna erro
    {
        printf ("\nO arquivo informado nao existe!\n");
        return;
    }

    ///ABERTURA ARQUIVO PARA ESCRITA
    FILE *saida;
    char nomeArquivoComprimido[MAX]; ///Próximas linhas servem para gerar o nome do arquivo com final .lz
    strcpy(nomeArquivoComprimido, nomeArquivo);
    strcat(nomeArquivoComprimido,".lz");
    saida = fopen(nomeArquivoComprimido, "wb");

    ///**************************************************
    FILE *buffer;
    buffer = fopen("lzBuffer.txt", "w");
    ///**************************************************

    simbolo[0] = fgetc(entrada);///RECEBE O PRIMEIRO DADO DO ARQUIVO
    simbolo[1] = '\0';

    while(!feof(entrada))///GARANTE A LEITURA DO ARQUIVO ATÉ O FINAL
    {
        /*SITUAÇÕES
        -O PRIMEIRO DADO NÃO EXISTE NO DICIONÁRIO
            - INCLUI O SÍMBOLO NO DICIONÁIO COM UM CÓDIGO
            - FAZ O CÓDIGO COMPACTADO CONCATENANDO CODIGO DE VAZIO COM O CÓDIGO DO DADO.
            - ATUALIZA O TAMANHO DO CÓDIGO DO DICIONÁRIO
        - O PRIMEIRO DADO EXISTE O DICIONÁRIO
            - NÃO ACABOU O ARQUIVO
                - IR PEGANDO PROX DADO ATÉ NÃO ACHAR NO DICIONÁRIO
                - INCLUIR O DADO NO DICIONÁRIO
                - FAZ O CÓDIGO COMPACTADO CONCATENANDO CODIGO DO DICIONÁRIO COM O CÓDIGO DO DADO.
                - ATUALIZA O TAMANHO DO CÓDIGO DO DICIONÁRIO
            - ACABOU O ARQUIVO
                - SE AUX NÃO ESTÁ NO DICIONÁRIO, INCLUIR E FAZER IGUAL SITUAÇÃO 1
                - SE AUX ESTÁ NO DICIONÁRIO, PEGAR CÓDIGO NO DICIONÁRIO
                - ATUALIZA O TAMANHO DO CÓDIGO DO DICIONÁRIO
        */
        if (!existeDicionario(dicionario, simbolo))///O PRIMEIRO DADO NÃO EXISTE NO DICIONÁRIO
        {
            inserirNoDicionario(dicionario, simbolo);
            ///PRÓXIMAS 2 LINHAS SERVEM PARA CONCATENAR E GERAR O CÓDIGO COMPRIMIDO
            strcpy(comprimido, dicionario->codigo);///COPIA O SIMBOLO VAZIO PARA A VARIAVEL comprimido
            strcat(comprimido,simbolo);///CONCATENA COM O SIMBOLO RECEBIDO
fprintf(buffer,"%s", comprimido);
            igualaCasasBinarias(dicionario);
        }
        else
        {
            while (existeDicionario(dicionario,simbolo) && !feof(entrada))///WHILE PARA IR PEGANDO SIMBOLO NO ARQUIVO ATÉ NÃO TER MAIS NO DICIONÁRIO OU ACABAR O ARQUIVO
            {
                strcpy(aux,simbolo);///COPIA O VALOR DE simbolo EM aux
                indice++;///Se o símbolo já está no dicionário é necessário colocar mais um caractere e verificar novamente
                simbolo[indice] = fgetc(entrada);
                simbolo[indice+1] = '\0';
                aux2 = simbolo[indice];///aux2 RECEBE O NOVO SÍMBOLO
            }
            if (feof(entrada))
            {
                if (existeDicionario(dicionario,aux))
                {
                    strcpy(comprimido, buscaCodigoNoDicionario(dicionario,aux));
fprintf(buffer,"%s", comprimido);
                }
                else///SE NÃO EXISTE NO DICIONÁRIO
                {
                    inserirNoDicionario(dicionario, simbolo);
                    ///PRÓXIMAS 2 LINHAS SERVEM PARA CONCATENAR E GERAR O CÓDIGO COMPRIMIDO
                    strcpy(comprimido, buscaCodigoNoDicionario(dicionario,aux));///COPIA O SIMBOLO VAZIO PARA A VARIAVEL comprimido
                    comprimido[strlen(comprimido)] = aux2;
                    comprimido[strlen(comprimido)+1] = '\0';
fprintf(buffer,"%s", comprimido);
                }
            }
            else///SE O ARQUIVO NÃO ACABOU (O SIMBOLO NÃO EXISTE NO DICIONÁRIO)
            {
                inserirNoDicionario(dicionario, simbolo);
                ///PRÓXIMAS 2 LINHAS SERVEM PARA CONCATENAR E GERAR O CÓDIGO COMPRIMIDO
                strcpy(comprimido, buscaCodigoNoDicionario(dicionario,aux));///COPIA O SIMBOLO VAZIO PARA A VARIAVEL comprimido
                comprimido[strlen(comprimido)] = aux2;
                comprimido[strlen(comprimido)+1] = '\0';
fprintf(buffer,"%s", comprimido);
                igualaCasasBinarias(dicionario);
            }
        }


        indice = 0;///Reseta o índice
        simbolo[0] = fgetc(entrada);
        simbolo[1] = '\0';
    }
    //imprimirDicionario (dicionario);

    fclose(entrada);
    fclose (buffer);
    FILE* ler;
    ler = fopen("lzBuffer.txt", "r");
    int cont=0;
    while(!feof(ler))
    {
        while ((strlen(byte) != 8) && !feof(ler))
        {
            byte[cont] = fgetc(ler);
            byte[cont+1] = '\0';
            cont++;
        }
        fprintf(saida,"%s", byte);
        printf ("feito");
        cont = 0;
        byte[cont]='\0';
    }

    fclose(saida);
    fclose(ler);

    ///DESALOCAR DICIONÁRIO
    dicionario = destroiDicionario(dicionario);
}

//****************DESCOMPACTAR**************

char* buscaSimboloNoDicionario (NO *dicionario, char *codigo)///Busca no dicionario o código referente ao símbolo passado
{
    NO *aux = dicionario->prox;

    while(aux != NULL)
    {
        if (strcmp(aux->codigo,codigo) == 0)
            return aux->simbolo;
        aux = aux->prox;
    }
    return NULL;
}

int existeDicionarioCodigo (NO * dicionario, char *codigo)///Verifica se um codigo está no dicionario
{
    NO *aux = dicionario; ///Auxiliar para percorrer o dicionario
    int esta=0; ///variavel que informara se está no dicionario
    while (aux!=NULL && !esta) ///Percorre a lista até chegar ao final dela ou encontrar o simbolo
    {
        if(strcmp(aux->codigo, codigo) == 0)
            esta = 1;
        aux = aux->prox;
    }
    return esta;
}

///PASSA PELO DICIONARIO E IGUALA O TAMANHO DE BITS DOS SIMBOLOS COM O SIMBOLO DE MAIOR CODIGO BINARIO
NO* igualaCasasBinariasCodigo (NO *dicionario, int **bit)
{
    NO *aux = dicionario;///aux aponta pro inicio da lista
    int tam, tamAux, i, j;///tam(qtd de bits do maior codigo no momento), tamAux(qtd de bits do codigo de aux)
    char  auxCod[TAM_STRING];

    while(aux->prox != NULL)///Percorre até o final do dicionário
        aux = aux->prox;
    tam = strlen(aux->codigo);///tamanho de quantos bits tem o maior simbolo
    *bit = tam; ///ATUALIZA A QTD DE BITS
    aux = dicionario;///aux aponta para o inicio do dicionario
    while (aux != NULL)///Iguala os simbolos do dicinario com a qtd binria do maior simbolo no momento
    {
        strcpy(auxCod,aux->codigo);
        if (strlen(auxCod) == tam)///se o cógido já tem a qtd de bits do maior, encerra.
            break;
        tamAux = strlen(auxCod);///Verifica a qtd de bits do codigo de aux
        for(i=tamAux;i>=1;i--)
            auxCod[i] = auxCod[i-1];
        auxCod[tamAux+1] = '\0';
        auxCod[0] = '0';
        strcpy(aux->codigo,auxCod);
        aux = aux->prox;
    }
    return dicionario;
}

/// A FUNÇÃO descompactar(char* nomeArquivo) RECEBERÁ UM ARQUIVO A SER LIDO E DESCOMPACTADO E AO FINAL IRÁ SALVAR O ARQUIVO DESCOMPRIMIDO
void descompactar(char* nomeArquivo)
{
    FILE *entrada;
    NO *dicionario = NULL, *vazio;
    char codigo[TAM_STRING], aux[TAM_STRING], comprimido[TAM_STRING], aux2[2];///SIMBOLO[VAI RECEBENDO OS SIMBOLOS DO ARQUIVO], AUX[RECEBE A VARIÁVEL SIMBOLO MENOS O ULTIMO PEGO], AUX2[RECEBE O ULTIMO SIMBOLO DO ARQUIVO]
    int  indice=0;
    int *bit = 1;

    ///COLOCANDO O SIMBOLO VAZIO NO DICIONÁRIO
    vazio = criaSimbolo();
    strcpy(vazio->codigo,"0");
    dicionario = vazio;

    ///ABERTURA DO ARQUIVO
    entrada = fopen(nomeArquivo, "rb");///ABRE UM ARQUIVO BINÁRIO PARA LEITURA.
    if (!entrada)///se o nome do arquivo não existe, retorna erro
    {
        printf ("\nO arquivo informado nao existe!\n");
        return;
    }

    ///ABERTURA ARQUIVO PARA ESCRITA
    FILE *saida;
    char nomeArquivoDesomprimido[MAX];
    strcpy(nomeArquivoDesomprimido, nomeArquivo);
    ///RETIRANDO O .lz DO ARQUIVO.
    int tam = strlen(nomeArquivo);
    strcpy(nomeArquivoDesomprimido,nomeArquivo);
    nomeArquivoDesomprimido[tam-3] = '\0';
    saida = fopen(nomeArquivoDesomprimido, "wb");

    codigo[0] = fgetc(entrada);///RECEBE O PRIMEIRO DADO DO ARQUIVO
    codigo[1] = '\0';
    while(!feof(entrada))///GARANTE A LEITURA DO ARQUIVO ATÉ O FINAL
    {
        /*SITUAÇÕES
        -O PRIMEIRO DADO NÃO EXISTE NO DICIONÁRIO
            - INCLUI O SÍMBOLO NO DICIONÁIO COM UM CÓDIGO
            - FAZ O CÓDIGO COMPACTADO CONCATENANDO CODIGO DE VAZIO COM O CÓDIGO DO DADO.
            - ATUALIZA O TAMANHO DO CÓDIGO DO DICIONÁRIO
        - O PRIMEIRO DADO EXISTE O DICIONÁRIO
            - NÃO ACABOU O ARQUIVO
                - IR PEGANDO PROX DADO ATÉ NÃO ACHAR NO DICIONÁRIO
                - INCLUIR O DADO NO DICIONÁRIO
                - FAZ O CÓDIGO COMPACTADO CONCATENANDO CODIGO DO DICIONÁRIO COM O CÓDIGO DO DADO.
                - ATUALIZA O TAMANHO DO CÓDIGO DO DICIONÁRIO
            - ACABOU O ARQUIVO
                - SE AUX NÃO ESTÁ NO DICIONÁRIO, INCLUIR E FAZER IGUAL SITUAÇÃO 1
                - SE AUX ESTÁ NO DICIONÁRIO, PEGAR CÓDIGO NO DICIONÁRIO
                - ATUALIZA O TAMANHO DO CÓDIGO DO DICIONÁRIO
        */
        if (strcmp(codigo,dicionario->codigo) == 0)///SE O CÓDIGO POSSUIR O SIMBOLO PARA VAZIO, DESCARTA E PEGA O PROXIMO
        {
            codigo[0] = fgetc(entrada);///RECEBE O PRIMEIRO DADO DO ARQUIVO
            codigo[1] = '\0';
        }
        if (!existeDicionarioCodigo(dicionario, codigo))///O PRIMEIRO DADO NÃO EXISTE NO DICIONÁRIO ou é o vazio
        {
            inserirNoDicionario(dicionario, codigo);
            fprintf(saida,"%s", codigo);
            igualaCasasBinariasCodigo(dicionario, &bit);
        }
        else
        {
            while (existeDicionarioCodigo(dicionario,codigo) && !feof(entrada))///WHILE PARA IR PEGANDO SIMBOLO NO ARQUIVO ATÉ NÃO TER MAIS NO DICIONÁRIO OU ACABAR O ARQUIVO
            {
                strcpy(aux,codigo);///COPIA O VALOR DE codigo EM aux
                codigo[indice] = fgetc(entrada);
                codigo[indice+1] = '\0';
                aux2[0] = codigo[indice];///aux2 RECEBE O NOVO SÍMBOLO
                aux2[1] = '\0';
                indice++;///Se o símbolo já está no dicionário é necessário colocar mais um caractere e verificar novamente
            }
            if (feof(entrada))
            {
                if (existeDicionarioCodigo(dicionario,aux))
                {
                    strcpy(comprimido, buscaSimboloNoDicionario(dicionario,aux));
                    fprintf(saida,"%s", comprimido);
                }
                else///SE NÃO EXISTE NO DICIONÁRIO
                {
                    inserirNoDicionario(dicionario, codigo);
                    ///PRÓXIMAS 2 LINHAS SERVEM PARA CONCATENAR E GERAR O CÓDIGO COMPRIMIDO
                    strcpy(comprimido, buscaSimboloNoDicionario(dicionario,aux));///COPIA O SIMBOLO VAZIO PARA A VARIAVEL comprimido
                    strcat(comprimido,aux2);
                    fprintf(saida,"%s", comprimido);
                }
            }
            else///SE O ARQUIVO NÃO ACABOU (O SIMBOLO NÃO EXISTE NO DICIONÁRIO)
            {
                ///PRÓXIMAS 2 LINHAS SERVEM PARA CONCATENAR E GERAR O CÓDIGO COMPRIMIDO
                strcpy(comprimido, buscaSimboloNoDicionario(dicionario,aux));///COPIA O SIMBOLO VAZIO PARA A VARIAVEL comprimido
                strcat(comprimido,aux2);
                inserirNoDicionario(dicionario, comprimido);
                fprintf(saida,"%s", comprimido);
                igualaCasasBinariasCodigo(dicionario, &bit);
            }
        }

        for (indice=0;indice < bit;indice++)
        {
            do
            {
                codigo[indice] = fgetc(entrada);
                codigo[indice+1] = '\0';
            }while (!feof(entrada));
        }
    }
    //imprimirDicionario (dicionario);

    fclose(entrada);
    fclose(saida);
    ///DESALOCAR DICIONÁRIO
    dicionario = destroiDicionario(dicionario);
}

