#pragma once

#include <cstdint>
#include <cmath>

namespace vm
{
    class floating_point
    {
    public:
        static float add(float a, float b);
        static float sub(float a, float b);
        static float mul(float a, float b);
        static float div(float a, float b);
        static float sqrt(float a);
        static float sin(float a);
        static float cos(float a);
        static float tan(float a);
        static float asin(float a);
        static float acos(float a);
        static float atan(float a);
        static float atan2(float y, float x);
        static float exp(float a);
        static float log(float a);
        static float log10(float a);
        static float pow(float a, float b);
        static float ceil(float a);
        static float floor(float a);
        static float round(float a);
        static float trunc(float a);
        static float fmod(float a, float b);
        static float remainder(float a, float b);
        static float copysign(float a, float b);
        static float nextafter(float a, float b);
        static float fmin(float a, float b);
        static float fmax(float a, float b);
        static float fdim(float a, float b);
        static float fma(float a, float b, float c);
        static bool isfinite(float a);
        static bool isinf(float a);
        static bool isnan(float a);
        static bool isnormal(float a);
        static bool signbit(float a);
        static int fpclassify(float a);
        static float abs(float a);
        static float scalbn(float a, int n);
        static float ldexp(float a, int exp);
        static float frexp(float a, int* exp);
        static float modf(float a, float* iptr);
        static float hypot(float a, float b);
        static float erf(float a);
        static float erfc(float a);
        static float tgamma(float a);
        static float lgamma(float a);
    };
} 