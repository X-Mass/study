#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define LAST_MESSAGE 255

int main(void)
{
    
      printf("-1\n");
    int msqid;
    char pathname[]="09-1a.c";
    key_t  key;
    int len, maxlen;

    struct mymsgbuf
    {
       long mtype;
       struct payload {
         int id;
         float num;
       } pl;
    } mybuf;



    /* Create or attach message queue  */
    
    key = ftok(pathname, 0);
    
    if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0){
       printf("Can\'t get msqid\n");
       exit(-1);
    }


    /* Send information */
    
    len = sizeof(mybuf.pl);
    for (int i = 0; i < 5; i++){
       
      mybuf.mtype = 2;
      mybuf.pl.num = 7331;
      mybuf.pl.symbol = 'W';
      printf("sending\n");
      if (msgsnd(msqid, (struct msgbuf *) &mybuf, len, 0) < 0){
      printf("Can\'t send message to queue\n");
      msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
      exit(-1);
      }
    }    
       
    /* Send the last message */   
       
    for (int i = 0; i < 5; i++){
      mybuf.mtype = 3;
      printf("waiting\n");
      if (( len = msgrcv(msqid, (struct msgbuf *) &mybuf, len, 3, 0)) < 0){
      printf("Can\'t receive message from queue\n");
      exit(-1);
      }
      printf("message num = %d, info = %c\n", mybuf.pl.num, mybuf.pl.symbol);
    }  

    return 0;    
}
