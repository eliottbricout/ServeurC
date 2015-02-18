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
	int erreur=1;
	
	http_request request;
	const char *message_erreur400 = "HTTP/1.1 400 Bad Request\r\nConnection: close\r\nContent-Length: 18\r\n\r\n400 Bad request !\n";
	const char *message_erreur404 = "HTTP/1.1 404 Bad Request\r\nConnection: close\r\nContent-Length: 18\r\n\r\n404 Bad request !\n";
	const char *message_bienvenu = "HTTP/1.1 200 OK\r\nConnection: close\r\nContent-Length: 17\r\n\r\n Good request ! \n";
	FILE *fp =fdopen(socket_client , "w+");
	fgets_or_exit(buff,SIZE_BUFF,fp);
	erreur = parse_http_request(buff,&request);

	while(fgets_or_exit(buff , SIZE_BUFF, fp)!=NULL && ligneVide(buff)==0){
	}
	printf("%d\n",erreur);
	
	if(erreur==0){
		fprintf(fp ,message_erreur400);
	}else{
		if(strcmp(request.url,"/")==0){
			fprintf(fp ,message_bienvenu);
		}else{
			printf("%s" ,message_erreur404);
			fprintf(fp ,message_erreur404);
		}
	}
	
	printf("<Serveur> Un client a quitté le serveur.\n");
	fclose(fp);
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

int parse_http_request ( const char * request_line , http_request * request ){
	if((request->url=malloc(1024))==NULL){
		perror("pb malloc");
		return 0;
	}
	//&& version[0]>=0 && version[0]<=9 && version[1]>=0 && version[1]<=9
	if(sscanf(request_line,"GET %s HTTP/%d.%d",request->url,&request->major_version,&request->minor_version)==3 ){
		
		return 1;
	}
		
	return 0;
}

int ligneVide(char * str){
	if(str[0]=='\n' || str[0]=='\r' ){
		return 1;
	}
	return 0;

}

char *fgets_or_exit ( char *buffer , int size , FILE *stream ){
	if(fgets(buffer,size,stream)==NULL){
		printf("<Serveur> Un client a quitté le serveur.\n");
		exit(0);
	}
	printf("%s",buffer);
	return buffer;
}

