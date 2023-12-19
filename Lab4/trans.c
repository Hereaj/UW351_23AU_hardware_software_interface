/*
 * CSE 351 Lab 4 (Caches and Cache-Friendly Code)
 * Part 2 - Optimizing Matrix Transpose
 *
 * Name(s):  Kevin Kuk, Jaehyeon Ahn
 * NetID(s): 2377312, 2377324
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[M][N], int B[N][M]);
 * and compute B = A^T.
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1 KiB direct mapped cache with a block size of 32 bytes.
 */

#include <stdio.h>
#include "support/cachelab.h"

int is_transpose(int M, int N, int A[M][N], int B[N][M]);


/*
 * transpose_submit - This is the transpose function that you will be graded
 *     on. Do not change the description string "Transpose submission", as the
 *     driver searches for that string to identify the transpose function to be
 *     graded.
 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[M][N], int B[N][M]) {
    if (M == 32) {
        int index_M, index_N, index_switch;
        int v0, v1, v2, v3, v4, v5, v6, v7;

        for (index_M = 0; index_M < M; index_M += 8) {
            for (index_N = 0; index_N < N; index_N += 8) {
                for (index_switch = index_M; index_switch < index_M + 8; index_switch++) {

                    v0 = A[index_switch][index_N];
                    v1 = A[index_switch][index_N + 1];
                    v2 = A[index_switch][index_N + 2];
                    v3 = A[index_switch][index_N + 3];
                    v4 = A[index_switch][index_N + 4];
                    v5 = A[index_switch][index_N + 5];
                    v6 = A[index_switch][index_N + 6];
                    v7 = A[index_switch][index_N + 7];
                    
                    B[index_N][index_switch] = v0;
                    B[index_N + 1][index_switch] = v1;
                    B[index_N + 2][index_switch] = v2;
                    B[index_N + 3][index_switch] = v3;
                    B[index_N + 4][index_switch] = v4;
                    B[index_N + 5][index_switch] = v5;
                    B[index_N + 6][index_switch] = v6;
                    B[index_N + 7][index_switch] = v7;

                }
            }
        }
    }

    else if (M == 64) {
        int index_M, index_N, index_switch;
        int v0, v1, v2, v3;

        for (index_M = 0; index_M < M; index_M += 4) {
            for (index_N = 0; index_N < N; index_N += 4) {
                for (index_switch = index_M; index_switch < index_M + 4; index_switch++ ){

                    v0 = A[index_switch][index_N];
                    v1 = A[index_switch][index_N + 1];
                    v2 = A[index_switch][index_N + 2];
                    v3 = A[index_switch][index_N + 3];

                    B[index_N][index_switch] = v0;
                    B[index_N + 1][index_switch] = v1;
                    B[index_N + 3][index_switch] = v3;
                    B[index_N + 2][index_switch] = v2;
                    
                }
            }
        }
    }
}


// You can define additional transpose functions below. We've defined a simple
// one below to help you get started.

/*
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[M][N], int B[N][M]) {
    int i, j, tmp;

    for (i = 0; i < M; i++) {
        for (j = 0; j < N; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }

}


/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions() {
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc);

    /* Register any additional transpose functions */
    registerTransFunction(trans, trans_desc);

}


/*
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[M][N], int B[N][M]) {
    int i, j;

    for (i = 0; i < M; i++) {
        for (j = 0; j < N; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}
