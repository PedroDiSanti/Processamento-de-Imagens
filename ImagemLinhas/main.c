#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef int * imagem;

#define TRUE  1
#define FALSE 0
#define CREATOR "# CREATOR: Luiz Eduardo V:1.00 R:MAR/2015\n"

/////LISTA
typedef struct elemento* Lista;

struct elemento {
    int num;
    struct elemento *prox;
};

typedef struct elemento Elem;

Lista* cria_lista() {
    Lista* li = (Lista*) malloc(sizeof (Lista));
    if (li != NULL) {
        *li = NULL;
    }
    printf("Lista Criada com sucesso!");
    return li;
}

void libera_lista(Lista* li) {
    if (li != NULL) {
        Elem* no;
        while ((*li) != NULL) {
            no = *li;
            *li = (*li)->prox;
            free(no);
        }
        free(li);
    }
}

int tamanho_lista(Lista* li) {
    if (li == NULL) return 0;
    int cont = 0;
    Elem* no = *li;
    while (no != NULL) {
        cont++;
        no = no->prox;
    }
    return cont;
}

int lista_vazia(Lista* li) {
    if (li == NULL) return 1;
    if (*li == NULL) return 1;
    return 0;
}

void printa_lista(Lista* li){
    Elem* no = *li;
    while (no->prox != (*li)) {
        printf("\n%d",no->num);
        no = no->prox;
    }
}

void modifica_lista(Lista *li, int melhor){
    Elem *aux = *li;
    int cont=1;
    while(cont != melhor){
       cont++;
       aux = aux->prox;
    }
    *li = aux;
    printa_lista(li);
}

void diminuir(Lista* li){
    Elem *aux = *li;
    int melhor,melhor2=0,cont=0,novo,novo2=0;
    while(aux->prox != (*li)){
        cont++;
        if(aux->num == 0){
            novo2++;
        }else{
            novo = cont - novo2;
            if(novo2>melhor2){
                melhor2 = novo2;
                melhor = novo;                
            }
            novo=0;
            novo2=0;
        }
        aux = aux->prox;
    }
    modifica_lista(li,melhor);
}



int insere_lista(Lista* li, int num) {
    if (li == NULL) return 0;
    Elem* no = (Elem*) malloc(sizeof (Elem));
    if (no == NULL) return 0;
    no->num = num;
    no->prox = NULL;
    if ((*li) == NULL) {//lista vazia:insere inicio
        *li = no;
        no->prox = no;
    } else {
        Elem *aux = *li; //sempre percorre uma lista pelo auxiliar e nunca com a cabeça dela (*li)
        while (aux->prox != (*li)) {
            aux = aux->prox;
        }
        aux->prox = no;
        no->prox = *li;
    }
    return 1;
}

////IMAGEM


int aloca_memo(imagem *I, int nl, int nc) {
    *I = (int *) malloc(nl * nc * sizeof (int));
    if (*I) return TRUE;
    else return FALSE;
}

int desaloca_memo(imagem *I) {
    free(*I);
}

void info_imagem(char *nome, int nl, int nc) {
    printf("\nINFORMACOES SOBRE A IMAGEM:");
    printf("\n--------------------------\n");
    printf("Nome do arquivo.............: %s \n", nome);
    printf("Numero de linhas............: %d \n", nl);
    printf("Numero de colunas...........: %d \n", nc);
}

int le_imagem_pbm(char *nome, imagem *I, int *nl, int *nc) {
    int i, j, k, LIMX, LIMY;
    char c, LINHA[100];
    FILE *arq;
    if ((arq = fopen(nome, "r")) == NULL) {
        printf("Erro na ABERTURA do arquivo <%s>\n\n", nome);
        return FALSE;
    }
    /*-- PBM = "P1" -----------*/
    fgets(LINHA, 80, arq);
    if (!strstr(LINHA, "P1")) {
        printf("Erro no FORMATO do arquivo <%s>\n\n", nome);
        return FALSE;
    }
    /*-- Dimensoes da imagem --*/
    fgets(LINHA, 80, arq);
    do {
        fgets(LINHA, 80, arq);
    } while (strchr(LINHA, '#'));
    sscanf(LINHA, "%d %d", &LIMX, &LIMY);

    if (LIMX == 0 || LIMY == 0) {
        printf("Erro nas DIMENSOES do arquivo <%s>\n\n", nome);
        return FALSE;
    }

    if (aloca_memo(I, LIMY, LIMX)) {
        for (i = 0; i < LIMY; i++) {
            for (j = 0; j < LIMX; j++) {
                fscanf(arq, "%d", &k);
                *(*(I) + i * LIMX + j) = k;
            }
        }
        fclose(arq);
    } else {
        printf("Erro na ALOCACAO DE MEMORIA para o arquivo <%s>\n\n");
        printf("Rotina: le_imagem_pbm\n\n");
        fclose(arq);
        return FALSE;
    }
    *nc = LIMX;
    *nl = LIMY;
    return TRUE;
}

void grava_imagem_pbm(imagem I, char *nome, int nl, int nc) {
    int i, j, x, k, valores_por_linha;
    FILE *arq;
    if ((arq = fopen(nome, "wt")) == NULL) {
        printf("Erro na CRIACAO do arquivo <%s>\n\n", nome);
    } else {
        fputs("P1\n", arq);
        fputs(CREATOR, arq);
        fprintf(arq, "%d  %d\n", nc, nl);
        valores_por_linha = 16;
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

void msg(char *s) {
    printf("\nContorno de uma imagem pbm");
    printf("\n-------------------------------");
    printf("\nUSO.:  %s  <IMG.PBM>", s);
    printf("\nONDE:\n");
    printf("    <IMG.PBM>  Arquivo da imagem em formato PBM\n\n");
}



void printa_In(imagem I,int nl, int nc){
    int i, j;
    for (i = 0; i < nl; i++){
        for (j = 0; j < nc; j++){
            printf("\n%d",I[i*nc+j]);
        }
    }
}

void borda(imagem I, imagem O, int nl, int nc) {
    Lista* li = cria_lista();
    int i, j,prox,aux,i2,j2,cont=0;
    char direcao;
    for (i = 0; i < nl; i++){
        for (j = 0; j < nc; j++){
            O[i*nc+j] = 0;
        }
    }
    for (i = nl; i >= 0; i--){
        for (j = nc; j >= 0; j--){
            if(I[i*nc+j]==1){
                if(cont == 0){
                    i2 = i;
                    j2 = j;
                    cont++;
                }
            }
        }
    }
    direcao = 'e';
    i = i2;
    j = j2;
    int cont2 = 0;
    j++;
    while( i2 != i || j2 != j ){
        if(cont2 == 0){
            j--;
        }
        ////////////////////////////////////////////////////////////////////
        if(direcao == 'd'){
            if(I[(i+1) * nc +j] == 1){
                i++;
                direcao = 'b';
                insere_lista(li,3);
                O[i*nc+j] = 1;
            }
            else if(I[i*nc+(j+1)] == 1){
                insere_lista(li,0);
                O[i*nc+j] = 1;
                j++;
            }
            else if (I[(i-1)*nc+j] == 0){
                j--;
                direcao = 'e';
                insere_lista(li,2);
                O[i*nc+j] = 1;
            }
            else{
                i--;
                insere_lista(li,1);
                O[i*nc+j] = 1;
                direcao = 'c';
            }
        }
        ///////////////////////////////////////////////////////////////////
        else if(direcao == 'b'){
            if(I[i * nc +(j-1)] == 1){
                j--;
                direcao = 'e';
                insere_lista(li,2);
                O[i*nc+j] = 1;
            }
            else if(I[(i+1)*nc+j] == 1){
                insere_lista(li,3);
                O[i*nc+j] = 1;
                i++;
            }
            else if(I[i*nc+(j+1)] == 0){
                i--;
                insere_lista(li,1);
                O[i*nc+j] = 1;
                direcao = 'c';
            }
            else {
                j++;
                insere_lista(li,0);
                O[i*nc+j] = 1;
                direcao = 'd';
            }
            
        }
        ////////////////////////////////////////////////////////////////
        else if(direcao == 'e'){
            if(I[(i-1)* nc +j] == 1){
                i--;
                direcao = 'c';
                insere_lista(li,1);
                O[i*nc+j] = 1;
            }
            else if (I[i * nc + (j-1)] == 1) {
                insere_lista(li, 2);
                O[i*nc+j] = 1;
                j--;
            }
            else if (I[(i+1) * nc + j] == 0){
                j++;
                insere_lista(li, 0);
                O[i*nc+j] = 1;
                direcao = 'd';
            }
            else{
                i++;
                insere_lista(li, 3);
                O[i*nc+j] = 1;
                direcao = 'b';
            }
        }
        /////////////////////////////////////////////////////////////
        else if(direcao == 'c'){
            if(I[i* nc +(j+1)] == 1){
                j++;
                direcao = 'd';
                insere_lista(li,0);
                O[i*nc+j] = 1;
            }
            else if(I[(i-1)*nc+j] == 1){
                insere_lista(li,1);
                O[i*nc+j] = 1;
                i--;
            }
            else if(I[i*nc+(j-1)] == 0){
                i++;
                insere_lista(li,3);
                O[i*nc+j] = 1;
                direcao = 'b';
            }
            else{
                j--;
                insere_lista(li,2);
                O[i*nc+j] = 1;
                direcao = 'e';
            }
        }
        cont2++;
    }
    printf("\n\nSequencia Normal!!\n");
    printa_lista(li);
    printf("\n\nSequencia Menor!!\n");
    diminuir(li);
    
}

int main(int argc, char** argv) {
    
    int OK, nc, nl,tipo_img; 
    char nome[100];
    char comando[100];
    char pbm[100];
    imagem In, Out;
    printf("Entre com o nome do arquivo: ");
    scanf("%s", pbm);
    if (argc < 0)
        msg(argv[0]);
    else {
        OK = le_imagem_pbm(pbm, &In, &nl, &nc);
        if (OK) {
            info_imagem(pbm, nl, nc);
            aloca_memo(&Out, nl, nc);
           // printa_In(In, nl, nc);
            borda(In, Out, nl, nc);
            strcpy(nome, pbm);
            strcat(nome, "-contorno.pbm");
            grava_imagem_pbm(Out, nome, nl, nc);
            strcpy(comando, "./i_view32 ");
            strcat(comando, nome);
            strcat(comando, " /hide=7");
            //system ("./i_view32 peixe.pbm /hide=7");
            system(comando);
            desaloca_memo(&In);
            desaloca_memo(&Out);
        }
    }
    return 0;
}
