#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
int main()
{
    char     *array;
    int     shmid;
    char    pathname[] = "b.c";
    key_t   key;
    int fd = open(pathname, O_RDONLY);
    if((key = ftok(pathname,0)) < 0){
        printf("Can\'t generate key\n");
        exit(-1);
    }
    if((shmid = shmget(key, sizeof(char) * 20000, 0)) < 0){
        printf("Can\'t find shared memory\n");
        exit(-1);
    }
    if((array = (char *)shmat(shmid, NULL, 0)) == (char *)(-1)){
        printf("Can't attach shared memory\n");
        exit(-1);
    }
    printf("%s", array);
}
