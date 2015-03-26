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
#include "stats.h"
#include <sys/mman.h>

static web_stats* stats;

int init_stats(){
	if((stats=mmap ( NULL , sizeof(stats) ,PROT_READ | PROT_WRITE , MAP_SHARED | MAP_ANONYMOUS, -1, 0 ))==MAP_FAILED){
		perror("mmap");
	}
	stats->served_connections=0;
    stats->served_requests=0;
	stats->ok_200=0;
	stats->ko_400=0;
	stats->ko_403=0;
	stats->ko_404=0;
	return 0;
}

web_stats *get_stats(){
	return stats;
}
