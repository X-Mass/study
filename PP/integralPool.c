#include <stdlib.h>
#include <math.h>
#include <omp.h>
#include <stdio.h>
#include <time.h>
#include <stdbool.h>
#include <stddef.h>
#include <pthread.h>


struct tpool;
struct args;
typedef struct tpool tpool_t;

typedef void (*thread_func_t)(struct args *arg);

tpool_t *tpool_create(size_t num);
void tpool_destroy(tpool_t *tm);

bool tpool_add_work(tpool_t *tm, thread_func_t func, struct args *arg);

void tpool_wait(tpool_t *tm);
 struct tpool_work {
    thread_func_t      func;
    struct args              *arg;
    struct tpool_work *next;
};
typedef struct tpool_work tpool_work_t;

struct tpool {
    tpool_work_t    *work_first;
    tpool_work_t    *work_last;
    pthread_mutex_t  work_mutex;
    pthread_cond_t   work_cond;
    pthread_cond_t   working_cond;
    size_t           working_cnt;
    size_t           thread_cnt;
    bool             stop;
};
struct args {
    long double(*f)();
    long double a;
    long double b;
    long double prec;
    long double* ans; 
};

static tpool_work_t *tpool_work_create(thread_func_t func, struct args *arg)
{
    tpool_work_t *work;

    if (func == NULL)
        return NULL;

    work       = malloc(sizeof(*work));
    work->func = func;
    work->arg  = arg;
    work->next = NULL;
    return work;
}

static void tpool_work_destroy(tpool_work_t *work)
{
    if (work == NULL)
        return;
    free(work);
}


static tpool_work_t *tpool_work_get(tpool_t *tm)
{
    tpool_work_t *work;

    if (tm == NULL)
        return NULL;

    work = tm->work_first;
    if (work == NULL)
        return NULL;

    if (work->next == NULL) {
        tm->work_first = NULL;
        tm->work_last  = NULL;
    } else {
        tm->work_first = work->next;
    }

    return work;
}

static void *tpool_worker(void *arg)
{
    tpool_t      *tm = arg;
    tpool_work_t *work;

    while (1) {
        pthread_mutex_lock(&(tm->work_mutex));

        while (tm->work_first == NULL && !tm->stop)
            pthread_cond_wait(&(tm->work_cond), &(tm->work_mutex));

        if (tm->stop)
            break;

        work = tpool_work_get(tm);
        tm->working_cnt++;
        pthread_mutex_unlock(&(tm->work_mutex));

        if (work != NULL) {
            work->func(work->arg);
            tpool_work_destroy(work);
        }

        pthread_mutex_lock(&(tm->work_mutex));
        tm->working_cnt--;
        if (!tm->stop && tm->working_cnt == 0 && tm->work_first == NULL)
            pthread_cond_signal(&(tm->working_cond));
        pthread_mutex_unlock(&(tm->work_mutex));
    }

    tm->thread_cnt--;
    pthread_cond_signal(&(tm->working_cond));
    pthread_mutex_unlock(&(tm->work_mutex));
    return NULL;
}

tpool_t *tpool_create(size_t num)
{
    tpool_t   *tm;
    pthread_t  thread;
    size_t     i;

    if (num == 0)
        num = 2;

    tm             = calloc(1, sizeof(*tm));
    tm->thread_cnt = num;

    pthread_mutex_init(&(tm->work_mutex), NULL);
    pthread_cond_init(&(tm->work_cond), NULL);
    pthread_cond_init(&(tm->working_cond), NULL);

    tm->work_first = NULL;
    tm->work_last  = NULL;
    int j;
    for (j=0; j<num; j++) {
        pthread_create(&thread, NULL, tpool_worker, tm);
        pthread_detach(thread);
    }

    return tm;
}

void tpool_destroy(tpool_t *tm)
{
    tpool_work_t *work;
    tpool_work_t *work2;

    if (tm == NULL)
        return;

    pthread_mutex_lock(&(tm->work_mutex));
    work = tm->work_first;
    while (work != NULL) {
        work2 = work->next;
        tpool_work_destroy(work);
        work = work2;
    }
    tm->stop = true;
    pthread_cond_broadcast(&(tm->work_cond));
    pthread_mutex_unlock(&(tm->work_mutex));

    tpool_wait(tm);

    pthread_mutex_destroy(&(tm->work_mutex));
    pthread_cond_destroy(&(tm->work_cond));
    pthread_cond_destroy(&(tm->working_cond));

    free(tm);
}

bool tpool_add_work(tpool_t *tm, thread_func_t func, struct args *arg)
{
    tpool_work_t *work;

    if (tm == NULL)
        return false;

    work = tpool_work_create(func, arg);
    if (work == NULL)
        return false;

    pthread_mutex_lock(&(tm->work_mutex));
    if (tm->work_first == NULL) {
        tm->work_first = work;
        tm->work_last  = tm->work_first;
    } else {
        tm->work_last->next = work;
        tm->work_last       = work;
    }

    pthread_cond_broadcast(&(tm->work_cond));
    pthread_mutex_unlock(&(tm->work_mutex));

    return true;
}

void tpool_wait(tpool_t *tm)
{
    if (tm == NULL)
        return;

    pthread_mutex_lock(&(tm->work_mutex));
    while (1) {
        if ((!tm->stop && tm->working_cnt != 0) || (tm->stop && tm->thread_cnt != 0)) {
            pthread_cond_wait(&(tm->working_cond), &(tm->work_mutex));
        } else {
            break;
        }
    }
    pthread_mutex_unlock(&(tm->work_mutex));
}

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
    return sin(1.0/x) + cos(x*x/0.03) / x*2;
}
int main(int argc, char** argv)
{
    long double prec;
    int N = 4;//threads num (number of cores)
    int intervalNum = 20;
    clock_t start, end;
    printf("Precision: ");
    scanf("%Lf", &prec);
    long double* ansArray = calloc(intervalNum, sizeof(*ansArray));
    long double interval = (1 - 0.001) / intervalNum;
    tpool_t *tm   = tpool_create(N);
    start = clock();
    int i;
    for (i = 0; i < intervalNum; i++) {
        struct args *args = malloc(sizeof(*args));
        args->a = 0.001 + interval * i;
        args->b = 0.001 + interval * (i+1);
        args->prec = prec/intervalNum;
        args->ans = &ansArray[i];
        args->f = &f;
        tpool_add_work(tm, integrate, args);
        printf("%d work added. From: %.10Lf, to: %.10Lf\n", i, args->a, args->b);
    }
    tpool_wait(tm);
    long double answer = 0;
    for (i = 0; i < intervalNum; i++) {
        answer += ansArray[i];
    }
    printf("answer: %.10Lf\n", answer);
    end = clock();
    printf("Estimated time: %f\n", ((double) (end - start)) / CLOCKS_PER_SEC);
	return 0;
}