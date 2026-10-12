#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define x0 193

int compare_floats(const void *a, const void *b)
{
    float fa = *(const float *)a;
    float fb = *(const float *)b;
    if (fa < fb) return -1;
    if (fa > fb) return 1;
    return 0;
}

/*sort arr

for each DISTINCT value x:

       true_cdf = x

       empirical_cdf = (# samples <= x) / N

       difference = abs(empirical_cdf - true_cdf)

       Dn = max(Dn, difference)
       */

double ks_test(float *arr, int N)
{      double Dn = 0.0;
    for (int i = 0; i < N; i++) {
        if (i == N - 1 || arr[i] != arr[i + 1]) {
            double x = arr[i];
            double true_cdf = x;
            double empirical_cdf = (double)(i + 1) / N;
            double difference = fabs(empirical_cdf - true_cdf);
            if (difference > Dn)
                Dn = difference;
        }
    }
    return Dn;
}

int x = x0;
float *ran_gen(int a, int m, int N) {
    float *arr = malloc(sizeof(float) * N);
    //int * count=malloc(sizeof(int)*m);
    for (int i = 0; i < N; i++) {
        x = (int)(((long long)x * a) % m);
        //count[x]++;
        arr[i] = (float)x / (float)m;
    }
    return arr;
}

int main()
{
    uint64_t a = 16807;
    uint64_t m = (1U << 31) - 1;

    int N = 1000;

    int Dn_Samples = 10000;

    /* Open CSV file */
    FILE *fp = fopen("Dn_data.csv", "w");

    if (fp == NULL) {
        printf("Error opening CSV file\n");
        return 1;
    }
    fprintf(fp, "experiment,Dn\n");
    for (int i = 0; i < Dn_Samples; i++) {
        /* Generate random numbers */
        float *arr = ran_gen(a, m, N);
        /* Sort the array */
        qsort(arr, N, sizeof(float), compare_floats);
        /* Calculate KS statistic */
        float dn = ks_test(arr, N);
        /* Write Dn to CSV */
        fprintf(fp, "%d,%.10f\n", i + 1, dn);
        printf("Dn: %f\n", dn);
        /* Free allocated memory */
        free(arr);
    }

    /* Close CSV */
    fclose(fp);
    printf("\nData saved to Dn_data.csv\n");
    return 0;
}
