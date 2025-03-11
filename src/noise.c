#include "../include/noise.h"

#include <stdlib.h>
#include <stdint.h>

// Constants for 2D OpenSimplexNoise
#define STRETCH_CONSTANT_2D (-0.211324865405187) // (1/sqrt(2+1)-1)/2
#define SQUISH_CONSTANT_2D  (0.366025403784439)  // (sqrt(2+1)-1)/2
#define NORM_CONSTANT_2D    47.0

// Gradient table for 2D
static const int8_t gradients2D[] = {
    5,  2,   2,  5,
   -5,  2,  -2,  5,
    5, -2,   2, -5,
   -5, -2,  -2, -5,
};

// Fast floor function
static int fast_floor(double x) {
    int xi = (int)x;
    return x < xi ? xi - 1 : xi;
}

// Dot product for 2D gradients
static double dot(const int8_t* g, double x, double y) {
    return g[0] * x + g[1] * y;
}

// Initialize the noise context
OpenSimplexNoise* init_open_simplex(int64_t seed) {
    OpenSimplexNoise* noise = (OpenSimplexNoise*)malloc(sizeof(OpenSimplexNoise));
    noise->seed = seed;

    // Initialize permutation tables
    uint8_t source[256];
    for (int i = 0; i < 256; i++) {
        source[i] = (uint8_t)i;
    }

    // Simple seed-based shuffle (not cryptographically secure, but sufficient for noise)
    for (int i = 255; i >= 0; i--) {
        seed = seed * 6364136223846793005LL + 1442695040888963407LL;
        int r = (int)((seed & 0xFF) % (i + 1));
        noise->perm[i] = source[r];
        noise->permGradIndex[i] = (int16_t)(noise->perm[i] % (sizeof(gradients2D) / 2)) * 2;
        source[r] = source[i];
    }

    return noise;
}

// Free the noise context
void osn_free(OpenSimplexNoise* noise) {
    free(noise);
}

// 2D OpenSimplexNoise function
double osn_noise_2d(OpenSimplexNoise* noise, double x, double y) {
    // Place input coordinates onto grid
    double stretchOffset = (x + y) * STRETCH_CONSTANT_2D;
    double xs = x + stretchOffset;
    double ys = y + stretchOffset;

    // Floor to get grid coordinates of rhombus (stretched square)
    int xsb = fast_floor(xs);
    int ysb = fast_floor(ys);

    // Skew back to get distances
    double squishOffset = (xsb + ysb) * SQUISH_CONSTANT_2D;
    double xb = xsb + squishOffset;
    double yb = ysb + squishOffset;

    // Compute grid distances
    double xins = xs - xsb;
    double yins = ys - ysb;

    // Sum coordinates to determine which region we're in
    double inSum = xins + yins;

    // Positions relative to origin point
    double dx0 = x - xb;
    double dy0 = y - yb;

    double value = 0.0;

    // Contribution (1,0)
    double dx1 = dx0 - 1.0 - SQUISH_CONSTANT_2D;
    double dy1 = dy0 - 0.0 - SQUISH_CONSTANT_2D;
    double attn1 = 2.0 - dx1 * dx1 - dy1 * dy1;
    if (attn1 > 0) {
        attn1 *= attn1;
        int i = noise->permGradIndex[noise->perm[(xsb + 1) & 0xFF] ^ (ysb & 0xFF)];
        value += attn1 * attn1 * dot(gradients2D + i, dx1, dy1);
    }

    // Contribution (0,1)
    double dx2 = dx0 - 0.0 - SQUISH_CONSTANT_2D;
    double dy2 = dy0 - 1.0 - SQUISH_CONSTANT_2D;
    double attn2 = 2.0 - dx2 * dx2 - dy2 * dy2;
    if (attn2 > 0) {
        attn2 *= attn2;
        int i = noise->permGradIndex[noise->perm[xsb & 0xFF] ^ ((ysb + 1) & 0xFF)];
        value += attn2 * attn2 * dot(gradients2D + i, dx2, dy2);
    }

    // Contribution (0,0)
    double attn0 = 2.0 - dx0 * dx0 - dy0 * dy0;
    if (attn0 > 0) {
        attn0 *= attn0;
        int i = noise->permGradIndex[noise->perm[xsb & 0xFF] ^ (ysb & 0xFF)];
        value += attn0 * attn0 * dot(gradients2D + i, dx0, dy0);
    }

    // Extra vertex if in specific region
    if (inSum <= 1.0) { // We're inside the triangle (0,0) -> (1,0) -> (0,1)
        double zins = 1.0 - inSum;
        if (zins > xins || zins > yins) {
            double dx3, dy3;
            if (xins > yins) {
                dx3 = dx0 - 1.0 + 2.0 * SQUISH_CONSTANT_2D;
                dy3 = dy0 - 0.0 + 2.0 * SQUISH_CONSTANT_2D;
            } else {
                dx3 = dx0 - 0.0 + 2.0 * SQUISH_CONSTANT_2D;
                dy3 = dy0 - 1.0 + 2.0 * SQUISH_CONSTANT_2D;
            }
            double attn3 = 2.0 - dx3 * dx3 - dy3 * dy3;
            if (attn3 > 0) {
                attn3 *= attn3;
                int i = noise->permGradIndex[noise->perm[(xsb + 1) & 0xFF] ^ ((ysb + 1) & 0xFF)];
                value += attn3 * attn3 * dot(gradients2D + i, dx3, dy3);
            }
        }
    } else { // We're inside the triangle (1,0) -> (0,1) -> (1,1)
        double zins = 2.0 - inSum;
        if (zins < xins || zins < yins) {
            double dx3, dy3;
            if (xins > yins) {
                dx3 = dx0 - 1.0 + 2.0 * SQUISH_CONSTANT_2D;
                dy3 = dy0 - 0.0 + 2.0 * SQUISH_CONSTANT_2D;
            } else {
                dx3 = dx0 - 0.0 + 2.0 * SQUISH_CONSTANT_2D;
                dy3 = dy0 - 1.0 + 2.0 * SQUISH_CONSTANT_2D;
            }
            double attn3 = 2.0 - dx3 * dx3 - dy3 * dy3;
            if (attn3 > 0) {
                attn3 *= attn3;
                int i = noise->permGradIndex[noise->perm[(xsb + 1) & 0xFF] ^ ((ysb + 1) & 0xFF)];
                value += attn3 * attn3 * dot(gradients2D + i, dx3, dy3);
            }
        }
    }

    // Normalize the result
    return value / NORM_CONSTANT_2D;
}
