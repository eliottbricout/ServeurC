#include <stdio.h>
#include <string.h>
#include "socket.h"
#include "main.h"
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

#define SIZE_BUFF 2048
char buff[SIZE_BUFF];
int nbclient = 0;

int main(int argc , char **argv){
	int socket_serveur;
	int socket_client;

	initSignaux();
	if(argc > 1 && strcmp(argv[1], "-advice") == 0){
		printf("Don 't Panic !\n ");
		return 42;
	}

	socket_serveur = creer_serveur(8080);

	while(1){
		socket_client = accept(socket_serveur , NULL , NULL);
		if(socket_client == -1){
			perror("Bug de connection");
		}else{
			nbclient++;
			if(fork() == 0){
				printf("<Serveur> Un client vient de se connecter\n");

				printf("<Serveur> Vous avez %d client connecté\n", nbclient);

				gestion_client(socket_client);
			}
		}
	}

	return 0;

}

void gestion_client(int socket_client){
	
	/* On peut maintenant dialoguer avec le client */
	/*const char *message_bienvenue = " Bonjour, bienvenue sur mon serveur bitch \n " ;*/
	FILE *fp =fdopen(socket_client , "w+");
	/*	printf("<Bowser> %s",message_bienvenue);*/
	while(fgets(buff , SIZE_BUFF, fp)!=NULL){

		decoupageGET(buff);

		/* printf("<Serveur>%s",buff); */
	}

	printf("<Serveur> Un client a quitté le serveur.\n");
	exit(0);
}

void initSignaux(){
	if(signal(SIGPIPE, SIG_IGN) == SIG_ERR){
		perror("Problème de signal");
	}

	struct sigaction sa;
	sa.sa_handler = traitement_signal;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;

	if(sigaction(SIGCHLD, &sa, NULL) == -1){
		perror("Problème de sigaction(SIGCHLD)");
	}

}

void traitement_signal(int sig){
	printf("%d",sig);
	wait(NULL);
}

void decoupageGET(char * str){
	char s[2] = " ";
	char *token;
	int nbMot = 0;

	/* get the first token */
	token = strtok(str, s);
	printf("TOKEN : %s\n", token);

	if(strcmp(token,"GET") == 0){
		while( token != NULL ){
			token = strtok(NULL, s);
			printf("TOKEN : %s\n", token);
		}	
	}

}

