#include<stdint.h>
#include <math.h> // Required for exp()


float uniform_rv(void){
    uint64_t a = 16807;
    uint64_t m = (1U << 31) - 1;
    static float x = 0;
    x = (int)(((long long)x * a) % m);
    return ((float)x/(float)m);
}

float poisson_rv(float lambda){
    float u=uniform_rv();
    int i=0;
    int F=0;
    float p=exp(-lambda);
    while(u>F){
        p=p*lambda/(i+1);
        i++;
        F=F+p;
    }
    return i;
}