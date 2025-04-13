#include "simd_instructions.hpp"

namespace vm
{
    void simd_instructions::add_ps(__m128& a, const __m128& b)
    {
        a = _mm_add_ps(a, b);
    }

    void simd_instructions::sub_ps(__m128& a, const __m128& b)
    {
        a = _mm_sub_ps(a, b);
    }

    void simd_instructions::mul_ps(__m128& a, const __m128& b)
    {
        a = _mm_mul_ps(a, b);
    }

    void simd_instructions::div_ps(__m128& a, const __m128& b)
    {
        a = _mm_div_ps(a, b);
    }

    void simd_instructions::sqrt_ps(__m128& a)
    {
        a = _mm_sqrt_ps(a);
    }

    void simd_instructions::rsqrt_ps(__m128& a)
    {
        a = _mm_rsqrt_ps(a);
    }

    void simd_instructions::min_ps(__m128& a, const __m128& b)
    {
        a = _mm_min_ps(a, b);
    }

    void simd_instructions::max_ps(__m128& a, const __m128& b)
    {
        a = _mm_max_ps(a, b);
    }

    void simd_instructions::and_ps(__m128& a, const __m128& b)
    {
        a = _mm_and_ps(a, b);
    }

    void simd_instructions::or_ps(__m128& a, const __m128& b)
    {
        a = _mm_or_ps(a, b);
    }

    void simd_instructions::xor_ps(__m128& a, const __m128& b)
    {
        a = _mm_xor_ps(a, b);
    }

    void simd_instructions::not_ps(__m128& a)
    {
        a = _mm_xor_ps(a, _mm_castsi128_ps(_mm_set1_epi32(0xFFFFFFFF)));
    }

    void simd_instructions::cmpeq_ps(__m128& a, const __m128& b)
    {
        a = _mm_cmpeq_ps(a, b);
    }

    void simd_instructions::cmplt_ps(__m128& a, const __m128& b)
    {
        a = _mm_cmplt_ps(a, b);
    }

    void simd_instructions::cmple_ps(__m128& a, const __m128& b)
    {
        a = _mm_cmple_ps(a, b);
    }

    void simd_instructions::cmpgt_ps(__m128& a, const __m128& b)
    {
        a = _mm_cmpgt_ps(a, b);
    }

    void simd_instructions::cmpge_ps(__m128& a, const __m128& b)
    {
        a = _mm_cmpge_ps(a, b);
    }

    void simd_instructions::cmpneq_ps(__m128& a, const __m128& b)
    {
        a = _mm_cmpneq_ps(a, b);
    }

    void simd_instructions::cmpnlt_ps(__m128& a, const __m128& b)
    {
        a = _mm_cmpnlt_ps(a, b);
    }

    void simd_instructions::cmpnle_ps(__m128& a, const __m128& b)
    {
        a = _mm_cmpnle_ps(a, b);
    }

    void simd_instructions::cmpngt_ps(__m128& a, const __m128& b)
    {
        a = _mm_cmpngt_ps(a, b);
    }

    void simd_instructions::cmpnge_ps(__m128& a, const __m128& b)
    {
        a = _mm_cmpnge_ps(a, b);
    }

    void simd_instructions::blendv_ps(__m128& a, const __m128& b, const __m128& mask)
    {
        a = _mm_blendv_ps(a, b, mask);
    }

    void simd_instructions::shuffle_ps(__m128& a, const __m128& b, int imm8)
    {
        a = _mm_shuffle_ps(a, b, imm8);
    }

    void simd_instructions::permute_ps(__m128& a, int imm8)
    {
        a = _mm_permute_ps(a, imm8);
    }

    void simd_instructions::extract_ps(float& dst, const __m128& src, int imm8)
    {
        dst = _mm_cvtss_f32(_mm_permute_ps(src, imm8));
    }

    void simd_instructions::insert_ps(__m128& dst, float src, int imm8)
    {
        dst = _mm_insert_ps(dst, _mm_set_ss(src), imm8);
    }

    void simd_instructions::load_ps(__m128& dst, const float* src)
    {
        dst = _mm_load_ps(src);
    }

    void simd_instructions::store_ps(float* dst, const __m128& src)
    {
        _mm_store_ps(dst, src);
    }

    void simd_instructions::loadu_ps(__m128& dst, const float* src)
    {
        dst = _mm_loadu_ps(src);
    }

    void simd_instructions::storeu_ps(float* dst, const __m128& src)
    {
        _mm_storeu_ps(dst, src);
    }

    void simd_instructions::movemask_ps(int& dst, const __m128& src)
    {
        dst = _mm_movemask_ps(src);
    }

    void simd_instructions::setzero_ps(__m128& dst)
    {
        dst = _mm_setzero_ps();
    }

    void simd_instructions::set1_ps(__m128& dst, float src)
    {
        dst = _mm_set1_ps(src);
    }

    void simd_instructions::set_ps(__m128& dst, float e3, float e2, float e1, float e0)
    {
        dst = _mm_set_ps(e3, e2, e1, e0);
    }
} 