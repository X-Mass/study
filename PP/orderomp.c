#include <omp.h>
#include <stdio.h>
#include <unistd.h>
int main() {
    int i = 1;
    #pragma omp parallel for ordered schedule(static)
    for(int n=0; n<10; ++n) {
        #pragma omp ordered
        printf("value %d by thread %d\n", i*=2, n);
    }
}