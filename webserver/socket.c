#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>


int creer_serveur (int port ){

	int socket_serveur ;
	struct sockaddr_in saddr ;

	socket_serveur = socket( AF_INET , SOCK_STREAM , 0);
	if ( socket_serveur == -1){
		perror ( "socket_serveur" );
	}
	/* Utilisation de la socket serveur */
	
	
	saddr.sin_family = AF_INET ; /* Socket ipv4 */
	saddr.sin_port = htons (port); /* Port d ’ écoute */
	saddr.sin_addr.s_addr = INADDR_ANY ; 
	/* écoute sur toutes les interfaces */
	
	if(bind(socket_serveur, (struct sockaddr *) &saddr,sizeof(saddr))==-1){
		perror ( " bind socker_serveur " );
	}
	
	
	if ( listen ( socket_serveur , 10) == -1){
		perror ( " listen socket_serveur " );
	}
	return 0;
}
