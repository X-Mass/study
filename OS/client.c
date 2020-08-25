#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define LAST_MESSAGE 255

int main(void)
{
    int msqid;
    char pathname[]="09-1a.c";
    key_t  key;
    int len, maxlen;

    struct mymsgbuf
    {
       long mtype;
       struct payload {
         int id;
         int num;
       } pl;
    } mybuf;
    
    key = ftok(pathname, 0);
    
    if ((msqid = msgget(key, 0666)) < 0){
       printf("Can\'t get msqid\n");
       exit(-1);
    }
    len = sizeof(mybuf.pl);
    mybuf.mtype = 2;
    mybuf.pl.id = getpid();
    scanf("%d", &mybuf.pl.num);
    if (msgsnd(msqid, (struct msgbuf *) &mybuf, len, 0) < 0){
        printf("Can\'t send message to queue\n");
        msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
        exit(-1);
    }

    if (( len = msgrcv(msqid, (struct msgbuf *) &mybuf, len, getpid(), 0)) < 0){
        printf("Can\'t receive message from queue\n");
        exit(-1);
    }
    printf("square: %d", mybuf.pl.num);

    return 0;       
}
