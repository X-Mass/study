#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(){

    int N =10;

    int   semid;
    char pathname[]="08-1a.c";
    key_t key;
    struct sembuf mybuf[2];

    key = ftok(pathname, 3);
    
    if((semid = semget(key, 3, 0666 | IPC_CREAT)) < 0){
      printf("Can\'t create semaphore set\n");
      exit(-1);
    }
    
    mybuf[0].sem_flg = mybuf[1].sem_flg = 0;
    mybuf[0].sem_num = 0;
    mybuf[1].sem_num = 1;
    mybuf[0].sem_op = mybuf[1].sem_op = 1;
    if(semop(semid, mybuf, 2) < 0){
        printf("Can\'t raise for condition\n");
        exit(-1);
    }  

    for (int i = 0; i < N; i++) {
        // printf("parrent %d came to next itteration\n", i);
        pid_t child = fork();
        if (child != 0) { //child
            if (i%2 == 1) { //main road 1, 3, 5, ...
                mybuf[0].sem_num = 0;
                mybuf[0].sem_op = -1;
                mybuf[1].sem_num = 2;
                mybuf[1].sem_op = 1;
                if(semop(semid, mybuf, 2) < 0){
                    printf("Can\'t raise for condition\n");
                    exit(-1);
                }  
                printf("start->I");
                mybuf[1].sem_num = 1;
                mybuf[1].sem_op = -1;
                mybuf[0].sem_op = 1;
                if(semop(semid, mybuf, 2) < 0){
                    printf("Can\'t raise for condition\n");
                    exit(-1);
                }
                printf("->II");
                mybuf[1].sem_op = 1;
                mybuf[0].sem_num = 2;
                mybuf[0].sem_op = -1;
                if(semop(semid, mybuf, 2) < 0){
                    printf("Can\'t raise for condition\n");
                    exit(-1);
                }  
                printf("->finish (%d)\n", i+1);
            }
            else { //secondary road
                mybuf[0].sem_num = 1;
                mybuf[1].sem_num = 2;
                mybuf[0].sem_op = -1;
                mybuf[1].sem_op = 0;
                if(semop(semid, mybuf, 2) < 0){
                    printf("Can\'t raise for condition\n");
                    exit(-1);
                } 
                printf("start->II");
                mybuf[0].sem_op = 1;
                if(semop(semid, mybuf, 1) < 0){
                    printf("Can\'t raise for condition\n");
                    exit(-1);
                } 
                printf("->finish (%d)\n", i+1);
            }
            // printf("child is dying\n");
            break;
        }
        // printf("parrent %d going to next itteration\n", i);
    }
    
    return 0;
}