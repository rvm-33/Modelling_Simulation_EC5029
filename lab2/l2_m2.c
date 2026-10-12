#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>

#define x0 193

int x = x0;

float ran_gen(int a, int m)
{
    x = (int)(((long long)x * a) % m);
    return (float)x / (float)m;
}

float *poisson_gen(int a, int m, int T, int lambda, int *num_events)
{
    float L = expf(-lambda * T);
    float product = 1.0f;
    int N = 0;

    do {
        float U = ran_gen(a, m);
        product *= U;
        N++;
    } while (product > L);

    N--;
    *num_events = N;

    if (N <= 0)
        return NULL;

    float *arr = malloc(sizeof(float) * N);

    if (arr == NULL)
        return NULL;

    for (int i = 0; i < N; i++)
        arr[i] = T * ran_gen(a, m);

    for (int i = 0; i < N - 1; i++) {
        for (int j = i + 1; j < N; j++) {
            if (arr[i] > arr[j]) {
                float temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }
    }

    return arr;
}

int main()
{
    int a = 16807;
    int m = (1U << 31) - 1;

    int T = 10000;
    int lambda = 2;
    int num_events;

    float *arr = poisson_gen(a, m, T, lambda, &num_events);

    if (arr == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }

    FILE *fp = fopen("poisson_data2.csv", "w");

    if (fp == NULL) {
        printf("Error opening CSV file\n");
        free(arr);
        return 1;
    }

    fprintf(fp, "Event,Time\n");

    for (int i = 0; i < num_events; i++)
        fprintf(fp, "%d,%.6f\n", i + 1, arr[i]);

    fclose(fp);
    free(arr);

    printf("Number of events = %d\n", num_events);
    printf("Data saved to poisson_data.csv\n");

    return 0;
}