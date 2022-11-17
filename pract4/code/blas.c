#include <cblas.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define n 2048
#define m 10

void FillMatrix(float *A){
    srand(time(NULL));
    for (int i = 0; i < n * n; i++) {
        A[i] = rand() % 100;
    }
}

void PrintMatrix(float *matrix) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%.9f\t", matrix[i * n + j]);
        }
        printf("\n");
    }
    printf("===============================\n");
}

void IdentityMatrix(float *matrix) {
    for (int i = 0; i < n; i++) {
        matrix[i * n + i] = 1;
    }
}

float CountMaxRowSum(float *matrix){
    float *tmpSum = (float *) calloc(n, sizeof(float));
    for (int i = 0; i < n; i++){
        /*
        @brief counts sum of absolute values in vector
        @param n - number of elements in vector
        @param matrix + n * i - array
        @param 1 - the increment for indexing vector
        */
        tmpSum[i] = cblas_sasum(n, matrix + n * i, 1);
    }
    /*
        @brief returns thr position that has the largest 
                absolute value in vector
        @param n - number of elements in vector
        @param matrix + n * i - array
        @param 1 - the increment for indexing vector
    */
    float maxSumRow = tmpSum[cblas_isamax(n, tmpSum, 1)];

    free (tmpSum);
    return maxSumRow;
}

float CountMaxColumnSum(float *matrix){
    float *tmpSum = (float *) calloc(n, sizeof(float));

    for (int i = 0; i < n; i++){
        tmpSum[i] = cblas_sasum(n, matrix + i, n);
    }
    float result = tmpSum[cblas_isamax(n, tmpSum, 1)];

    free (tmpSum);
    return result;
}

void TransponateMatrix(float *transpMatr, float *matr){
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            transpMatr[i * n + j] = matr[i * n + j];
        }
    }
} 

float SubMatrixes(float *matrix1, float *matrix2, float *res){
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++){
            res[i * n + j] = matrix1[i * n + j] - matrix2[i * n + j];
        }
    }
}

void SumMatrixes(float *matrix, float *store){
    /*
        @brief contains the sum of matrixes
        @param n * n - number of elements in vectors
        @param 1 - scalar
        @param matrix - array
        @param 1 - the increment for the elements of matrix
        @param store - array
        @param 1 - the increment for the elements of store
    */
    cblas_saxpy(n * n, 1, matrix, 1, store, 1);
}

void MultMatrixOnScalar(float *matrix, float scalar){
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            matrix[i * n + j] = matrix[i * n + j] * scalar;
        }
    }
}

void MultMatrixes(float *matrix1, float *matrix2, float *result) {
    /*
        @brief C := alpha*op(A)*op(B) + beta*C
        @param CblasRowMajor - store matrix on raws
        @param CblasNoTrans - no operations for matrix1
        @param CblasNoTrans - no operations for matrix2
        @param n, n, n - sizes of matrixes
        @param 1 - alpha - coeff
        @param matrix1
        @param n -  number of elements in matrix dimension
        @param matrix2
        @param n -  number of elements in matrix dimension
        @param 1 - beta - coeff
        @param result 
        @param n -  number of elements in matrix dimension
    */
    cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, 
                n, n, n,
                1, matrix1, n, 
                matrix2, n, 1, 
                result, n);
}

void InverseMatrix(float *A) {
    float *B = (float*) calloc(n * n, sizeof(float));
    TransponateMatrix(B, A);
    
    float maxRow = CountMaxRowSum(A);
  
    float maxColumn = CountMaxColumnSum(A);

    float scalar = 1 / (maxColumn * maxRow);

    float *IdentMatr = (float*) calloc(n * n, sizeof(float)); 
    IdentityMatrix(IdentMatr);    
    
    MultMatrixOnScalar(B, scalar);

    float *R = (float*) calloc(n * n, sizeof(float));
    MultMatrixes(B, A, R); // R contains B * A
    SubMatrixes(IdentMatr, R, R); // R contatins IdentMatr - BA

    float  *temp = (float*) calloc(n * n, sizeof(float));
    memcpy(temp, R, sizeof(float) * n * n); //tmp contains R
    for (int iter = 0; iter < m; iter++) {
        SumMatrixes(temp, IdentMatr); //IdentMatr + R + R^2 + R^3 + ...
        memset(A, 0, sizeof(float) * n * n); //A consist of zeroes
        
        MultMatrixes(temp, R, A); //A contains R, R^2, R^3, R^4, ...
        memcpy(temp, A, sizeof(float) * n * n); //temp contains A, i.e R, R^2, R^3, R^4, ...
    }
    memset(A, 0, sizeof(float) * n * n); //A consist of zeroes
    MultMatrixes(IdentMatr, B, A); // A consist of (sum it bractets, i.e IdentMatr) * B

    free(temp);
    free(IdentMatr);
    free(B);
    free(R);
}

int main() {
    float *A = (float*) calloc(n * n, sizeof(float));;
    FillMatrix(A);

    printf("n = %d\n", n);
    printf("m = %d\n", m);
    
    // printf("Start matrix:\n");
    // PrintMatrix(A);

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC_RAW, &start);

    InverseMatrix(A);
    clock_gettime(CLOCK_MONOTONIC_RAW, &end);

    // printf("Result matrix:\n");
    // PrintMatrix(A);

    printf("Time taken: %lf sec.\n", end.tv_sec - start.tv_sec + 
                            0.000000001*(end.tv_nsec-start.tv_nsec));
    

    free(A);
}
