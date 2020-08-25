#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
struct args {
    long int a;
    long int b;
    long int *num;
    long int *denum;
};
void *func(void *i) {
    struct args *args = i;
    int n, d;
    d = 1;
    n = 0;
    for (int i = args->a + 1; i < args->b + 1; i++) {
        n = n * i + 1;
        d *= i;
    }
    *args->denum = d;
    *args->num = n;
    return NULL;
}

int main() {
    int N = 2;
    int members = 10;
    pthread_t *thid = malloc(N * sizeof(pthread_t));
    long int *ansNum = malloc(N * sizeof(*ansNum));
    long int *ansDenum = malloc(N * sizeof(*ansDenum));
    for (int i = 0; i < N; i++) {
        struct args *args = malloc(sizeof(*args));
        args->a = members * i / N;
        args->b = members * (i+1) / N;
        args->num = &ansNum[i];
        args->denum = &ansDenum[i];
        printf("from %ld to %ld\n", args->a, args->b);
        pthread_create(&thid[i], (pthread_attr_t *)NULL, &func, (void *)args);
    }
    for (int i = 0; i < N; i++) {
        pthread_join(thid[i], (void **)NULL);
    }
    long int n, d;
    n = ansNum[0];
    d = ansDenum[0];
    for (int i = 1; i < N; i++) {
        n = n * ansDenum[i] + ansNum[i];
        d *= ansDenum[i];
    }
    printf("answer: %Le", 1+ (long double)n / (long double)d);
    return 0;
}