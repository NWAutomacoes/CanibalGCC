//Implementação por:
//Nícolas Wilson Souza
//Lucas Endo Prestes
//Universidade Federal de Santa Catarina
//Matéria: Sistemas computacionais para Controle e Automação
//09/2018
//nicolaszwilson@gmail.com

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define por 10 //Quantidade de porções que o cozinheiro faz
#define numCaniba 5 //Quantidade de canibais 
#define tempCoz 5 //Tempo para cozinhar em segundos
#define tempCome 1 //Tempo que o canibal demora para comer em segundos
int semIni = 10; //Quantidade de porções iniciais

pthread_mutex_t coz =     PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t fila =    PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t servh =   PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t foodReady =   PTHREAD_MUTEX_INITIALIZER;

/* thread */
void *Comer(){
	int semafor;
	while(1){
	pthread_mutex_lock(&fila);
	printf("Mutex Fila Locked\n");
	if(semIni > 0){ //Se houver comida na travessa, então...
		printf("Canibal comendo antes do else\nNum por: %d\n", semIni); //Animação canibal comendo
		semIni--;
		sleep(tempCome);
	} //Fechamento IF
	else { //Se não tem comida na travessa, então...
		printf("Chama o cozinheiro\n");		
		pthread_mutex_lock(&servh); //Bloqueia para o canibal ser o próximo a se servir quando o cozinheiro voltar
		pthread_mutex_unlock(&coz); //Desbloqueia o cozinheiro
		pthread_mutex_lock(&foodReady); //Fica esperando até o cozinheiro liberar
		printf("Canibal comendo depois do else\n"); //Animação do canibal comendo
		semIni--;
		pthread_mutex_unlock(&servh); //Desbloqueia para qualquer um poder pedir porção
	} //Fechamento ELSE
	printf("Canibal Comeu\n\n");
	fflush(stdout);
	pthread_mutex_unlock(&fila); //Desbloqueia para liberar a funilação de canibais tentando se servir
} //Fechamento WHILE
} //Fechamento VOID

void *Cozinha(){
	while(1){
	pthread_mutex_lock(&coz);
	printf("Cozinhando...\n");	
	semIni = por;
	sleep (tempCoz);
	printf("\nO chefe de cozinha Rodrigo Hilber está em ação...\n\n");
	fflush(stdout);
	pthread_mutex_unlock(&foodReady);
	}
}

int main(){
	int rc;
	pthread_t caniba[numCaniba];
	pthread_t cozinheiro;
	pthread_mutex_lock(&coz);
	pthread_mutex_unlock(&servh);
	pthread_mutex_unlock(&fila);
	pthread_mutex_lock(&foodReady);


	//Cria N threads Canibais
	for(int x=0; x < numCaniba; x++){
		rc = pthread_create(&caniba[x], NULL, Comer, NULL);
		printf("Criando canibal %d\n", x);
		if(rc){
			printf("Erro na criacao da Thread Canibal: %d\n", x);
		}
	}
	//Cria a thread do cozinheiro
	rc = pthread_create(&cozinheiro, NULL, Cozinha, NULL);
	printf("Contratando cozinheiro\n");
	if(rc){
		printf("Erro na criacao da Thread Cozinheiro");
	}
	pthread_join(cozinheiro, NULL);
	
}

