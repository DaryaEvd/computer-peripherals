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
    float maxRow = 0;
    for (int i = 0; i < n; i++) {
        float tmp = 0;
        for (int j = 0; j < n; j++) {
            tmp += fabs(matrix[i * n + j]);
        }
        if (tmp > maxRow) {
            maxRow = tmp;
        }
    }
    return maxRow;
}

float CountMaxColumnSum(float *matrix){
    float maxColumn = 0;
    for (int i = 0; i < n; i++) {
        float tmp = 0;
        for (int j = 0; j < n; j++) {
            tmp += fabs(matrix[j * n + i]);
        }
        if (tmp > maxColumn) {
            maxColumn = tmp;
        }
    }
    return maxColumn;
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
    for (int i = 0; i < n * n ; i++) {
            store[i] += matrix[i]; 
        }
}

void MultMatrixOnScalar(float *matrix, float scalar){
   for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            matrix[i * n + j] = matrix[i * n + j] * scalar;
        }
    }
}

void MultMatrixes(float *matrix1, float *matrix2, float *result) {
    for (int i = 0; i < n; i++) {
        for (int k = 0; k < n; k++) {
            for (int j = 0; j < n; j++) {
                result[i * n + j] += matrix1[i * n + k] * matrix2[k * n + j];
            }
        }
    }
}

void InverseMatrix(float *A) {
    float *B = (float*) calloc(n * n, sizeof(float));
    TransponateMatrix(B, A);
    
    float maxRow = CountMaxRowSum(A);

    float maxColumn = CountMaxColumnSum(A);

    float scalar = 1 / (maxColumn * maxRow);

    float *I = (float*) calloc(n * n, sizeof(float)); 
    IdentityMatrix(I);    
    
    MultMatrixOnScalar(B, scalar);

    float *R = (float*) calloc(n * n, sizeof(float));
    MultMatrixes(B, A, R); // R contains B * A
    SubMatrixes(I, R, R); // R contatins I - BA

    float  *temp = (float*) calloc(n * n, sizeof(float));
    memcpy(temp, R, sizeof(float) * n * n); //tmp contains R
    for (int iter = 0; iter < m; iter++) {
        SumMatrixes(temp, I); //I + R + R^2 + R^3 + ...
        memset(A, 0, sizeof(float) * n * n); //A consist of zeroes
        
        MultMatrixes(temp, R, A); //A contains R, R^2, R^3, R^4, ...
        memcpy(temp, A, sizeof(float) * n * n); //temp contains A, i.e R, R^2, R^3, R^4, ...
    }
    memset(A, 0, sizeof(float) * n * n); //A consist of zeroes
    MultMatrixes(I, B, A); // A consist of (sum it bractets, i.e I) * B

    free(temp);
    free(I);
    free(B);
    free(R);
}

int main() {
    float *A = (float*) calloc(n * n, sizeof(float));;
    FillMatrix(A);

    printf("n = %d\n", n);
    printf("m = %d\n", m);

    printf("Start matrix:\n");
    PrintMatrix(A);

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC_RAW, &start);

    InverseMatrix(A);
    clock_gettime(CLOCK_MONOTONIC_RAW, &end);
    
    printf("Result matrix:\n");
    PrintMatrix(A);

    printf("Time taken: %lf sec.\n", end.tv_sec - start.tv_sec + 
                            0.000000001*(end.tv_nsec-start.tv_nsec));

    free(A);
}
