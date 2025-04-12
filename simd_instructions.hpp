#pragma once

#include <cstdint>
#include <immintrin.h>

namespace vm
{
    class simd_instructions
    {
    public:
        static void add_ps(__m128& a, const __m128& b);
        static void sub_ps(__m128& a, const __m128& b);
        static void mul_ps(__m128& a, const __m128& b);
        static void div_ps(__m128& a, const __m128& b);
        static void sqrt_ps(__m128& a);
        static void rsqrt_ps(__m128& a);
        static void min_ps(__m128& a, const __m128& b);
        static void max_ps(__m128& a, const __m128& b);
        static void and_ps(__m128& a, const __m128& b);
        static void or_ps(__m128& a, const __m128& b);
        static void xor_ps(__m128& a, const __m128& b);
        static void not_ps(__m128& a);
        static void cmpeq_ps(__m128& a, const __m128& b);
        static void cmplt_ps(__m128& a, const __m128& b);
        static void cmple_ps(__m128& a, const __m128& b);
        static void cmpgt_ps(__m128& a, const __m128& b);
        static void cmpge_ps(__m128& a, const __m128& b);
        static void cmpneq_ps(__m128& a, const __m128& b);
        static void cmpnlt_ps(__m128& a, const __m128& b);
        static void cmpnle_ps(__m128& a, const __m128& b);
        static void cmpngt_ps(__m128& a, const __m128& b);
        static void cmpnge_ps(__m128& a, const __m128& b);
        static void blendv_ps(__m128& a, const __m128& b, const __m128& mask);
        static void shuffle_ps(__m128& a, const __m128& b, int imm8);
        static void permute_ps(__m128& a, int imm8);
        static void extract_ps(float& dst, const __m128& src, int imm8);
        static void insert_ps(__m128& dst, float src, int imm8);
        static void load_ps(__m128& dst, const float* src);
        static void store_ps(float* dst, const __m128& src);
        static void loadu_ps(__m128& dst, const float* src);
        static void storeu_ps(float* dst, const __m128& src);
        static void movemask_ps(int& dst, const __m128& src);
        static void setzero_ps(__m128& dst);
        static void set1_ps(__m128& dst, float src);
        static void set_ps(__m128& dst, float e3, float e2, float e1, float e0);
    };
} 