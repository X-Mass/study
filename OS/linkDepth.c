#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main() {
    int n = 0;
    char new[10], old[10]; 
    FILE *fptr;
    sprintf(old, "%d", n); 
    symlink("a.c", old);
    n += 1;
    while ((fptr = fopen(old,"r")) != NULL) {
        printf("OK. Current depth: %d\n", n);
        sprintf(new, "%d", n); 
        symlink(old, new);
        n += 1;
        strcpy(old, new);
        fclose(fptr);
    }
   printf("Total depth: %d", n - 1);
   fclose(fptr); 
}