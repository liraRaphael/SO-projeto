/**
 *	GRUPO - Os Cotileiros 
 *	Projeto 1
 *	
 *	Giovanni Bassetto - 216968
 *	Raphael Lira dos Santos - 223865
 *
**/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <time.h>

int 
	//guarda o tamanho da matriz
	tamanhoMatriz,
	//numero de Threads
	nThread;

double
	//matriz principal que será carregada
	**matriz,
	**diag1,**diag2;
	
	
//struct para conter os dados das threads
typedef struct dadosMatriz{
	//define a identificação
	int id;
} dadosMatriz;

dadosMatriz * inf;

//guardará os ids das threds
pthread_t * tId;

//metodo que inicia as matrizes
void iniciaMatrizes(double ** aux){
	int
		i;	
	for(i=0;i<tamanhoMatriz;i++){
		aux[i] = (double *) malloc(tamanhoMatriz * sizeof(double));
		
	}

}

//trabalha as funções das matrizes
void * operaMatrizes(void * args){
	
	dadosMatriz * dados = args;
	
	
	int 
		//variaveis para o loop
		i,j;
		
	//transforme a matriz diag1
	for(i=dados->id;i<tamanhoMatriz;i+=nThread){
		for(j=i;j<tamanhoMatriz;j++){
			diag1[i][j] = matriz[i][j];
			diag2[i][j] = 0;
		}
	}

	//transforme a matriz diag2
	for(i=dados->id;i<tamanhoMatriz;i+=nThread){
		for(j=0;j<i;j++){
			diag2[i][j] = matriz[i][j];
			diag1[i][j] = 0;
		}
	}	
		
		
}


//grava a matriz em arquivo
int gravar(double ** aux, char * nome){
	
	FILE 
		//crio "arq" - gravação
		* arq; 
		
	int 
		//responsável por fazer o loop de gravação
		i,j;

	//abre arquivo
	arq = fopen(nome , "w+");		
	
	//caso o arquivo não for aberto, dê erro
	if(arq == NULL)
		return 1;
	
	//começa a sequencia de gravação da matriz
	for(i=0;i<tamanhoMatriz;i++){
		for(j=0;j<tamanhoMatriz;j++){
			fprintf(arq,"\t%lf",aux[i][j]);
		}
		
		//quebra a linha
		fprintf(arq,"\n");
	}
		
	
	//fecha o arquivo
	fclose(arq);
	
   return 0;
   
}

//le arquivo da matriz
int ler (char * nome){
	FILE 
		//crio "arq" - leitura
		* arq; 
	
	int 
	    i,j;
	
	
	//abre arquivo
	arq = fopen(nome , "r");		
	
	//caso o arquivo não for aberto, dê erro
	if(arq == NULL)
		return 1;
	
	i = 0;
	//começa a sequencia de gravação da matriz
	while(!feof(arq) && i < tamanhoMatriz){
		
		for(j=0;j<tamanhoMatriz;j++){
			fscanf(arq,"%lf",&matriz[i][j]);			
		}

		i++;
    }
    
    //fecha o arquivo
	fclose(arq);
	
   return 0;
   
}
 
int main(int argc, char ** argv){
	
	char 
		//guarda o caminho do arquivo
		* caminho ,
		* saida1, * saida2;
		
	int 
		i;
	clock_t 
		tIni,tFim;
	
	//verefica o numero de argumentos
	if(argc < 4){
		printf("Numero de argumentos insuficiente.\n");
		return 1;
	}
	

	//recebe o tamanho da matriz NxN
	sscanf(argv[1],"%d",&tamanhoMatriz);
 	
 	//recebe a quantidade de Threads
 	sscanf(argv[2],"%d",&nThread);
 	
 	//recebe o caminho da matriz
 	caminho = argv[3];
 	
	//aloca o char das matrizes
 	saida1 = (char *) malloc((strlen(caminho) + 6) * sizeof(char *));
	saida2 = (char *) malloc((strlen(caminho) + 6) * sizeof(char *));

 	//gera os caminhos de saidas
	sscanf(caminho,"%s",saida1);
	strcat(saida1,".diag1");

	sscanf(caminho,"%s",saida2);
	strcat(saida2,".diag2");
 	
 	//inicializa as matrizes
 	matriz = (double **) malloc(tamanhoMatriz * sizeof(double *));
	diag1  = (double **) malloc(tamanhoMatriz * sizeof(double *));
	diag2  = (double **) malloc(tamanhoMatriz * sizeof(double *));

	iniciaMatrizes(matriz);
	iniciaMatrizes(diag1);
	iniciaMatrizes(diag2);
	
	//lê o arquivo com a matriz
	if(ler(caminho)){
		printf("Erro ao ler o arquivo.\n");
		return 1;
	}


	//inicializa a struct para passar para as threads
	inf = (dadosMatriz *) malloc(nThread * sizeof(dadosMatriz));
	tId = (pthread_t *)   malloc(nThread * sizeof(pthread_t));
	
	//grava o tempo incial
	tIni = clock();
	
	//aloca as threads
	for(i=0;i<nThread;i++){
		inf[i].id = i;
		pthread_create(&tId[i],NULL,operaMatrizes,&inf[i]);
	}

	
	for(i=0;i<nThread;i++){
		pthread_join(tId[i],NULL);
	}
	
	//grava o tempo final
	tFim = clock();
 	
 	printf("O tempo gasto com as threds foi de %lf segundos.\n",(double) (tFim - tIni)/CLOCKS_PER_SEC);
 	
	
 	//grava as matrizes
 	if(gravar(diag1,saida1)){
		printf("Erro ao gravar o arquivo.\n");
		return 1;
	}
	
 	if(gravar(diag2,saida2)){
		printf("Erro ao gravar o arquivo.\n");
		return 1;
	}
	
	return 0;
	
}

