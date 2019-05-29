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
struct{
	//define a identificação
	int id;
} dadosMatriz;

struct dadosMatriz * inf;

//guardará os ids das threds
pthread_t * tId;

//gera a matriz da metade para cima
void matrizSuperior(int cont){
	int 
		//variaveis para o loop
		i,j;
		
	//pega a matriz	
	for(i=cont;i<tamanhoMatriz;i+=nThread){
		for(j=i;j<tamanhoMatriz;j++){
			diag1[i][j] = matriz[i][j];
			diag2[i][j] = 0;
		}
	}
}


//gera a matriz da metade para baixo
void matrizInferior(int cont){
	int 
		//variaveis para o loop
		i,j;
		
	//pega a matriz	
	for(i=cont;i<tamanhoMatriz;i+=nThread){
		for(j=0;j<i;j++){
			diag2[i][j] = matriz[i][j];
			diag1[i][j] = 0;
		}
	}
}

//trabalha as funções das matrizes
void * operaMatrizes(struct dadosMatriz * dados){
	matrizSuperior(dados->id);
	matrizInferior(dados->id);		
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
	arq = fopen(nome , "w");		
	
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
int ler (double ** aux, string nome){
	FILE 
		//crio "arq" - leitura
		* arq; 
	
	//abre arquivo
	arq = fopen(nome , "r");		
	
	//caso o arquivo não for aberto, dê erro
	if(arq == NULL)
		return 1;
	
	i = 0;
	//começa a sequencia de gravação da matriz
	while(!feof(arq)){
		
		//inicializa a linha da matriz
		aux[i] = (double) malloc(tamanhoMatriz * sizeof(double));
		
		for(j=0;j<tamanhoMatriz;j++){
			fscanf(arq,"%lf",aux[i][j]);
		}
				
		i++;
    }
    
    //fecha o arquivo
	fclose(arq);
	
   return 0;
   
}
 
int main(int argc, char argv[][]){
	
	char 
		//guarda o caminho do arquivo
		caminho[75] ,
		saida1[75],saida2[75];
		
	int 
		i;
	clock_t 
		tIni,tFim;
	
	//verefica o numero de argumentos
	if(argc < 3){
		cout << "Número de argumentos insuficiente" << endl;
		return 1;
	}
	
	//recebe o tamanho da matriz NxN
 	tamanhoMatriz = stoi(argv[0]);
 	
 	//recebe a quantidade de Threads
 	nThread = stoi(argv[1]);
 	
 	//recebe o caminho da matriz
 	caminho = argv[2];
 	
 	
 	//gera os caminhos de saidas
 	strcpy(saida1,caminho);
 	strcpy(saida1,".diag1");
 	
 	strcpy(saida2,caminho);
 	strcpy(saida2,".diag2");
 	
 	//inicializa as matrizes
 	matriz = (double *) malloc(tamanhoMatriz * tamanhoMatriz * sizeof(double *));
	diag1  = (double *) malloc(tamanhoMatriz * tamanhoMatriz * sizeof(double *));
	diag2  = (double *) malloc(tamanhoMatriz * tamanhoMatriz * sizeof(double *));
	
	//lê o arquivo com a matriz
	ler(matriz,caminho);
	
	//inicializa a struct para passar para as threads
	inf = (dadosMatriz) malloc(nThread * sizeof(dadosMatriz));
	tId = (pthread_t) malloc(nThread * sizeof(pthread_t));
	
	//grava o tempo incial
	tIni = clock();
	
	//aloca as threads
	for(i=0;i<nThread;i++){
		inf[i]->id = i;
		pthread_create(tId[i],NULL,operaMatrizes,inf);
	}

	
	for(i=0;i<nThread;i++){
		pthread_join(tId[i],NULL);
	}
	
	//grava o tempo final
	tFim = clock();
 	
 	printf("O tempo gasto com as threds foi de %lf segundos.\n",double(tFim - tIni)/CLOCKS_PER_SEC);
 	
 	//grava as matrizes
 	gravar(diag1,saida1);
 	gravar(diag1,saida2);
	return 0;
	
}

