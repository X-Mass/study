#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
void getType(char* str, mode_t st_mode) {
    strcpy(str, "ERROR");
    if (S_ISREG(st_mode)) {
        strcpy(str, "regular");
    }
    if (S_ISDIR(st_mode)) {
        strcpy(str, "directory");
    }
    if (S_ISCHR(st_mode)) {
        strcpy(str, "character device");
    }
    if (S_ISBLK(st_mode)) {
        strcpy(str, "block device");
    }
    if (S_ISFIFO(st_mode)) {
        strcpy(str, "FIFO (named pipe)");
    }
    if (S_ISLNK(st_mode)) {
        strcpy(str, "link");
    }
    if (S_ISSOCK(st_mode)) {
        strcpy(str, "socket");
    }
}

int timespec2str(char *buf, uint len, struct timespec *ts) {
    int ret;
    struct tm t;

    tzset();
    if (localtime_r(&(ts->tv_sec), &t) == NULL)
        return 1;

    ret = strftime(buf, len, "%F %T", &t);
    if (ret == 0)
        return 2;
    len -= ret - 1;
    return 0;
}
int main(int argc, char *argv[]) {
    char* path;
    if (argc == 1) {
        path = ".";
    } else if (argc == 2) {
        path = argv[1];
    } else {
        printf("Too many arguments. Try again\n");
        return -1;
    }
    DIR* dir;
    if ((dir = opendir(path)) == NULL) {
        printf("Can not open directory. Check the name you typed\n");
        return -1;
    }
    errno = 0;
    struct stat sb;
    struct dirent* dp;
    char type[18]; //the longest type has 17 symblos + end
    char timeA[30];
    char timeM[30];
    char timeC[30];
    char fullPath[256];
    strcpy(fullPath, path);
    strcat(fullPath, "/");
    printf("FORMAT: NAME || TYPE || USER ID || GROUP ID || SIZE IN kB || LINKS NUMBER || I-NODE NUMBER || SYSTEM BLOCK SIZE || BLOCKS USED || DEVICE ID || TIME ACCESS || TIME MODIFICATION || TIME STATUS CHANGE || DEVICE ID (if special file)\n");
    while ((dp = readdir(dir)) != NULL) {
        strcat(fullPath, dp->d_name);
        if(stat(fullPath, &sb) == 0) {
            if (S_ISLNK(sb.st_mode)) { //if is link
                if(stat(fullPath, &sb) == 0) {
                    getType(type, sb.st_mode);
                    char endName[1024];
                    ssize_t len;
                    if ((len = readlink(fullPath, endName, sizeof(endName)-1))) {
                        endName[len] = '\0';
                    } else {
                        strcpy(endName, "Can't get file name");
                    }
                    timespec2str(timeM, sizeof(timeM), &sb.st_mtim);
                    timespec2str(timeA, sizeof(timeA), &sb.st_atim);
                    timespec2str(timeC, sizeof(timeC), &sb.st_ctim);
                    printf("LINKED(%10s) || %s || %ld || %ld || %lld || %ld || %ld || %ld || %lld || %ld || %s || %s || %s || %ld\n", 
                    endName, type, (long int)sb.st_uid, (long int)sb.st_gid, (long long int)sb.st_size / 1024, (long int)sb.st_nlink, (long int)sb.st_ino, 
                    (long int)sb.st_blksize, (long long int)sb.st_blocks, (long int)sb.st_dev, timeA, timeM, timeC, (long int)sb.st_rdev);
                    lstat(dp->d_name, &sb);
                } else {
                    printf("Can not read the linked file");
                }
            }
            getType(type, sb.st_mode);
            timespec2str(timeA, sizeof(timeA), &sb.st_atim);
            timespec2str(timeM, sizeof(timeM), &sb.st_mtim);
            timespec2str(timeC, sizeof(timeC), &sb.st_ctim);
            printf("%18s || %s || %ld || %ld || %lld || %ld || %ld || %ld || %lld || %ld || %s || %s || %s || %ld\n", 
                dp->d_name, type, (long int)sb.st_uid, (long int)sb.st_gid, (long long int)sb.st_size / 1024, (long int)sb.st_nlink, (long int)sb.st_ino, 
                (long int)sb.st_blksize, (long long int)sb.st_blocks, (long int)sb.st_dev, timeA, timeM, timeC, (long int)sb.st_rdev);
        } else {
            int err = errno;
            printf("%18s || Can not read attributes. errno: %d\n", dp->d_name, err);
        }
        errno = 0;
        strcpy(fullPath, path);
        strcat(fullPath, "/");
    }
    if (errno == 0) {
        closedir(dir);
        printf("----------That's all Folks!----------\n");
        return 0;
    } else {
        closedir(dir);
        printf("Reading error\n");
        return errno;
    }
} 