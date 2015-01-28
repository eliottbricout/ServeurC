#include <stdio.h>
#include <string.h>
#include "socket.h"
#include <unistd.h>

#define SIZE_BUFF 2048
#define char buff[SIZE_BUFF];

int main ( int argc , char **argv ){
/* Arnold Robbins in the LJ of February '95 , describing RCS */
	int socket_client;
	if(argc>1 && strcmp(argv[1], "-advice" ) == 0) {
		printf (" Don 't Panic !\n " );
		return 42;
	}
	socket_client=creer_serveur(8080);
	
	while(1){
	
		while(read(socket_client ,buff,SIZE_BUFF)>0){
			write ( socket_client , buff , strlen (buff));
		}
	
	}
	
	printf ( " Need an advice ?\n " );
	return 0;
	
}
