#include <stdio.h> 
#include <stdlib.h> 
#include <time.h> 
 
#define PI 3.1415926535897 
 
double CalcSin(double x, long long n){ 
    double sinx = 0; 
    x = x * PI / 180; 
    double sum = x; 
    for (long long i = 1; i <= 2 * n - 1; i += 2){ 
        sinx += sum; 
        sum = (sum * x * x * (-1)) / ( (i + 1) * (i + 2)) ; 
    } 
    return sinx; 
} 
 
int main(int argc, char **argv){ 
    struct timespec start, end; 
    clock_gettime (CLOCK_MONOTONIC_RAW, &start); 
    if (argc == 1){ 
        printf("Bad input. Enter x and n in command line"); 
                return 0; 
    } 
    double x = atof(argv[1]); 
    long long n = atoll(argv[2]); 
 
    double sinx = CalcSin(x, n); 
    printf("%lf\n", sinx); 
     
    clock_gettime(CLOCK_MONOTONIC_RAW, &end); 
    printf("Time taken: %lf sec.\n", end.tv_sec-start.tv_sec 
                + 0.000000001*(end.tv_nsec-start.tv_nsec)); 
    return 0; 
}

