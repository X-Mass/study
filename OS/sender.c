#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <wait.h>
#include <errno.h>
#include <stdlib.h>
int pid;
int toSend = 123451233;
int offset;
/*Sender*/
void userhandl2s(int nsig) {
    kill(pid, SIGUSR1);
}
void userhandl1s(int nsig) {
    if (offset == 33) {
        kill(pid, SIGINT);
        exit(1);
    }
    if ((toSend>>offset & 1) == 0) {
        kill(pid, SIGUSR1);
    }
    if ((toSend>>offset & 1) == 1) {
        kill(pid, SIGUSR2);
    }
    offset += 1;
}
int main(void){

    
    offset = 0;
    (void) signal(SIGUSR2, SIG_IGN);
    (void) signal(SIGUSR1, userhandl1s);
    printf ("Mine ID is: %lu\nEnter other process ID: ", getpid());
    scanf ("%d",&pid);
    printf ("Integer you want to send: ");
    scanf ("%d",&toSend);
    (void) signal(SIGUSR2, userhandl2s);
    while(1);
    return 0;

}
