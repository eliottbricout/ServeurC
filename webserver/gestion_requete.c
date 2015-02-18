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