#include "mySemaphore.h"
#include <unistd.h>
#include <stdio.h>
#include <sys/mman.h>

sem_t *semaphore;

sem_t *getSemaphore(){
    return semaphore;
}

/*fonction d'initialisation de la semaphore*/
int initSemaphore(){
    /*creation d'une zone memoire partagee pour la semaphore*/
    void *mms;
    mms=mmap(NULL, sizeof(sem_t), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
    if(mms==MAP_FAILED){
        perror("mmap semaphore");
        return -1;
    }
    semaphore=mms;
    
    if(sem_init(semaphore, 1, 1)==-1){
     	perror("init semaphore");
        return -1;
    }
    return 0;
}
