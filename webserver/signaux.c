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


extern int nbclient;

void initSignaux(){
	if(signal(SIGPIPE, SIG_IGN) == SIG_ERR){
		perror("Problème de signal");
	}

	struct sigaction sa;
	sa.sa_handler = traitement_signal;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;

	if(sigaction(SIGCHLD, &sa, NULL) == -1){
		perror("Problème de sigaction(SIGCHLD)");
	}

}

void traitement_signal(int sig){
	sig=sig;
	wait(NULL);
	nbclient--;
}
