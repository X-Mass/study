
#include <math.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
long double f(long double x) {
    return sin(1.0/x);
}
struct args {
    long double(*f)();
    long double a;
    long double b;
    long double step;
    long double* ans; 
};
void integrate (struct args *args) {
    long double s = f(args->a)+f(args->b); 
    for (int i = 1; args->a + i*args->step < args->b; i++) 
        s += 2*f(args->a + i * args->step); 
    *(args->ans) = (args->step/2)*s;
}


int main(int argc, char** argv)
{
    long double step, start, end;
    clock_t s, e;
    int N = 2;
    start = 0.001;
    end = 1;
    long double* ansArray = calloc(N, sizeof(*ansArray));
    pthread_t *thid = malloc(N * sizeof(pthread_t));
    printf("Step: ");//ввод шага
    scanf("%Le", &step);
    int stepsPerUnit = (int)((end - start)/step)/N;
    s = clock();
    for (int i = 0; i < N; i++) {
        struct args *args = malloc(sizeof(*args));
        args->a = start + stepsPerUnit * i * step;
        args->b = start + stepsPerUnit * (i+1) * step;
        args->step = step;
        args->ans = &ansArray[i];
        args->f = &f;
        pthread_create(&thid[i], (pthread_attr_t *)NULL, &integrate, (void *)args);
    }
    for (int i = 0; i < N; i++) {
        pthread_join(thid[i], (void **)NULL);
    }
    long double answer = 0;
    for (int i = 0; i < N; i++) {
        answer += ansArray[i];
    }
    e = clock();
    free(ansArray);
    printf("answer: %Le\n", answer);
    printf("Estimated time: %f\n", ((double) (e - s)) / CLOCKS_PER_SEC);
	return 0;
}