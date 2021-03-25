#include "matrix_enter.h"
#include <algorithm>
#include <iostream>
#include <math.h>
#include <stdio.h>

using namespace std;

double f(int k, int n, int i, int j)
{
    double num = 0;
    if (k == 1) {
        num = n - max(i, j) + 1;
    } else if (k == 2) {
        num = max(i, j);
    } else if (k == 3) {
        num = abs(i - j);
    } else if (k == 4) {
        num = 1.0 / (i + j - 1);
    }
    return num;
}

int matrix_enter(int n, int k, char *str, double *A)
{
    if (k == 0) {
        FILE *file;
        file = fopen(str, "r");
        if (file == 0) {
            return 0;
        }
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                int s = fscanf(file, "%lf", &A[j * n + i]);
                if (s == 0 || s == EOF) {
                    return 0;
                }
            }
        }
        fclose(file);
    } else {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                A[j * n + i] = f(k, n, i + 1, j + 1);
            }
        }
    }
    return 1;
}
