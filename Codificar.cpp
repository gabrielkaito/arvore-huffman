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

struct tree{
	int simbolo;
	int frequencia;
	struct tree *esq, *dir;
};
typedef struct tree Tree;

struct tabela{
	char palavra[20];
	int frequencia;
	char huffman[30];
	int simbolo;
	struct tabela *prox;
};
typedef struct tabela TpTabela;

struct floresta{
	struct tree *PTree;
	struct floresta *prox;
};
typedef struct floresta TpFloresta;



//===================================================================================================================================
struct Pilha{
	struct tree *No;
	struct Pilha *prox;
};
typedef struct Pilha Pilha;
#define MAXPILHA 20

struct TpPilha{ 
	int TOPO;
	int PILHA[MAXPILHA];
};
typedef struct TpPilha PilhaInt;

void inicializa (TpPilha *p){ 
	(*p).TOPO = -1;
} 

void insere (TpPilha *p, int elem){ 
	(*p).PILHA[++(*p).TOPO] = elem; 
} 

int retira (TpPilha *p){ 
	return (*p).PILHA[(*p).TOPO--];
} 

char isEmpty (Pilha *P){
	return P == NULL;
}

void init(Pilha **P){
	*P = NULL;
}

void Push (Pilha **P, Tree *No){
	Pilha *Nova;
	Nova = (Pilha*) malloc(sizeof(Pilha));
	Nova->No = No;
	Nova->prox = NULL;
	
	if (*P == NULL)
		*P = Nova;
	else{
		Nova->prox = *P;
		*P = Nova;
	}
}

void Pop (Pilha **P, Tree **No){
	*No = (*P)->No;
	*P = (*P)->prox;
}

//===================================================================================================================================

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
TpTabela *CriaNo(char palavra[], int simbolo){
	TpTabela *P = (TpTabela *)malloc(sizeof(TpTabela));
	
	strcpy(P -> palavra, palavra);
	P -> prox = NULL;
	P -> frequencia = 1;
	P -> simbolo = simbolo;
	
	return P;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
TpTabela *BuscaNo(TpTabela *P, char palavra[]){
	while(P != NULL && stricmp(P -> palavra, palavra) != 0)
		P = P -> prox;
	return P;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
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

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void OrdenaTabela(TpTabela **Tabela){
	TpTabela *inicio, *menor, *atual;
	
	char palavraAUX[20];
	int freqAUX, simboloAUX;
	
	
	inicio = *Tabela;
	atual = inicio -> prox;
	menor = *Tabela;
	
	while(inicio != NULL){
		while(atual != NULL){
			if(atual -> frequencia < menor -> frequencia)
				menor = atual;
			
			atual = atual -> prox;
		}
		
		//Troca
		strcpy(palavraAUX, menor -> palavra);
		freqAUX = menor -> frequencia;
		simboloAUX = menor -> simbolo;
		
		strcpy(menor -> palavra, inicio -> palavra);
		menor -> frequencia = inicio -> frequencia;
		menor -> simbolo = inicio -> simbolo;
		
		strcpy(inicio -> palavra, palavraAUX);
		inicio -> frequencia = freqAUX;
		inicio ->  simbolo = simboloAUX;
		//
		
		inicio = inicio -> prox;
		if(inicio != NULL)
			atual = inicio -> prox;
		menor = inicio;
	}
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
TpTabela *CriaTabela(char frase[]){
	TpTabela *Tabela = NULL, *aux, *atual;
	char palavra[20];
	int contSimbolo = 0, j;
	
	for(int i = 0; frase[i] != '\0'; i++){
		palavra[0] = '\0';
		
		if(frase[i] == 32){
			aux = BuscaNo(Tabela, " ");
			if(aux == NULL){
				if(Tabela == NULL){
					Tabela = CriaNo(" ", contSimbolo);
					contSimbolo++;
				}
				else{
					atual = Tabela;
					while(atual -> prox != NULL)
						atual = atual -> prox;
					atual -> prox = CriaNo(" ", contSimbolo);
					contSimbolo++;
				}
			}
			else
				aux -> frequencia++;
		}
		else{
			j = 0;
			while(frase[i] != 32 && frase[i] != '\0'){
				if(frase[i] == ',' || frase[i] == '!' || frase[i] == '?' || frase[i] == '.'){
					i++;
				}
				else{
					palavra[j] = frase[i];
					i++;
					j++;					
				}
			}
			i--;
			palavra[j] = '\0';
			
			aux = BuscaNo(Tabela, palavra);
			if(aux == NULL){
				if(Tabela == NULL){
					Tabela = CriaNo(palavra, contSimbolo);
					contSimbolo++;
				}
				else{
					atual = Tabela;
					while(atual -> prox != NULL)
						atual = atual -> prox;
					atual -> prox = CriaNo(palavra, contSimbolo);
					contSimbolo++;
				}				
			}	
			else
				aux -> frequencia++;
		}
	}
	
	
	OrdenaTabela(&Tabela);

	return Tabela;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Tree *CriaTree(int simbolo, int frequencia){
	Tree *nova = (Tree *)malloc(sizeof(Tree));
	nova -> simbolo = simbolo;
	nova -> frequencia = frequencia;
	nova -> dir = nova -> esq = NULL;
	
	return nova;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void InsereOrdenadoArvore(TpFloresta **Floresta, TpFloresta *nova){
	TpFloresta *ant = *Floresta, *atual;
	
	
	if((*Floresta) -> PTree -> frequencia > nova -> PTree -> frequencia){
		nova -> prox = *Floresta;
		*Floresta = nova;
	}
	else{
		atual = ant -> prox;
		while(atual != NULL && atual -> PTree -> frequencia < nova ->PTree -> frequencia){
			ant = ant -> prox;
			atual = atual -> prox;			
		}
		nova -> prox = atual;
		ant -> prox = nova;
	}	
		
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
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

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CriaHuffman(Tree *Raiz, TpTabela *Tabela){
	Pilha *P;
	PilhaInt PI;
	Tree *aux;
	TpTabela *T;
	char huffman[30];
	int i = 0, nivel, auxN;
	
	inicializa(&PI);
	init (&P);
	Push(&P, Raiz);
	insere(&PI,0);
	
	while(!isEmpty(P)){
		Pop(&P, &aux);
		nivel = retira(&PI);
		if(i != 0){
			i= nivel-1;
			
			huffman[i] = '1';
			i++;
		}
		
		if(aux -> esq == NULL && aux -> dir == NULL){
			//i++;
			huffman[i] = '\0';
			T = Tabela;
			while(T -> simbolo != aux -> simbolo)
				T = T -> prox;
			strcpy(T -> huffman, huffman);
				
		}
		auxN = nivel;
		while(aux != NULL){
			if(aux -> dir != NULL){
				Push(&P, aux -> dir);
				insere(&PI, auxN+1);
			}
			aux = aux -> esq;
			if(aux != NULL){
				huffman[i] = '0';
				i++;
				if(aux -> esq == NULL && aux -> dir == NULL){
					huffman[i] = '\0';
					T = Tabela;
					while(T -> simbolo != aux -> simbolo)
						T = T -> prox;
					strcpy(T -> huffman, huffman);	
				}
			}
			auxN++;
		}
	}
}
	 	
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void ArvoreHuffman(TpTabela *Tabela){
	TpTabela *auxTab = Tabela;
	TpFloresta *Floresta = NULL, *auxFloresta, *antF, *atualF, *novaF;
	int maior = 1, x;
	
	Floresta = (TpFloresta *)malloc(sizeof(TpFloresta));
	Floresta -> PTree = CriaTree(auxTab -> simbolo, auxTab -> frequencia);
	Floresta -> prox = NULL;
	auxFloresta = Floresta;
	auxTab = auxTab -> prox;
	
	while(auxTab != NULL){
		auxFloresta -> prox = (TpFloresta *)malloc(sizeof(TpFloresta));
		auxFloresta -> prox -> PTree = CriaTree(auxTab -> simbolo, auxTab -> frequencia);
		auxFloresta -> prox -> prox = NULL;
		auxFloresta = auxFloresta -> prox;
		
		auxTab = auxTab -> prox;
	}
	
	while(Floresta -> prox != NULL){
		antF = Floresta;
		atualF = antF -> prox;	
		
		novaF = (TpFloresta *)malloc(sizeof(TpFloresta));
		novaF -> prox = NULL;
		novaF -> PTree = CriaTree(-1, antF -> PTree -> frequencia + atualF -> PTree -> frequencia);
		novaF -> PTree -> esq = antF -> PTree;
		novaF -> PTree -> dir = atualF -> PTree;
		
		InsereOrdenadoArvore(&Floresta, novaF);
		Floresta = atualF -> prox;
		
		free(antF);
		free(atualF);
	}
	
	
	
	
	//clrscr();



	CriaHuffman(Floresta -> PTree, Tabela);
	Profundidade(Floresta -> PTree,1,&maior);
	x = pow(2,maior);
	exibeArvore(Floresta -> PTree,90,x/2,2);
	
	

}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void GravaArq (TpTabela *Tabela){
	FILE *ptr = fopen("NovoArq.dat","wb");
	R Reg;
	
	while(Tabela != NULL){
		strcpy(Reg.palavra, Tabela -> palavra);
		strcpy(Reg.huffman, Tabela -> huffman);
		Reg.simbolo = Tabela -> simbolo;
		Reg.frequencia = Tabela -> frequencia;
		fwrite(&Reg, sizeof(R),1,ptr);
		Tabela = Tabela -> prox;
	}
	fclose(ptr);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CodificaFrase(TpTabela *Tabela, char frase[]){
	FILE *ptr = fopen("codificado.txt","w");
	int i = 0, j = 0, k = 0;
	char palavra[30];
	char codifica[900];
	TpTabela *aux;
	
	for(i = 0; frase[i] != '\0'; i++){
		if(frase[i] == 32){
			aux = BuscaNo(Tabela, " ");
			strcpy(palavra, aux -> huffman);
			k = 0;
			while(palavra[k] != '\0'){
				codifica[j] = palavra[k];
				j++;
				k++;
			}
		}
		else{
			k = 0;
			while(frase[i] != 32 && frase[i] != '\0'){
				if(frase[i] == ',' || frase[i] == '!' || frase[i] == '?' || frase[i] == '.'){
					i++;
				}
				else{
					palavra[k] = frase[i];
					i++;
					k++;					
				}
			}
			i--;
			palavra[k] = '\0';
			aux = BuscaNo(Tabela, palavra);
			strcpy(palavra, aux -> huffman);
			k = 0;
			while(palavra[k] != '\0'){
				codifica[j] = palavra[k];
				j++;
				k++;
			}		
		}
	}
	codifica[j] = '\0';
	fputs(codifica,ptr);
	fclose(ptr);	
	
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

int main(void){
	TpTabela *Tabela;
	char frase[900];

	
	strcpy(frase, "Amo como ama o amor. Nao conheco nenhuma outra razao para amar senao amar. Que queres que te diga, alem de que te amo, se o que quero dizer-te e que te amo?");
	printf("(%s)\n", frase);
	
	Tabela = CriaTabela(frase);
	ArvoreHuffman(Tabela);	
	ExibeTabela(Tabela);
	strcpy(frase, "Quero dizer-te que amo como ama o amor. Senao amar nao conheco.");
	CodificaFrase(Tabela, frase);
	GravaArq(Tabela);

	
	printf("\n\n\n\n\n");
}








