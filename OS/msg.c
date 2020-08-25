#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
struct msbuf {
    long mtype;
    char payload[32];
};
int main() {
    char pathname[]="msg.c";
    key_t key = ftok(pathname, 3);
    msgget(key, IPC_CREAT | 0666);
}
