#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <conio2.h>
#include <math.h>

struct Registro{
	char palavra[20];
	char huffman[30];
	int frequencia;
	int simbolo;
};
typedef struct Registro R;

struct tab{
	char palavra[20];
	char huffman[30];
	int frequencia;
	int simbolo;
	struct tab *prox;
};
typedef struct tab TpTabela;
	
struct tree{
	int simbolo;
	struct tree *esq, *dir;
};
typedef struct tree Tree;

//-------------------------------------------------------------------------------------------------------
void CriaTab(TpTabela **Tabela, Registro Reg){
	TpTabela *nova = (TpTabela *)malloc(sizeof(TpTabela));
	TpTabela *aux;
	
	strcpy(nova -> palavra, Reg.palavra);
	strcpy(nova -> huffman, Reg.huffman);
	nova -> simbolo = Reg.simbolo;
	nova -> frequencia = Reg.frequencia;
	nova -> prox = NULL;
	
	
	if(*Tabela == NULL)
		(*Tabela) = nova;
	else{
		aux = *Tabela;
		while(aux -> prox != NULL)
			aux = aux -> prox;
		aux -> prox = nova;	
	}
	
}

//-------------------------------------------------------------------------------------------------------
void ExibeTabela(TpTabela *P){
	
	printf("\n\n\n\n");
	while(P != NULL){
		printf("Simbolo: %d\n", P -> simbolo);
		printf("Palavra: %s\n", P -> palavra);
		printf("Huffman: %s\n", P -> huffman);
		printf("Frequen: %d\n", P -> frequencia);
		
		printf("--------------\n");
		P = P -> prox;
	}
}

//-------------------------------------------------------------------------------------------------------
void LeArq(TpTabela **Tabela){
	FILE *ptr = fopen("NovoArq.dat", "rb");
	R Reg;
	
	fread(&Reg, sizeof(R),1,ptr);	
	while(!feof(ptr)){
		CriaTab(&*Tabela, Reg);
		fread(&Reg, sizeof(R),1,ptr);
	}
	fclose(ptr);
}

//-------------------------------------------------------------------------------------------------------
Tree *CriaTree(int simbolo){
	Tree *nova = (Tree *)malloc(sizeof(Tree));
	nova -> simbolo = simbolo;
	nova -> dir = nova -> esq = NULL;
	
	return nova;
}

//-------------------------------------------------------------------------------------------------------
void ExcluiTab(TpTabela **Tabela, TpTabela *Excluir){
	TpTabela *ant, *atual;
	
	if(*Tabela == Excluir)
		*Tabela = (*Tabela) -> prox;
	else{
		ant = *Tabela;
		atual = (*Tabela) -> prox;
		while(atual != Excluir){
			atual = atual -> prox;
			ant = ant -> prox;
		}
		ant -> prox = atual -> prox;	
	}
	
}

//-------------------------------------------------------------------------------------------------------
void Arvore(Tree **arvore, Tree *nova, char huffman[]){
	int i;
	Tree *aux = *arvore;
	
	for(i = 0; huffman[i] != '\0'; i++){
		if(huffman[i] == '0'){
			if(huffman[i+1] != '\0'){
				if(aux -> esq == NULL)
					aux -> esq = CriaTree(-1);
				
				aux = aux -> esq;
			}
			else{
				aux -> esq = nova;
			}
		}
		else{
			if(huffman[i+1] != '\0'){
				if(aux -> dir == NULL)
					aux -> dir = CriaTree(-1);
	
				aux = aux -> dir;
			}
			else{
				aux -> dir = nova;
			}			
		}
	}
}

//-------------------------------------------------------------------------------------------------------
void CriaArvore(TpTabela *Tabela, Tree **arvore){
	TpTabela *maior, *aux, *T = Tabela;
	Tree *novaTree;
	char huffman[30];
	
	if(*arvore == NULL)
		*arvore = CriaTree(-1);
	
	while(Tabela != NULL){
		aux = Tabela;
		maior = Tabela;
		while(aux != NULL){
			if(maior -> frequencia < aux -> frequencia){
				maior = aux;
			}
			aux = aux -> prox;			
		}
		ExcluiTab(&Tabela, maior);
		
		novaTree = CriaTree(maior -> simbolo);
		strcpy(huffman, maior -> huffman);
		
		Arvore(&*arvore, novaTree ,huffman);	
	}

	
}

//-------------------------------------------------------------------------------------------------------
void exibeArvore(Tree *raiz, int x, int dist, int y)
{
	//Sleep(60);
    int i;
    if(raiz != NULL){		
    	gotoxy(x, y);
    	printf("%d", raiz->simbolo);
    	if(raiz->esq != NULL && dist>2)
    	{
    		gotoxy(x-1,y+1);
    		printf("/");
    		for(i=dist-2;i>1;i--){
    			gotoxy(x-i,y+2);
    			printf("-");
    		}
    		gotoxy(x-dist+1,y+3);
    		printf("/");
    	}
    	else if(raiz->esq != NULL && dist==2){
    		gotoxy(x,y+1);
    		printf("-");
    		gotoxy(x-1,y+2);
    		printf("/");
    		gotoxy(x-2,y+3);
    		printf("/");
    	}
    	if(raiz->dir != NULL && dist>2)
    	{
    		gotoxy(x+1,y+1);
    		printf("\\");
    		for(i=2;i<dist-1;i++){
    			gotoxy(x+i,y+2);
    			printf("-");
    		}
    		gotoxy(x+dist-1,y+3);
    		printf("\\");
    	}
    	else if(raiz->dir != NULL && dist==2){
    		gotoxy(x,y+1);
    		printf("-");
    		gotoxy(x+1,y+2);
    		printf("\\");
    		gotoxy(x+2,y+3);
    		printf("\\");	
    	}
    	exibeArvore(raiz->esq,x-dist,dist/2,y+4);
    	exibeArvore(raiz->dir,x+dist,dist/2,y+4);
    }
}
	
void Profundidade (Tree *Raiz, int n, int *Maior)
{
	if (Raiz != NULL)
	{
		if (n > *Maior)
			*Maior = n;
		Profundidade(Raiz->esq,n+1,&*Maior);
		Profundidade(Raiz->dir,n+1,&*Maior);
	}
}

//-------------------------------------------------------------------------------------------------------
void Decodificar(Tree *arvore, TpTabela **Tabela){
	FILE *ptr = fopen("codificado.txt", "r");
	char palavra[900];
	int i;
	Tree *aux;
	TpTabela *T;
	
	fscanf(ptr,"%s", &palavra);
	fclose(ptr);
	
	while(*Tabela != NULL){
		T = *Tabela;
		*Tabela = (*Tabela) -> prox;
		free(T);
	}
	LeArq(&*Tabela);
	
	aux = arvore;
	for(i = 0; palavra[i] != '\0'; i++){
		
		if(palavra[i] == '0')
			aux = aux -> esq;
		else
			aux = aux -> dir;
		
		if(aux -> dir == NULL && aux -> esq == NULL){
			T = *Tabela;
			while(T -> simbolo != aux -> simbolo)
				T = T -> prox;
			printf("%s", T -> palavra);
			aux = arvore;
		}
	}
	
}

//-------------------------------------------------------------------------------------------------------
int main(){
	TpTabela *Tabela = NULL;
	Tree *arvore = NULL;
	int maior = 1, x;
	
	LeArq(&Tabela);
	CriaArvore(Tabela, &arvore);
	Decodificar(arvore, &Tabela);
	ExibeTabela(Tabela);
	

	/*Profundidade(arvore,1,&maior);
	x = pow(2,maior);
	exibeArvore(arvore,90,x/2,2);*/
	//printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");


}















