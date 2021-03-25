#include "inverse_matrix.h"
#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
void permutate(int n, int diag, double *B, int *permutation)
{
    int ind_max = diag;
    for (int i = diag + 1; i < n; i++) {
        if (fabs(B[permutation[i] * n + diag]) >
            fabs(B[permutation[ind_max] * n + diag])) {
            ind_max = i;
        }
    }
    int temp = permutation[diag];
    permutation[diag] = permutation[ind_max];
    permutation[ind_max] = temp;
}

void *inverse_matrix(void *args)
{
    const double zero = 0.0000000000000000000001;
    ARGS *A = (ARGS *)args;
    int n = A->n;
    double *B = A->matrix;
    double *X = A->result;
    double *tmp_common_col = A->tmp_common_col;
    int thread_num = A->thread_num;
    int *permutation = A->perm;
    int total_threads = A->total_threads;
    int myrank = thread_num;
    pthread_barrier_t *barrier = A->barrier;
    int *check = A->check;
    int num1 = thread_num * n;
    int k;
    int j;
    int t0;
    int t1;
    double c1;
    double c2;
    double c3;
    for (int i = 0; i < n; i++) {
        if (i % total_threads == thread_num) {
            myrank += total_threads;
            permutate(n, i, B, permutation);
            int perm = permutation[i] * n;
            if (fabs(B[perm + i]) < zero) {
                *check = 0;
            }
            for (j = i; j < n; j++) {
                c1 = B[perm + j];
                for (t0 = 0; t0 < total_threads; t0++) {
                    tmp_common_col[t0 * n + j] = c1;
                }
            }
        }
        pthread_barrier_wait(barrier);
        c3 = tmp_common_col[num1 + i];
        if (fabs(c3) < zero) {
            return NULL;
        }
        if (myrank > n) {
            myrank = n;
        }

        for (k = thread_num; k < myrank; k += total_threads) {
            X[k * n + i] /= c3;
        }
        for (k = myrank; k < n; k += total_threads) {
            X[k * n + i] /= c3;
            B[permutation[k] * n + i] /= c3;
        }
        for (k = thread_num; k < myrank; k += total_threads) {
            c2 = X[k * n + i];
            for (j = i + 1; j < n; j++) {
                X[k * n + j] -= tmp_common_col[num1 + j] * c2;
            }
        }
        for (k = myrank; k < n; k += total_threads) {
            c1 = B[permutation[k] * n + i];
            c2 = X[k * n + i];
            for (j = i + 1; j < n; j++) {
                B[permutation[k] * n + j] -= tmp_common_col[num1 + j] * c1;
                X[k * n + j] -= tmp_common_col[num1 + j] * c2;
            }
        }
        pthread_barrier_wait(barrier);
    }
    for (int i = n - 1; i >= 0; i--) {
        num1 = (2 * thread_num + i % 2) * n;
        if (i % total_threads == thread_num) {
            int perm = permutation[i] * n;
            for (t1 = 0; t1 < i; t1++) {
                c1 = B[perm + t1];
                for (t0 = 0; t0 < total_threads; t0++) {
                    tmp_common_col[(2 * t0 + i % 2) * n + t1] = c1;
                }
            }
        }
        pthread_barrier_wait(barrier);
        for (j = thread_num; j < n; j += total_threads) {
            c1 = X[j * n + i];
            for (k = 0; k < i; k++) {
                X[j * n + k] -= tmp_common_col[num1 + k] * c1;
            }
        }
    }
    return NULL;
}
