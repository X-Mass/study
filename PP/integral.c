
#include <math.h>
#include <omp.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
// void integrate (long double(*f)(), long double a, long double b, long double prec, long double *ans) {
//     long double fa, fb, qt, qs, c, fc;
//     fa = f(a);
//     fb = f(b);
//     qt = (b - a)/2 * (fa + fb);
//     c = (a + b)/2;
//     fc = f(c);
//     qs = (b - a)/6 * (fa+4*fc+fb);
//     if (fabsl(qt-qs) <= prec) {
//         *ans += qs;
//     } else {
//         integrate(f, a, c, prec/2, ans);
//         integrate(f, c, b, prec/2, ans);
//     }
// }
struct args {
    long double(*f)();
    long double a;
    long double b;
    long double prec;
    long double* ans; 
};
void integrate (struct args *args) {
    // sleep(1); 
    long double fa, fb, qt, qs, c, fc;
    long double(*f)() = args->f;
    fa = f(args->a);
    fb = f(args->b);
    qt = (args->b - args->a)/2 * (fa + fb);
    c = (args->a + args->b)/2;
    fc = f(c);
    qs = (args->b - args->a)/6 * (fa+4*fc+fb);
    if (fabsl(qt-qs) <= args->prec) {
        *(args->ans) += qs;
        // printf("this branch done: from %.10Le to %.10Le\n", args->a, args->b);
    } else {
        long double b = args->b;
        long double precision = args->prec;
        args->b = c;
        args->prec = precision / 2;
        // printf("brahcnig: from %.10Lf to %.10Lf with prec:%.10Lf\n", args->a, args->b, args->prec);
        integrate(args);
        args->prec = precision / 2;
        args->a = c;
        args->b = b;
        // printf("brahcnig: from %.10Lf to %.10Lf with prec:%.10Lf\n", args->a, args->b, args->prec);
        integrate(args);
    }
}
long double f(long double x) {
    return sin(1.0/x);
}

int main(int argc, char** argv)
{
    long double prec;
    clock_t start, end;
    // int intervalNum = 20;
    int N = 2;
    long double* ansArray = calloc(N, sizeof(*ansArray));
    printf("Precision: ");
    scanf("%Le", &prec);
    start = clock();
    // #pragma omp parallel for schedule(dynamic)
    // for (int i = 0; i < intervalNum; i++) {
    //     integrate(&f, 0.001 + i*interval, 0.001 + (i+1) * interval, prec/intervalNum, &ansArray[i]);
    // }
    pthread_t *thid = malloc(N * sizeof(pthread_t));
    long double interval = (1 - 0.001) / N;
    for (int i = 0; i < N; i++) {
        struct args *args = malloc(sizeof(*args));
        args->a = 0.001 + interval * i;
        args->b = 0.001 + interval * (i+1);
        args->prec = prec/N;
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
    end = clock();
    free(ansArray);
    printf("answer: %Le\n", answer);
    printf("Estimated time: %f\n", ((double) (end - start)) / CLOCKS_PER_SEC);
	return 0;
}