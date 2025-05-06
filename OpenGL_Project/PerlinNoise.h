// Two-dimensional value noise based on Hugo Elias's description:
//   http://freespace.virgin.net/hugo.elias/models/m_perlin.htm

//Written By https://stackoverflow.com/users/4275347/michael-laszlo
//On Page https://stackoverflow.com/questions/29711668/perlin-noise-generation
#pragma once

#include <cstdio>
#include <cmath>
#include <cstdlib>

#define maxPrimeIndex 10

using namespace std;

static int primes[maxPrimeIndex][3] = {
  { 995615039, 600173719, 701464987 },
  { 831731269, 162318869, 136250887 },
  { 174329291, 946737083, 245679977 },
  { 362489573, 795918041, 350777237 },
  { 457025711, 880830799, 909678923 },
  { 787070341, 177340217, 593320781 },
  { 405493717, 291031019, 391950901 },
  { 458904767, 676625681, 424452397 },
  { 531736441, 939683957, 810651871 },
  { 997169939, 842027887, 423882827 }
};

static float Noise(int i, int x, int y) {
    int n = x + y * 57;
    n = (n << 13) ^ n;
    int a = primes[i][0], b = primes[i][1], c = primes[i][2];
    int t = (n * (n * n * a + b) + c) & 0x7fffffff;
    return 1.0f - (t) / 1073741824.0f;
}

static float SmoothedNoise(int i, int x, int y) {
    float corners = (Noise(i, x - 1, y - 1) + Noise(i, x + 1, y - 1) +
        Noise(i, x - 1, y + 1) + Noise(i, x + 1, y + 1)) / 16,
        sides = (Noise(i, x - 1, y) + Noise(i, x + 1, y) + Noise(i, x, y - 1) +
            Noise(i, x, y + 1)) / 8,
        center = Noise(i, x, y) / 4;
    return corners + sides + center;
}

static float Interpolate(float a, float b, float x) {  // cosine interpolation
    float ft = x * 3.1415927f,
        f = (1 - cos(ft)) * 0.5f;
    return  a * (1 - f) + b * f;
}

static float InterpolatedNoise(int i, float x, float y) {
    int integer_X = (int)x;
    float fractional_X = x - integer_X;
    int integer_Y = (int)y;
    float fractional_Y = y - integer_Y;

    float v1 = SmoothedNoise(i, integer_X, integer_Y),
        v2 = SmoothedNoise(i, integer_X + 1, integer_Y),
        v3 = SmoothedNoise(i, integer_X, integer_Y + 1),
        v4 = SmoothedNoise(i, integer_X + 1, integer_Y + 1),
        i1 = Interpolate(v1, v2, fractional_X),
        i2 = Interpolate(v3, v4, fractional_X);
    return Interpolate(i1, i2, fractional_Y);
}

static float ValueNoise_2D(float x, float y) {
    int numX = 512,
        numY = 512,
        numOctaves = 7;
    float persistence = 0.5;

    int primeIndex = 0;


    float total = 0,
        frequency = (float)pow(2, numOctaves),
        amplitude = 1;
    for (int i = 0; i < numOctaves; ++i) {
        frequency /= 2;
        amplitude *= persistence;
        total += InterpolatedNoise((primeIndex + i) % maxPrimeIndex,
            x / frequency, y / frequency) * amplitude;
    }
    return total / frequency;
}