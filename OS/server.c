#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

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
    
    if ((msqid = msgget(key, 0666 | IPC_CREAT | IPC_EXCL)) < 0){
       printf("Can\'t get msqid\n");
       exit(-1);
    }
    len = sizeof(mybuf.pl);

    while (1) {
        if (( len = msgrcv(msqid, (struct msgbuf *) &mybuf, len, -2, 0)) < 0){
            printf("Can\'t receive message from queue\n");
            exit(-1);
        }
        printf("type: %d", mybuf.mtype);
        if (mybuf.mtype == 2) {
            mybuf.pl.num *= mybuf.pl.num;
            mybuf.mtype = mybuf.pl.id;  
            if (msgsnd(msqid, (struct msgbuf *) &mybuf, len, 0) < 0){
                printf("Can\'t send message to queue\n");
                msgctl(msqid, IPC_RMID, (struct  оmsqid_ds *) NULL);
                exit(-1);
            }
        } else if (mybuf.mtype == 1) {
            msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
            printf("Shutting down server\n");
            exit(1);
        }
    }
    return 0;       
}
