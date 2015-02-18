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

int creer_serveur (int port){

    int socket_serveur ;
    struct sockaddr_in saddr ;

    socket_serveur = socket( AF_INET , SOCK_STREAM , 0);
    if(socket_serveur == -1){
        perror("Probleme socket_serveur");
    }
    /* Utilisation de la socket serveur */

    int optval = 1;
    if(setsockopt(socket_serveur , SOL_SOCKET , SO_REUSEADDR , & optval , sizeof(int)) == -1){
        perror("Probleme Can not set SO_REUSEADDR option ");
    }


    saddr.sin_family = AF_INET ; /* Socket ipv4 */
    saddr.sin_port = htons (port); /* Port d ’ écoute */
    saddr.sin_addr.s_addr = INADDR_ANY ;
    /* écoute sur toutes les interfaces */

    if (bind(socket_serveur, (struct sockaddr *) &saddr, sizeof(saddr)) == -1){
        perror("Probleme Bind socker_serveur ");
    }


    if(listen(socket_serveur , 10) == -1){
        perror("Probleme Listen socket_serveur ");
    }

    return socket_serveur;
}
