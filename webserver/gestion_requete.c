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
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include "stats.h"


#define SIZE_BUFF 2048
char buff[SIZE_BUFF];

// analyse la request get
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
		}else{
			request->method=HTTP_UNSUPPORTED;
			return 0;
		}
		
	}
	rewrite_url ( request->url );
	printf("GET %s\n",request->url);
	return 1;
}
// recrit l'url (enleve ce qui a apres le ? et remplace / par /index.html)
char * rewrite_url ( char * url ){
	char * s;
	s=strchr(url,'?');
	if(s!=NULL){
		*s='\0';
	}
	if(strcmp(url,"/")==0){
		strcat(url,"index.html");
	}
	return url;
}
// skip les ligne apres le get
void skip_headers ( FILE * client ){
	while(fgets_or_exit(buff , SIZE_BUFF, client)!=NULL && ligneVide(buff)==0){
	}
}
// regarde si la ligne est vide
int ligneVide(char * str){
	if(str[0]=='\n' || str[0]=='\r' ){
		return 1;
	}
	return 0;
}
// check et ouvre le fichier document_root/url
int check_and_open ( const char * url , const char * document_root ){
	int fd;
	char buff[1024];
	strcpy(buff,document_root);
	if((fd = open(strcat(buff,url), O_RDONLY))==-1){
		return -1;
	}
	get_file_size(fd);
	return fd;
}
// retourne la taille du fichier
int get_file_size(int fd){
	struct stat buff;
	if  (fstat(fd,&buff) != 0) {  
      printf("error!\n" );  
      return 0;  
   }  
	return (int)buff.st_size;
}
// envoye l'entete (code / phrase d'avant )
void send_status ( FILE* client , int code , const char * reason_phrase ){
	if(fprintf(client, "HTTP/1.1 %d %s\r\n", code,reason_phrase)<0){
		perror("bug fprintf");
	}
}
// envoye l'entete (connection / content-length / content-Type /data
void send_response ( FILE * client , int code , const char * reason_phrase ,const char * message_body ){
	send_status(client ,code ,reason_phrase);
	if(fprintf(client, "Connection: close\r\nContent-Length: %d\r\nContent-Type: text/plain\r\n\r\n%s",(int)strlen(message_body),message_body)<0){
		perror("bug fprintf");
	}
}
//lis une ligne de la requete envoyee par le client sur size octets et renvoie la ligne lue
char *fgets_or_exit ( char *buffer , int size , FILE *stream ){
	if(fgets(buffer,size,stream)==NULL){
		printf("<Serveur> Un client a quitté le serveur.\n");
		exit(0);
	}
	return buffer;

}
//renvoyes les stats
void send_stats ( FILE * client ){
	web_stats* stats=get_stats();
	char ligne[1024];
	snprintf(ligne, 1024, "served_connections=%d \nserved_requests=%d\nok_200=%d\nko_400=%d\nko_403=%d\nko_404=%d",stats->served_connections,stats->served_requests,stats->ok_200,stats->ko_400,stats->ko_403,stats->ko_404);
	send_response (  client , 200 , "OK" , ligne);
}
// envoye le fichier est adapte le content-Length
void send_file (FILE* client, int file,const char *extention){
	send_status(client ,200 ,"OK");
	printf("Content-Type: %s\r\nContent-Length: %d\r\n\r\n",extention,get_file_size(file));
	if(fprintf(client, "Content-Type: %s\r\nContent-Length: %d\r\n\r\n",extention,get_file_size(file))<0){
		perror("bug fprintf");
	}
	fflush(client);
	copy(file,fileno(client));
}
//copy de in dans out
int copy(int in, int out) {

	int taille;
	char buff[8000];

	if((taille = read(in,buff,sizeof(buff)))==-1){
		perror("error read copy");
		return -1;
	}
	while (taille > 0){
		if((write(out,buff,taille)==0)){
			perror("error write copy");
			return -1;
		}
		if((taille = read(in,buff,sizeof(buff)))==-1){
			perror("error read copy");
			return -1;
		}
	}	
	return out;
}
