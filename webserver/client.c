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
#include "mySemaphore.h"
#include <semaphore.h>
#include "stats.h"
#include "mimes.h"

#define SIZE_BUFF 2048
char buff[SIZE_BUFF];

// gere le client 
void gestion_client(int socket_client){

	/* On peut maintenant dialoguer avec le client */
	int bad_request=1;
	int fd;
	http_request request;
	FILE *fp;
	//ouvre la conection
	if((fp =fdopen(socket_client , "w+"))==NULL){
		perror("erreur fdopen");
	}
	
	// semaphore
	if(sem_wait(getSemaphore())==-1){
             perror("sem_wait");
             exit(-1);
     	}
     	// ajoute 1 request aux stats
	get_stats()->served_requests++;
	if(sem_post(getSemaphore())==-1){
            perror("sem_post");
            exit(-1);
        }
         
         
	//lis la requete
	fgets_or_exit(buff,SIZE_BUFF,fp);
	bad_request = parse_http_request(buff,&request);
	skip_headers ( fp );
	//lis la requete
	
	// mauvaise requete 
	if (bad_request==0){
		 if(sem_wait(getSemaphore())==-1){
		     perror("sem_wait");
		     exit(-1);
		 }
		 get_stats()->ko_400+=1;
		 if(sem_post(getSemaphore())==-1){
		    perror("sem_post");
		    exit(-1);
		 }
		 send_response(fp , 400 , "Bad Request" , " Bad request \r\n" );
		
		
		
	// http Unsupported
	}else if ( request . method == HTTP_UNSUPPORTED ){
		send_response ( fp , 405 , "Method Not Allowed" , "Method Not Allowed \r\n" );
		
		
	//requete interdit (..) 
	}else if ( strstr(request.url , "..")  != NULL){
		 if(sem_wait(getSemaphore())==-1){
		     perror("sem_wait");
		     exit(-1);
		 }
		 get_stats()->ko_403+=1;
		 if(sem_post(getSemaphore())==-1){
		    perror("sem_post");
		    exit(-1);
		 }
	    	send_response ( fp , 403 ,"Forbidden" , "Forbidden \r\n" );
	    
	  //requete pour les stats
	}else if ( strstr(request.url , "stats")  != NULL){
		 if(sem_wait(getSemaphore())==-1){
		     perror("sem_wait");
		     exit(-1);
		 }
		 get_stats()->ok_200+=1;
		 if(sem_post(getSemaphore())==-1){
		    perror("sem_post");
		    exit(-1);
		 }
	   	 send_stats(fp);
	    
	  //requete get clasique
	}else if ( ( fd = check_and_open (request.url , "../html") ) != -1){
		if(sem_wait(getSemaphore())==-1){
		     perror("sem_wait");
		     exit(-1);
		}
		get_stats()->ok_200+=1;
		if(sem_post(getSemaphore())==-1){
		    perror("sem_post");
		    exit(-1);
		}
		printf("%s",getMineType(request.url ));
		send_file (fp, fd ,getMineType(request.url ) );
		
	// fichier non trouvé
	}else{
		printf("pas2 trouve");
		if(sem_wait(getSemaphore())==-1){
		     perror("sem_wait");
		     exit(-1);
		}
		printf("\n 404 :%d\n",get_stats()->ko_404);
		get_stats()->ko_404+=1;
		printf("\n 404 :%d\n",get_stats()->ko_404);
		if(sem_post(getSemaphore())==-1){
		    perror("sem_post");
		    exit(-1);
		}
		printf("pas4 trouve");
		send_response (fp, 404 , "Not Found" , " Not Found \r\n" );
	}
	printf("pas5 trouve");
	printf("<Serveur>Un client a quitté le serveur.\n");
	fclose(fp);
	exit(0);
}
