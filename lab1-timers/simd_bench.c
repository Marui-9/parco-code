#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <time.h>
#ifdef _OPENMP
#include <omp.h>
#endif
#include <immintrin.h>

// Timing helpers
static inline double now_sec(void) {
#ifdef _OPENMP
    return omp_get_wtime();
#else
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec * 1e-9;
#endif
}

// Initialize arrays
static void init_arrays(float *A, float *B, size_t N) {
    for (size_t i = 0; i < N; ++i) {
        A[i] = (float)i * 0.5f;
        B[i] = (float)(N - i) * 0.25f;
    }
}

// Scalar baseline
void vec_add_scalar(const float *A, const float *B, float *C, size_t N) {
    for (size_t i = 0; i < N; ++i) {
        C[i] = A[i] + B[i];
    }
}

// Directive-based vectorization (OpenMP SIMD or Intel #pragma simd)
void vec_add_simd(const float *A, const float *B, float *C, size_t N) {
    #pragma omp simd
    for (size_t i = 0; i < N; ++i) {
        C[i] = A[i] + B[i];
    }
}

// Intrinsics: AVX2 (256-bit). Falls back to scalar if not available at compile-time.
void vec_add_avx(const float *A, const float *B, float *C, size_t N) {
#if defined(__AVX2__)
    size_t i = 0;
    const size_t V = 8; // 8 floats per 256-bit vector
    for (; i + V <= N; i += V) {
        __m256 a = _mm256_load_ps(A + i);   // assume aligned (we allocate aligned)
        __m256 b = _mm256_load_ps(B + i);
        __m256 c = _mm256_add_ps(a, b);
        _mm256_store_ps(C + i, c);
    }
    // remainder
    for (; i < N; ++i) C[i] = A[i] + B[i];
#elif defined(__AVX__)
    size_t i = 0;
    const size_t V = 8;
    for (; i + V <= N; i += V) {
        __m256 a = _mm256_load_ps(A + i);
        __m256 b = _mm256_load_ps(B + i);
        __m256 c = _mm256_add_ps(a, b);
        _mm256_store_ps(C + i, c);
    }
    for (; i < N; ++i) C[i] = A[i] + B[i];
#else
    // No AVX available at compile time, do scalar
    vec_add_scalar(A, B, C, N);
#endif
}

// Optional: AVX-512 version (if compiled with -mavx512f)
void vec_add_avx512(const float *A, const float *B, float *C, size_t N) {
#if defined(__AVX512F__)
    size_t i = 0;
    const size_t V = 16; // 16 floats per 512-bit vector
    for (; i + V <= N; i += V) {
        __m512 a = _mm512_load_ps(A + i);
        __m512 b = _mm512_load_ps(B + i);
        __m512 c = _mm512_add_ps(a, b);
        _mm512_store_ps(C + i, c);
    }
    for (; i < N; ++i) C[i] = A[i] + B[i];
#else
    vec_add_avx(A, B, C, N);
#endif
}

// Verify correctness (max absolute error)
float max_abs_err(const float *X, const float *Y, size_t N) {
    float e = 0.0f;
    for (size_t i = 0; i < N; ++i) {
        float d = fabsf(X[i] - Y[i]);
        if (d > e) e = d;
    }
    return e;
}

int main(int argc, char** argv) {
    // Choose a large default size (e.g., 16,777,216 elements ≈ 64 MB per array)
    size_t N = (1u<<27); // ~134M
    int iters = 5;
    if (argc > 1) N = strtoull(argv[1], NULL, 10);
    if (argc > 2) iters = atoi(argv[2]);
    printf("N=%zu (%.2f MB per array), iterations=%d\n", N, N*sizeof(float)/1e6, iters);

    // Allocate 64-byte aligned memory (good for AVX/AVX-512)
    size_t bytes = N * sizeof(float);
    float *A = NULL, *B = NULL, *C = NULL, *Ref = NULL;
#if defined(_MSC_VER)
    A = (float*)_aligned_malloc(bytes, 64);
    B = (float*)_aligned_malloc(bytes, 64);
    C = (float*)_aligned_malloc(bytes, 64);
    Ref = (float*)_aligned_malloc(bytes, 64);
    if(!A||!B||!C||!Ref){fprintf(stderr,"alloc failed\n"); return 1;}
#else
    if (posix_memalign((void**)&A, 64, bytes) ||
        posix_memalign((void**)&B, 64, bytes) ||
        posix_memalign((void**)&C, 64, bytes) ||
        posix_memalign((void**)&Ref,64, bytes)) {
        fprintf(stderr, "posix_memalign failed\n");
        return 1;
    }
#endif

    init_arrays(A, B, N);

    // Warm up & reference
    vec_add_scalar(A, B, Ref, N);

    // Helper to time a kernel
    auto bench = [&](const char* name, void(*kernel)(const float*, const float*, float*, size_t)) {
        // warm-up
        kernel(A, B, C, N);
        double best = 1e9;
        for (int t = 0; t < iters; ++t) {
            double t0 = now_sec();
            kernel(A, B, C, N);
            double t1 = now_sec();
            double dt = t1 - t0;
            if (dt < best) best = dt;
        }
        // bandwidth: read A+B, write C
        double gb = (3.0 * bytes) / 1e9;
        double gbps = gb / best;
        float err = max_abs_err(C, Ref, N);
        printf("%-18s: %8.4f s  |  %6.1f GB/s  |  max |err| = %.3g\n", name, best, gbps, err);
        return best;
    };

    // Run
    double ts = bench("scalar", vec_add_scalar);
    double to = bench("omp simd", vec_add_simd);
#if defined(__AVX512F__)
    double ta = bench("avx512", vec_add_avx512);
#else
    double ta = bench("avx/avx2", vec_add_avx);
#endif

    printf("Speedup omp simd vs scalar: %.2fx\n", ts / to);
    printf("Speedup avx*    vs scalar: %.2fx\n", ts / ta);

#if defined(_MSC_VER)
    _aligned_free(A); _aligned_free(B); _aligned_free(C); _aligned_free(Ref);
#else
    free(A); free(B); free(C); free(Ref);
#endif
    return 0;
}
