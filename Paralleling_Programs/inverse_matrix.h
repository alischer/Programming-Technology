#include <pthread.h>
typedef struct {
    int *check;
    double *matrix;
    double *result;
    int *perm;
    int n;
    int thread_num;
    int total_threads;
    pthread_barrier_t *barrier;
    double *tmp_common_col;
    double *sum;
    double *error;
} ARGS;
unsigned long long currentTimeNano();
unsigned long long currentTimeMillis();
void permutate(int n, int diag, double **B, int *permutation);
void *inverse_matrix(void *args);
