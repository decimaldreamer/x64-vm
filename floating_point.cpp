#include "floating_point.hpp"

namespace vm
{
    float floating_point::add(float a, float b)
    {
        return a + b;
    }

    float floating_point::sub(float a, float b)
    {
        return a - b;
    }

    float floating_point::mul(float a, float b)
    {
        return a * b;
    }

    float floating_point::div(float a, float b)
    {
        return a / b;
    }

    float floating_point::sqrt(float a)
    {
        return std::sqrt(a);
    }

    float floating_point::sin(float a)
    {
        return std::sin(a);
    }

    float floating_point::cos(float a)
    {
        return std::cos(a);
    }

    float floating_point::tan(float a)
    {
        return std::tan(a);
    }

    float floating_point::asin(float a)
    {
        return std::asin(a);
    }

    float floating_point::acos(float a)
    {
        return std::acos(a);
    }

    float floating_point::atan(float a)
    {
        return std::atan(a);
    }

    float floating_point::atan2(float y, float x)
    {
        return std::atan2(y, x);
    }

    float floating_point::exp(float a)
    {
        return std::exp(a);
    }

    float floating_point::log(float a)
    {
        return std::log(a);
    }

    float floating_point::log10(float a)
    {
        return std::log10(a);
    }

    float floating_point::pow(float a, float b)
    {
        return std::pow(a, b);
    }

    float floating_point::ceil(float a)
    {
        return std::ceil(a);
    }

    float floating_point::floor(float a)
    {
        return std::floor(a);
    }

    float floating_point::round(float a)
    {
        return std::round(a);
    }

    float floating_point::trunc(float a)
    {
        return std::trunc(a);
    }

    float floating_point::fmod(float a, float b)
    {
        return std::fmod(a, b);
    }

    float floating_point::remainder(float a, float b)
    {
        return std::remainder(a, b);
    }

    float floating_point::copysign(float a, float b)
    {
        return std::copysign(a, b);
    }

    float floating_point::nextafter(float a, float b)
    {
        return std::nextafter(a, b);
    }

    float floating_point::fmin(float a, float b)
    {
        return std::fmin(a, b);
    }

    float floating_point::fmax(float a, float b)
    {
        return std::fmax(a, b);
    }

    float floating_point::fdim(float a, float b)
    {
        return std::fdim(a, b);
    }

    float floating_point::fma(float a, float b, float c)
    {
        return std::fma(a, b, c);
    }

    bool floating_point::isfinite(float a)
    {
        return std::isfinite(a);
    }

    bool floating_point::isinf(float a)
    {
        return std::isinf(a);
    }

    bool floating_point::isnan(float a)
    {
        return std::isnan(a);
    }

    bool floating_point::isnormal(float a)
    {
        return std::isnormal(a);
    }

    bool floating_point::signbit(float a)
    {
        return std::signbit(a);
    }

    int floating_point::fpclassify(float a)
    {
        return std::fpclassify(a);
    }

    float floating_point::abs(float a)
    {
        return std::abs(a);
    }

    float floating_point::scalbn(float a, int n)
    {
        return std::scalbn(a, n);
    }

    float floating_point::ldexp(float a, int exp)
    {
        return std::ldexp(a, exp);
    }

    float floating_point::frexp(float a, int* exp)
    {
        return std::frexp(a, exp);
    }

    float floating_point::modf(float a, float* iptr)
    {
        return std::modf(a, iptr);
    }

    float floating_point::hypot(float a, float b)
    {
        return std::hypot(a, b);
    }

    float floating_point::erf(float a)
    {
        return std::erf(a);
    }

    float floating_point::erfc(float a)
    {
        return std::erfc(a);
    }

    float floating_point::tgamma(float a)
    {
        return std::tgamma(a);
    }

    float floating_point::lgamma(float a)
    {
        return std::lgamma(a);
    }
} 