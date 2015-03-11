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
	int bad_request=1;
	int fd;
	http_request request;
	FILE *fp =fdopen(socket_client , "w+");
	fgets_or_exit(buff,SIZE_BUFF,fp);
	bad_request = parse_http_request(buff,&request);
	skip_headers ( fp );
	printf("%d\n",bad_request);
	
	if (bad_request==0)
		send_response(fp , 400 , "Bad Request" , " Bad request \r\n" );
	else if ( request . method == HTTP_UNSUPPORTED )
		send_response ( fp , 405 , " Method Not Allowed" , " Method Not Allowed \r\n" );
	else if ( ( fd = check_and_open (request.url , "../html") ) != -1)
		send_file (fp, fd );
	else
		send_response (fp, 404 , "Not Found" , " Not Found \r\n" );
	
	printf("<Serveur> Un client a quitté le serveur.\n");
	fclose(fp);
	exit(0);
}
