#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
int main(int argc, char** argv) {
    MPI_Init(NULL, NULL);
    
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int worldSize;
    MPI_Comm_size(MPI_COMM_WORLD, &worldSize);
    int n;
    if (argc == 1) {
        n = 100000;//default number of bites used to staore answer (up to 19969)
    }
    else {
        n = atoi(argv[1]);
    }
    int number;
    int upperBorder = n < (rank + 1) * ((n / worldSize)+1) ? n : (rank + 1) * ((n / worldSize)+1);
    mpz_t numerator, denominator, result, iter, one;
    mpz_init_set_ui (one, (unsigned long int) 1);
    mpz_init_set_str(numerator, "1", 10);
    mpz_init_set_str(denominator, "1", 10);
    double start, finish;
    start = MPI_Wtime();
    for (int i = rank * ((n / worldSize)+1) + 1; i < upperBorder + 1; i++) {
        mpz_init_set_ui (iter, (unsigned long int) i);
        mpz_mul(numerator, numerator, iter);
        mpz_add(numerator, numerator, one);
        mpz_mul(denominator, denominator, iter);
    }
    if (rank != 0) {
        char* numStr, *denStr;
        int size = mpz_sizeinbase(numerator, 10) + 1;
        if((numStr = (char*) malloc((size)*sizeof(char)))==NULL) {
            exit(1);
        }
        mpz_get_str(numStr, 10, numerator);
        MPI_Send(&size, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        MPI_Send(numStr, size, MPI_CHAR, 0, 1, MPI_COMM_WORLD);
        free(numStr);
        size = mpz_sizeinbase(denominator, 10) + 1;
        if((denStr = (char*) malloc((size)*sizeof(char))) == NULL) {
            exit(1);
        }
        mpz_get_str(denStr, 10, denominator);
        MPI_Send(&size, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        MPI_Send(denStr, size, MPI_CHAR, 0, 1, MPI_COMM_WORLD);
        free(denStr);
    } else {
        mpz_t nextNumerator, nextDenominator;
        char *nextNumeratorStr, *nextDenominatorStr;
        int size;
        for (int i = 1; i < worldSize; i++) {    
            MPI_Recv(&size, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            nextNumeratorStr = (char*) malloc((size)*sizeof(char));
            MPI_Recv(nextNumeratorStr, size, MPI_CHAR, i, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Recv(&size, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            nextDenominatorStr = (char*) malloc((size)*sizeof(char));
            MPI_Recv(nextDenominatorStr, size, MPI_CHAR, i, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            mpz_init_set_str(nextNumerator, nextNumeratorStr, 10);
            mpz_init_set_str(nextDenominator, nextDenominatorStr, 10);
            mpz_mul(numerator, numerator, nextDenominator);
            mpz_add(numerator, numerator, nextNumerator);
            mpz_mul(denominator, denominator, nextDenominator);
        }
        mpf_t numeratorFloat, denominatorFloat, answer;
        mpf_init2(numeratorFloat, n);
        mpf_init2(denominatorFloat, n);
        mpf_init2(answer, n);
        mpf_set_z (numeratorFloat, numerator);
        mpf_set_z (denominatorFloat, denominator);
        mpf_div (answer, numeratorFloat, denominatorFloat);
        finish = MPI_Wtime();
        gmp_printf("e = %.Ff\n", answer);//need to be changed somehow from variable (up to 19969)
        
        printf("estimated time: %lf\n", (finish - start)*1000);
    }

    MPI_Finalize();
}
