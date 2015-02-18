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

#define SIZE_BUFF 2048
char buff[SIZE_BUFF];


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
		fprintf(fp, "%s", message_erreur400);
	}else{
		if(strcmp(request.url,"/")==0){
			fprintf(fp, "%s", message_bienvenu);
		}else{
			printf("%s", message_erreur404);
			fprintf(fp, "%s", message_erreur404);
		}
	}
	
	printf("<Serveur> Un client a quitté le serveur.\n");
	fclose(fp);
	exit(0);
}