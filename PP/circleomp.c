#include <omp.h>
#include <stdio.h>
#include <unistd.h>

int main() {
    int i;
    #pragma omp parallel for schedule(static)
    for(int n=0; n<10; ++n) {
        i = omp_get_num_threads();
        printf("Shalom! I'm number %d\n", i);
    }
    return 0;
}