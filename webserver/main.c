#include <stdio.h>
#include <string.h>
#include "socket.h"
#include "main.h"
#include "gestion_requete.h"
#include "client.h"
#include "signaux.h"
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include "stats.h"
#include "mySemaphore.h"

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
	
	if(init_stats()!=0){
	 	printf("erreur stats\n");
        return 42;
	}
	if(initSemaphore()!=0){
        printf("erreur semaphore\n");
        return 42;
    }
    	//creation du serveur
	socket_serveur = creer_serveur(8080);

	if(socket_serveur == -1){
          printf("erreur de creation serveur\n");	
          return 42;
	}

	while(1){
		socket_client = accept(socket_serveur , NULL , NULL);
		if(socket_client == -1){
			perror("Bug de connection");
		}else{
			nbclient++;
			// creation du fils
			if(fork() == 0){
				if(sem_wait(getSemaphore())==-1){
				     perror("sem_wait");
				     exit(-1);
			     	}
				get_stats()->served_connections++;
				if(sem_post(getSemaphore())==-1){
				    perror("sem_post");
				    exit(-1);
				 }
				printf("<Serveur> Un client vient de se connecter\n");

				printf("<Serveur> Vous avez %d client connecté\n", nbclient);
				gestion_client(socket_client);
			}else{
				close(socket_client);
			}
		}
	}

	return 0;

}



