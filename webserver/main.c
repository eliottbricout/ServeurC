#include <stdio.h>
#include <string.h>
#include "socket.h"
#include "main.h"
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>

#define SIZE_BUFF 2048
char buff[SIZE_BUFF];
int nbclient=0;

int main ( int argc , char **argv ){
/* Arnold Robbins in the LJ of February '95 , describing RCS */
	int socket_serveur;
	int socket_client;
	if(argc>1 && strcmp(argv[1], "-advice" ) == 0) {
		printf (" Don 't Panic !\n " );
		return 42;
	}
	socket_serveur=creer_serveur(8080);
	
	while(1){
		socket_client = accept ( socket_serveur , NULL , NULL );
		if ( socket_client == -1){
			perror ( "bug de connection" );
		}else{
		nbclient++;
			if(fork()==0){
				printf ( " un client viens de ce connecter\n " );
				
				printf ( " vous avez %d client connecter \n ",nbclient );
				
				gestion_client(socket_client);
			}
		}
	}
	
	return 0;
	
}

void gestion_client(int socket_client){
	/* On peut maintenant dialoguer avec le client */
	const char * message_bienvenue = " Bonjour , bienvenue sur mon serveur bitch \n " ;
	int ret;
	write ( socket_client , message_bienvenue , strlen ( message_bienvenue ));
	
	while((ret=read(socket_client ,buff,SIZE_BUFF))>0){
		if(write ( socket_client , buff , ret)==-1)
			perror("probleme de write");
	}

	printf ( " un client a quitter le serveur car il etait nul\n " );
}
