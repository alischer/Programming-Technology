#include "inverse_matrix.h"
#include "matrix_enter.h"
#include <chrono>
#include <iostream>
#include <math.h>
#include <pthread.h>
#include <stdlib.h>
#define DECIMAL 10
#define GIGA_MODIFIER 1e9
#define KILO_MODIFIER 1e3
#define MICRO_MODIFIER 1e-6
#define M_FIVE (-5)
#define BORD1 4
#define BORD2 6
#define FIVE 5

using namespace std;

unsigned long long currentTimeNano()
{
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return (long long)(t.tv_sec * GIGA_MODIFIER + t.tv_nsec);
}

unsigned long long currentTimeMillis()
{
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return (long long)(t.tv_sec * KILO_MODIFIER + t.tv_nsec * MICRO_MODIFIER);
}

void print_matrix(int m, int n, const double *A)
{
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < m; j++) {
            printf("%10.3e", A[j * n + i]);
        }
        printf("\n");
    }
}

void print_matrix_inv(int m, int n, const double *X, const int *inv_permutation)
{
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < m; j++) {
            printf("%10.3e", X[inv_permutation[i] + j * n]);
        }
        printf("\n");
    }
}

double calc(int n, const double *A, const double *X, const int *inv_permutation)
{
    double error = 0;
    for (int i = 0; i < n; i++) {
        double sum = 0;
        for (int j = 0; j < n; j++) {
            double multpl = 0.0;
            for (int k = 0; k < n; k++) {
                multpl += A[k * n + i] * X[j * n + inv_permutation[k]];
            }
            if (i == j) {
                sum += fabs(multpl - 1.0);
            } else {
                sum += fabs(multpl);
            }
        }
        error = max(error, sum);
    }
    return error;
}

int main(int argc, char *argv[])
{
    pthread_barrier_t barrier;
    int n;
    int m;
    int k;
    int p;
    string filename;
    char *endptr;
    if (argc < BORD1 || argc > BORD2) {
        return -1;
    }
    p = (int)strtol(argv[1], &endptr, DECIMAL);
    if (endptr == argv[1]) {
        return -1;
    }
    n = (int)strtol(argv[2], &endptr, DECIMAL);
    if (endptr == argv[2]) {
        return -1;
    }
    m = (int)strtol(argv[3], &endptr, DECIMAL);
    if (endptr == argv[3]) {
        return -1;
    }
    k = (int)strtol(argv[4], &endptr, DECIMAL);
    if (endptr == argv[4]) {
        return -1;
    }
    if (n == 0 || m == 0 || m > n || p <= 0) {
        cout << "Incorrect arguments" << endl;
        return -1;
    }
    if (k > 4) {
        return -1;
    }
    if (k == 0) {
        if (argv[FIVE] == NULL) {
            return -1;
        }
    }
    double *A = NULL;
    double *X = NULL;
    int *permutation = NULL;
    int *inv_permutation = NULL;
    ARGS *args = NULL;
    double *tmp_common_col_main = NULL;
    pthread_t *threads = NULL;
    unsigned long long time = 0;
    unsigned long long *time1 = NULL;
    int check = 1;
    try {
        A = new double[n * n];
        args = new ARGS[p];
        tmp_common_col_main = new double[2 * p * n];
        X = new double[n * n];
        permutation = new int[n];
        threads = new pthread_t[p];
        inv_permutation = new int[n];
        time1 = new unsigned long long[p];
    } catch (const std::bad_alloc &) {
        return -2;
    }
    int flag = matrix_enter(n, k, argv[FIVE], A);
    if (flag == 0) {
        delete[] A;
        delete[] args;
        delete[] tmp_common_col_main;
        delete[] X;
        delete[] permutation;
        delete[] threads;
        delete[] inv_permutation;
        delete[] time1;
        return -3;
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i == j) {
                X[i * n + j] = 1;
            } else {
                X[i * n + j] = 0;
            }
        }
        permutation[i] = i;
    }
    pthread_barrier_init(&barrier, NULL, p);
    time = currentTimeNano();
    for (int i = 0; i < p; i++) {
        args[i].check = &check;
        args[i].matrix = A;
        args[i].result = X;
        args[i].perm = permutation;
        args[i].n = n;
        args[i].thread_num = i;
        args[i].total_threads = p;
        args[i].barrier = &barrier;
        args[i].tmp_common_col = tmp_common_col_main;
        time1[i] = currentTimeNano();
        if (pthread_create(threads + i, NULL, inverse_matrix,
                           (void *)(args + i)) != 0) {
            cout << "Too many threads have been created" << endl;
            delete[] A;
            delete[] X;
            delete[] args;
            delete[] tmp_common_col_main;
            delete[] permutation;
            delete[] threads;
            delete[] inv_permutation;
            delete[] time1;
            pthread_barrier_destroy(&barrier);
            return M_FIVE;
        }
    }
    for (int i = 0; i < p; i++) {
        pthread_join(threads[i], NULL);
        time1[i] = currentTimeNano() - time1[i];
        cout << "Time of thread " << i << ": " << time1[i] << endl;
    }
    time = currentTimeNano() - time;
    delete[] time1;
    delete[] tmp_common_col_main;
    delete[] threads;
    delete[] args;
    pthread_barrier_destroy(&barrier);
    if (check == 0) {
        cout << "Matrix is not invertible";
        delete[] A;
        delete[] X;
        delete[] permutation;
        delete[] inv_permutation;
        return -4;
    }
    for (int i = 0; i < n; i++) {
        inv_permutation[permutation[i]] = i;
    }
    delete[] permutation;
    flag = matrix_enter(n, k, argv[FIVE], A);
    print_matrix(m, n, A);
    printf("Inverse:\n");
    print_matrix_inv(m, n, X, inv_permutation);
    printf("Time: %llu ns\n", time);
    printf("Residual:%10.3e\n", calc(n, A, X, inv_permutation));
    delete[] A;
    delete[] X;
    delete[] inv_permutation;
    return 0;
}
