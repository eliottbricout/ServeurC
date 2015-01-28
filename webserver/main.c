#include <stdio.h>
#include <string.h>
#include "socket.h"
#include <unistd.h>

#define SIZE_BUFF 2048
char buff[SIZE_BUFF];

int main ( int argc , char **argv ){
/* Arnold Robbins in the LJ of February '95 , describing RCS */
	int socket_client;
	int ret;
	if(argc>1 && strcmp(argv[1], "-advice" ) == 0) {
		printf (" Don 't Panic !\n " );
		return 42;
	}
	socket_client=creer_serveur(8080);
	

	
	while((ret=read(socket_client ,buff,SIZE_BUFF))>0){
		if(write ( socket_client , buff , ret)==-1)
			perror("probleme de write");
	}
	
	
	
	printf ( " un client a quitter le serveur car il etait nul\n " );
	return 0;
	
}
