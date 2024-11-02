#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>
#define MAX 10000
typedef struct{
	int quant;
	char nome[30];
}registro;

typedef struct{
	registro suma[MAX];
	int tamanho;
}tabela;
int contuni=0, contuni2=0, contuni3=0;
int NUM_COMP=0, NUM_MOV=0;
FILE *fptr;

void leitura_texto(char *file_path, tabela *t);
void leitura_stopwords(char *stopwords_path, tabela *t2);
void remover_stopwords(tabela *t, tabela *t2, tabela *t3);
void contador_palavras(tabela *t3, tabela *t4);
void ordenacao(tabela *t4);
void remover_duplicadas(tabela *t4);
void resumo(char *output_path, int tam, tabela *t4);


int main(int argc, char *argv[]){
    setlocale(LC_ALL,"");
    tabela t, t2, t3, t4;
    int tipo = atoi(argv[1]); // tipo de conteúdo (1-website, 2-arquivo de texto puro)
    char *file_path = argv[2]; // caminho para o website ou arquivo de texto
    int tam = atoi(argv[3]); // tamanho do resumo (a quantidade de palavras)
    char *stopwords_path = argv[4]; // caminho para o arquivo de texto com stopwords
    char *output_path = argv[5]; // caminho para o arquivo de texto de saída (resumo)
    
    switch (tipo){
    case 2:
        leitura_texto(file_path, &t);
        leitura_stopwords(stopwords_path, &t2);
        remover_stopwords(&t,&t2,&t3);
        contador_palavras(&t3,&t4);
        ordenacao(&t4);
        remover_duplicadas(&t4);
        resumo(output_path, tam, &t4);
        break;
    case 1:
        printf("Nosso trabalho e baseado em arquivo de texto puro!\n");
        break;
    default:
        printf("Opcao invalida\n");
        break;
    }
    
    return 0;
}
void leitura_texto(char *file_path, tabela *t){
    int i=0;
    printf("-----------------------------------------------------------------\n");
    fptr = fopen(file_path,"r");
    if (fptr==NULL){
        printf("Erro ao abrir arquivo\n");
        exit(1);
    }else{
        printf("Texto passado para tabela 1 com sucesso\n");  
        while (!feof(fptr)){
            fscanf(fptr, "%s", t->suma[i].nome);
            strtok(t->suma[i].nome, " !,.;?:)");
            strlwr(t->suma[i].nome);
            i++;
            contuni++;
        }
    }

    fclose(fptr);
}
void leitura_stopwords(char *stopwords_path, tabela *t2){
    int i=0;
    fptr = fopen(stopwords_path,"r");
    
    if (fptr==NULL){
        printf("Erro ao abrir arquivo\n");
        exit(1);
    }else{
        printf("Stopwords passadas para a tabela 2 com sucesso\n");  
        while (!feof(fptr)){

            fscanf(fptr, "%s", t2->suma[i].nome);
            strtok(t2->suma[i].nome, " !,.;?");
            strlwr(t2->suma[i].nome);
            i++;
            contuni2++;
        }
    }
    fclose(fptr);
}
void remover_stopwords(tabela *t, tabela *t2, tabela *t3){
    int cont=0, i, k;
    for(i=0;i<=contuni;i++){
        for(k=0;k<=contuni2;k++){
            if((strcmp(t->suma[i].nome,t2->suma[k].nome)==0)){
                cont++;
                break;
            }
        }
        if(cont==0){
            strcpy(t3->suma[contuni3].nome, t->suma[i].nome);
            contuni3++;
        }
        cont=0;
    }
    printf("Stopword removidas e texto (sem stopwords) passado para a tabela 3 com sucesso\n");
}

void contador_palavras(tabela *t3, tabela *t4){
    int cont=0, k, i;
    
    for(i=0;i<=contuni3;i++){
        for(k=0;k<=contuni3;k++){
            if((strcmp(t3->suma[i].nome, t3->suma[k].nome)==0)){
                cont++;
            }
        }
        t4->suma[i].quant = cont;
        strcpy(t4->suma[i].nome, t3->suma[i].nome);
        
        cont=0;
    }
    printf("Palavras contadas com sucesso!\n");
}

void ordenacao(tabela *t4){
    int i=0, k=0;
    int aux;
    char vet[30];
    
	for (i=1; i<contuni3; i++){
        NUM_MOV++;
        strcpy(vet,t4->suma[i].nome);
		aux = t4->suma[i].quant;
		k = i - 1;
        NUM_COMP++;
		while ((k>=0) && (aux < t4->suma[k].quant)){
            NUM_MOV++;
            strcpy(t4->suma[k+1].nome,t4->suma[k].nome);
			t4->suma[k+1].quant = t4->suma[k].quant;
			k--;
		}
        NUM_MOV++;
        strcpy(t4->suma[k+1].nome,vet);
		t4->suma[k+1].quant = aux;
	}
    printf("Palavras ordenadas com sucesso!\n");
    printf("Quantidade de palavras ordenadas = %d\n", contuni3);
}

void remover_duplicadas(tabela *t4){
    int l, i, k;
    for(i=0;i<=contuni3;i++){
        for(k=i + 1;k<=contuni3;k++){
            if((strcmp(t4->suma[i].nome, t4->suma[k].nome)==0)){
                for (l = k; l <= contuni3 - 1; l++) {
                    strcpy(t4->suma[l].nome, t4->suma[l+1].nome);
                    t4->suma[l].quant = t4->suma[l+1].quant;
                }
                contuni3--;   // decrementar o número de strings na tabela
                k--;   
            }
        }
    }
    printf("Palavras duplicadas removidas com sucesso!\n");
}
void resumo(char *output_path, int tam, tabela *t4){
    int i;
    fptr = fopen(output_path,"w");
    
    if (fptr==NULL){
        printf("Erro ao abrir arquivo\n");
        exit(1);
    }else{
        for (i=1; i<=tam; i++){
            fprintf(fptr,"%s %d\n", t4->suma[contuni3-i].nome, t4->suma[contuni3-i].quant);
        }
    }
    fclose(fptr);
    printf("Resumo pronto!\n");
    printf("Quantidade de movimentos = %d\nQuantidade de Comparacoes = %d\n", NUM_MOV, NUM_COMP);
    printf("-----------------------------------------------------------------\n");
}