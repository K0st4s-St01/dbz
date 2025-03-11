#ifndef NOISE_H
#define NOISE_H

typedef struct{
    int perm[256];
    int permGradIndex[256];
    int seed;
}OpenSimplexNoise;

OpenSimplexNoise* osn_init(int seed);
void osn_free(OpenSimplexNoise* noise);
double osn_noise_2d(OpenSimplexNoise* noise,double x,double y);
#endif