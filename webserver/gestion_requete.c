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

int parse_http_request ( const char * request_line , http_request * request ){

	char method[64];

	if((request->url=malloc(1024))==NULL){
		perror("pb malloc");
		return 0;
	}
	//&& version[0]>=0 && version[0]<=9 && version[1]>=0 && version[1]<=9
	if(sscanf(request_line,"%s %s HTTP/%d.%d",method, request->url, &request->major_version, &request->minor_version)==4 ){
		if(strcmp(method, "GET")==0){
			request->method=HTTP_GET;
			return 1;
		}else{
			request->method=HTTP_UNSUPPORTED;
		}
		
	}
		
	return 0;
}

void skip_headers ( FILE * client ){
	while(fgets_or_exit(buff , SIZE_BUFF, client)!=NULL && ligneVide(buff)==0){
	}
}

int ligneVide(char * str){
	if(str[0]=='\n' || str[0]=='\r' ){
		return 1;
	}
	return 0;

}

void send_status ( FILE * client , int code , const char * reason_phrase ){
	fprintf(client, "HTTP/1.1 %d %s\r\n", code,reason_phrase);
}

void send_response ( FILE * client , int code , const char * reason_phrase ,const char * message_body ){
	send_status(client ,code ,reason_phrase);
	fprintf(client, "Connection: close\r\nContent-Length: %d\r\n\r\n%s",(int)strlen(message_body),message_body);
}

char *fgets_or_exit ( char *buffer , int size , FILE *stream ){
	if(fgets(buffer,size,stream)==NULL){
		printf("<Serveur> Un client a quitté le serveur.\n");
		exit(0);
	}
	printf("%s",buffer);
	return buffer;
}
