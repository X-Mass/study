#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <wait.h>
#include <errno.h>
#include <stdlib.h>
int pid;
int parentid, recieved, offset;
char isStarted = 0;
/*Reciever*/

void userhandl1r(int nsig) {
    offset += 1;
    kill(pid, SIGUSR1);
}
void startHandler(int nsig) {
    isStarted = 1;
    (void) signal(SIGUSR1, userhandl1r);
    kill(pid, SIGUSR1);
}
void userhandl2r(int nsig) {
    recieved = recieved | (1 << offset);
    offset += 1;
    kill(pid, SIGUSR1);
}
void end(int nsig) {
    printf("Recieved: %d\n", recieved);
    exit(1);
}

int main(void){

    (void) signal(SIGUSR1, startHandler);
    (void) signal(SIGUSR2, userhandl2r);
    (void) signal(SIGINT, end);
    offset = recieved = 0;
    printf ("Mine ID is: %lu\nEnter other process ID: ", getpid());
    scanf ("%d",&pid);
    while(!isStarted) {
        kill(pid, SIGUSR2);
        sleep(0.2);
    }
    while(1);
    return 0;

}
