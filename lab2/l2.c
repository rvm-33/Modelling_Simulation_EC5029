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
    int capacity = (int)(lambda * T * 2.0f + 100);

    float *arr = malloc(sizeof(float) * capacity);

    if (arr == NULL) {
        return NULL;
    }

    float t = 1.0f;
    int I = 0;

    while (t < T) {

        float r = ran_gen(a, m);

        t = t - (1.0f / lambda) * logf(r);

        if (t >= T)
            break;

        arr[I] = t;
        I++;
    }

    *num_events = I;

    return arr;
}

int main()
{
    uint64_t a = 16807;
    uint64_t m = (1U << 31) - 1;

    int T = 10000;
    int lambda = 2;

    int num_events;

    float *arr = poisson_gen(
        a,
        m,
        T,
        lambda,
        &num_events
    );

    if (arr == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }

    /* Open CSV file */
    FILE *fp = fopen("poisson_data.csv", "w");

    if (fp == NULL) {
        printf("Error opening CSV file\n");
        free(arr);
        return 1;
    }

    /* CSV header */
    fprintf(fp, "Event,Time\n");

    /* Write event times to CSV */
    for (int i = 0; i < num_events; i++) {

        //printf("S(%d) = %f\n", i + 1, arr[i]);

        fprintf(
            fp,
            "%d,%.6f\n",
            i + 1,
            arr[i]
        );
    }

    fclose(fp);
    free(arr);

    printf("\nNumber of events = %d\n", num_events);
    printf("Data saved to poisson_data.csv\n");

    return 0;
}