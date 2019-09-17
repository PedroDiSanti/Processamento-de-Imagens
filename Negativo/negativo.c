#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define TRUE  1
#define FALSE 0

typedef int * imagem;

typedef struct{
    int valor1;
    int valor2;       
}dependecia;

dependecia dependencias[1000];

/*+-------------------------------------------------------------------------+
  | Rotinas para ALOCAR e DESALOCAR dinamicamente espaco de memoria para um |
  | vetor monodimensional que armazenara' a imagem.                         |
  | PARAMETROS:                                                             |
  | I  = Endereco do vetor (ponteiro de inteiros).                          |
  | nl = Numero de linhas.                                                  |
  | nc = Numero de colunas.                                                 |
  +-------------------------------------------------------------------------+*/
int aloca_memo(imagem *I, int nl, int nc) {
    *I = (int *) malloc(nl * nc * sizeof (int));
    if (*I) return TRUE;
    else return FALSE;
}

int desaloca_memo(imagem *I) {
    free(*I);
}

/*+-------------------------------------------------------------------------+
  | Apresenta informacoes sobre um arquivo de imagem.                       |
  | Parametros:                                                             |
  |   nome = nome fisico do arquivo de imagem.                              |
  |   nl   = numero de linhas da imagem.                                    |
  |   nc   = numero de colunas da imagem.                                   |
  |   mn   = maximo nivel de cinza da imagem.                               |
  +-------------------------------------------------------------------------+*/
void info_imagem(char *nome, int nl, int nc, int mn) {
    printf("\nINFORMACOES SOBRE A IMAGEM:");
    printf("\n--------------------------\n");
    printf("Nome do arquivo.............: %s \n", nome);
    printf("Numero de linhas............: %d \n", nl);
    printf("Numero de colunas...........: %d \n", nc);
    printf("Maximo nivel-de-cinza/cores.: %d \n\n", mn);
}

/*+-------------------------------------------------------------------------+
  | Rotina que faz a leitura de uma imagem em formato .PGM ASCII e armazena |
  | num vetor monodimensional. Um exemplo de imagem .PGM ASCII gravada neste|
  | formato:                                                                |
  |                                                                         |
  | P2                                                                      |
  | # CREATOR: XV Version 3.10a  Rev: 12/29/94                              |
  | 124 122                                                                 |
  | 255                                                                     |
  | 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255     |
  | 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255     |
  | (...)                                                                   |
  |                                                                         |
  | Lin 1: contem P2, o que identifica este arquivo como PGM ASCII.         |
  | Lin 2: contem um comentario qualquer iniciado com #.                    |
  | Lin 3: numero de colunas e numero de linhas da imagem.                  |
  | Lin 4: maximo nivel de cinza na imagem (255 normalmente).               |
  | Lin 5 em diante: valores de cinza da imagem.                            |
  |                                                                         |
  | PARAMETROS:                                                             |
  | nome = nome do arquivo (entra).                                         |
  | I    = ponteiro para o vetor imagem (retorna).                          |
  | nl   = numero de linhas da imagem (retorna).                            |
  | nc   = numero de colunas da imagem (retorna).                           |
  | mn   = maximo nivel de cinza (retorna).                                 |
  +-------------------------------------------------------------------------+*/
int le_imagem_pgm(char *nome, imagem *I, int *nl, int *nc, int *mn) {
    int i, j, k, LIMX, LIMY, MAX_NIVEL;
    char c, LINHA[100];
    FILE *arq;
    if ((arq = fopen(nome, "r")) == NULL) {
        printf("Erro na ABERTURA do arquivo <%s>\n\n", nome);
        return FALSE;
    }
    /*-- PGM = "P2" -----------*/
    fgets(LINHA, 80, arq);
    if (!strstr(LINHA, "P2")) {
        printf("Erro no FORMATO do arquivo <%s>\n\n", nome);
        return FALSE;
    }
    /*-- Dimensoes da imagem --*/
    fgets(LINHA, 80, arq);
    do {
        fgets(LINHA, 80, arq);
    } while (strchr(LINHA, '#'));
    sscanf(LINHA, "%d %d", &LIMX, &LIMY);
    fscanf(arq, "%d", &MAX_NIVEL);

    if (LIMX == 0 || LIMY == 0 || MAX_NIVEL == 0) {
        printf("Erro nas DIMENSOES do arquivo <%s>\n\n", nome);
        return FALSE;
    }

    if (aloca_memo(I, LIMY, LIMX)) {
        for (i = 0; i < LIMY; i++) {
            for (j = 0; j < LIMX; j++) {
                fscanf(arq, "%d", &k);
                if (k > MAX_NIVEL) {
                    printf("Erro nos DADOS do arquivo <%s>\n", nome);
                    printf("Valor lido: %d   Max Nivel: %d\n\n", k, MAX_NIVEL);
                    return FALSE;
                }
                *(*(I) + i * LIMX + j) = k;
            }
        }
        fclose(arq);
    } else {
        printf("Erro na ALOCACAO DE MEMORIA para o arquivo <%s>\n\n");
        printf("Rotina: le_imagem_pgm\n\n");
        fclose(arq);
        return FALSE;
    }
    *nc = LIMX;
    *nl = LIMY;
    *mn = MAX_NIVEL;
    return TRUE;
}

/*+-------------------------------------------------------------------------+
  | Rotina que grava o arquivo da imagem em formato PGM ASCII.              |
  | PARAMETROS:                                                             |
  | I    = ponteiro para o vetor imagem (entra).                            |
  | nome = nome do arquivo (entra).                                         |
  | nl   = numero de linhas (entra).                                        |
  | nc   = numero de colunas (entra).                                       |
  | mn   = maximo nivel de cinza (entra).                                   |
  +-------------------------------------------------------------------------+*/
void grava_imagem_pgm(imagem I, char *nome, int nl, int nc, int mn) {
    int i, j, x, k, valores_por_linha;
    FILE *arq;
    if ((arq = fopen(nome, "wt")) == NULL) {
        printf("Erro na CRIACAO do arquivo <%s>\n\n", nome);
    } else {
        fputs("P2\n", arq);
        fputs(CREATOR, arq);
        fprintf(arq, "%d  %d\n", nc, nl);
        fprintf(arq, "%d\n", mn);
        valores_por_linha = 540;
        for (i = 0, k = 0; i < nl; i++)
            for (j = 0; j < nc; j++) {
                x = *(I + i * nc + j);
                fprintf(arq, "%3d ", x);
                k++;
                if (k > valores_por_linha) {
                    fprintf(arq, "\n");
                    k = 0;
                }
            }
    }
    fclose(arq);
}

void negativo(imagem I, imagem O, int nl, int nc, int mn) {
    int i, j;
    for (i = 0; i < nl; i++)
        for (j = 0; j < nc; j++)
            O[i * nc + j] = 255 - I[i * nc + j];
}

int contaBolinhas(imagem I, imagem O, int nl, int nc, int mn){
    int i, j,cima,esquerda,centro,rotulo = 1,total,indexDependencia = 0;
    int vetorJaContados[1000],indexVetorJaContados = 0;
    for (i = 0; i < nl; i++){
        for (j = 0; j < nc; j++){
            
            centro = I[i * nc + j];
            if(i > 0 && j > 0){
                cima = O[(i-1) * nc + j];
                esquerda = O[i * nc + j - 1];
            }else{
                if(j == 0 && i > 0){
                    cima = O[(i-1) * nc + j];
                    esquerda = 255;
                }
                if( i == 0 && j > 0){
                    cima = 255;
                    esquerda = O[i * nc + j - 1];
                }
                if(i == 0 && j == 0){
                    cima = 255;
                    esquerda = 255;
                }
            }
            
            if(centro == 0){
                if(rotulado(cima,rotulo) && rotulado(esquerda,rotulo)){
                    if(cima != esquerda){
                        dependencias[indexDependencia].valor1 = cima;
                        dependencias[indexDependencia].valor2 = esquerda; 
                        indexDependencia++;
                    }
                    O[i * nc + j] = esquerda;
                }if(!rotulado(cima,rotulo) && rotulado(esquerda,rotulo)){
                    O[i * nc + j] = esquerda;
                }if(rotulado(cima,rotulo) && !rotulado(esquerda,rotulo)){
                    O[i * nc + j] = cima;
                }if(!rotulado(cima,rotulo) && !rotulado(esquerda,rotulo)){
                    if(rotulo == 255)
                        rotulo = 256;
                    O[i * nc + j] = rotulo;
                    rotulo++;
                }
            }else{
                O[i * nc + j] = centro;
            }            
        }
    }
    for (i = 0; i < nl; i++){
        for (j = 0; j < nc; j++){
            
            if(!jaContou(vetorJaContados,&indexVetorJaContados,indexDependencia,O[i * nc + j])){
                total++;
            }
        }
    }
    return total;
}

int jaContou(int *v, int *index, int indexDependencias, int valor){
    int i,indexJaContados = 0;
    if(valor == 0 || valor == 255){
        return TRUE;
    }
    for(i = 0; i < *index; i++){
        if(v[i] == valor){
            return TRUE;
        }
    }
    for(i = 0; i < indexDependencias; i++){
        if(dependencias[i].valor1 == valor){
            v[indexJaContados] = dependencias[i].valor2;
            indexJaContados++;
        }
        if(dependencias[i].valor2 == valor){
            v[indexJaContados] = dependencias[i].valor1;
            indexJaContados++;
        }
    }
    v[indexJaContados] = valor;
    return FALSE;
}

void imprimeMatriz(imagem O,int nl,int nc){
    int i, j;
    for(i = 0 ; i < nl; i++){
        for(j = 0; j < nc; j++){
            printf("%d ,", O[i * nc + j]);
        }
        printf("\n");
    }
}

int rotulado(int pixel, int rotulo){
    if(pixel >= 1 && pixel < rotulo  && pixel != 255){
        return TRUE;
    }else
        return FALSE;
}

void msg(char *s) {
    printf("\nNEGATIVO de uma imagem qualquer");
    printf("\n-------------------------------");
    printf("\nUSO.:  %s  <IMG.PGM>", s);
    printf("\nONDE:\n");
    printf("    <IMG.PGM>  Arquivo da imagem em formato PGM\n\n");
}

/*+------------------------------------------------------+
  |        P R O G R A M A    P R I N C I P A L          |
  +------------------------------------------------------+*/
main(int argc, char *argv[]) {
    int OK, nc, nl, mn, tipo_img, numBolinhas;
    char nome[100];
    char comando[100];
    imagem In, Out;
    if (argc < 2)
        msg(argv[0]);
    else {
        OK = le_imagem_pgm("/srv//Computacao/Processamento de Imagens/contarBolinhas.pgm", &In, &nl, &nc, &mn);
        if (OK) {
           // printf("\nCONTA BOLINHAS\n");
            aloca_memo(&Out, nl, nc);
            numBolinhas = contaBolinhas(In, Out, nl, nc, mn);
            printf("bolinhas = %d\n",numBolinhas);
           // strcpy(nome, argv[1]);
            //strcat(nome, "-negativo.pgm");
            //grava_imagem_pgm(Out, nome, nl, nc, mn);
           //strcpy(comando, "./i_view32 ");
           // strcat(comando, nome);
           // strcat(comando, " /hide=7");
            // system ("./i_view32 inverte.pgm /hide=7");
            //system(comando);
            desaloca_memo(&In);
            desaloca_memo(&Out);
        }
    }
    return 0;
}
