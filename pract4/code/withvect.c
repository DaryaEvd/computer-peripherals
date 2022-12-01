#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <xmmintrin.h>

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

void TransponateMatrix(float *transpA, float *A){
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            transpA[i * n + j] = A[i * n + j];
        }
    }
} 

void SubMatrixes(float *matrix1, float *matrix2, float *res){
    //fill values from matrix1 by aligned address
    __m128 maxtr1mm = _mm_load_ps(matrix1);

    //fill values from matrix2 by aligned address
    __m128 maxtr2mm = _mm_load_ps(matrix2);

    //sub values
    __m128 subMatr = _mm_sub_ps(maxtr1mm, maxtr2mm);

    //write values in res by aligned address
    _mm_store_ps(res, subMatr);
}

void SumMatrixes(float *matrix, float *store){
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j += 4){
            //fill values from matrix by aligned address
            __m128 matrixLine = _mm_load_ps(matrix + i * n + j);

            //load values from store by aligned address
            __m128 storeLine = _mm_load_ps(store + i * n + j);

            //sum matrixes
            __m128 tmpSum = _mm_add_ps(matrixLine, storeLine);

            //write values in store by aligned address
            _mm_store_ps(store + i * n + j, tmpSum);
        }
    }
}

void MultMatrixOnScalar(float *matrix, float scalar){
    //fill every vector component by number we wanna ultiply
    __m128 number = _mm_set1_ps(scalar);

    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j += 4){
            //fill values from matrix by aligned address
            __m128 line = _mm_load_ps(matrix + i * n + j);

            //multiply line on number and get partial sums
            __m128 result = _mm_mul_ps(line, number);

            //write  values by aligned address
            _mm_store_ps(matrix + i * n + j, result);
        }
    }
}

void MultMatrixes(float *matrix1, float *matrix2, float *result) {
    for (int i = 0; i < n; i++){
        for (int k = 0; k < n; k+=4){
            __m128 sum = _mm_setzero_ps(); //zerofy values
            for (int j = 0; j < n; j++){                
                //fill every vector component by value from the maxColumn of matrix1
                __m128 line1 = _mm_set1_ps(matrix1[i * n + j]);
                
                //load a line from matrix2
                __m128 line2 = _mm_load_ps(matrix2 + j * n + k);

                //temporary multiplication of temp values
                __m128 tmpMult = _mm_mul_ps(line1, line2);

                //tmp sum
                sum = _mm_add_ps(sum, tmpMult);
            }
            //write values in matrix result by aligned address
            _mm_store_ps(result + i * n + k, sum);
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
    
    for (int i = 0; i < n * n; i+=4) {
        SubMatrixes(I + i, R + i, R + i); // R contatins I - BA
    }

    float  *temp = (float*) calloc(n * n, sizeof(float));
    memcpy(temp, R, sizeof(float) * n * n); //tmp contains R
    for (int iter = 0; iter < m; iter++) {
        SumMatrixes(temp, I);
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
