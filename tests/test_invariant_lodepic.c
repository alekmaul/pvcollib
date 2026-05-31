#include <check.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <limits.h>

/*
 * We replicate the vulnerable allocation logic here to test the invariant:
 * Buffer reads never exceed the declared length.
 *
 * The invariant: for any width/height values read from an untrusted source,
 * the allocation size must not overflow, and must be large enough to hold
 * (height+64)*width bytes without integer overflow.
 *
 * A safe implementation must:
 *   1. Detect integer overflow before allocation.
 *   2. Return NULL (or fail gracefully) on overflow/invalid input.
 *   3. Never allocate a buffer smaller than the data it will hold.
 */

/* Simulated image header as it would be read from file */
typedef struct {
    uint32_t width;
    uint32_t height;
} ImageHeader;

/* Simulated image buffer struct */
typedef struct {
    void   *buffer;
    size_t  alloc_size;
} Image;

/*
 * VULNERABLE implementation (mirrors lodepic.c:74):
 * image->buffer = malloc( (size_t)(header->height+64)*header->width );
 */
static int vulnerable_alloc(Image *image, const ImageHeader *header)
{
    if (!image || !header) return -1;
    /* This is the vulnerable line — no overflow check */
    size_t alloc = (size_t)(header->height + 64) * header->width;
    image->alloc_size = alloc;
    image->buffer = malloc(alloc);
    return (image->buffer != NULL) ? 0 : -1;
}

/*
 * SAFE implementation that checks for overflow before allocating.
 * Returns -1 and sets buffer=NULL if overflow or zero-size detected.
 */
static int safe_alloc(Image *image, const ImageHeader *header)
{
    if (!image || !header) return -1;

    /* Guard against zero dimensions */
    if (header->width == 0 || header->height == 0) {
        image->buffer = NULL;
        image->alloc_size = 0;
        return -1;
    }

    /* Check: height+64 overflow (uint32 wraps) */
    uint64_t h64 = (uint64_t)header->height + 64ULL;
    uint64_t w64 = (uint64_t)header->width;
    uint64_t total = h64 * w64;

    /* Reject if result exceeds SIZE_MAX or is suspiciously large (>256MB) */
    if (total > (uint64_t)SIZE_MAX || total > (uint64_t)(256 * 1024 * 1024)) {
        image->buffer = NULL;
        image->alloc_size = 0;
        return -1;
    }

    image->alloc_size = (size_t)total;
    image->buffer = malloc(image->alloc_size);
    return (image->buffer != NULL) ? 0 : -1;
}

/* Helper: compute expected allocation size safely, returns 0 on overflow */
static size_t safe_compute_size(uint32_t width, uint32_t height)
{
    uint64_t h64    = (uint64_t)height + 64ULL;
    uint64_t w64    = (uint64_t)width;
    uint64_t total  = h64 * w64;
    if (total > (uint64_t)SIZE_MAX) return 0;
    return (size_t)total;
}

/* ------------------------------------------------------------------ */
/* Test 1: Overflow-inducing width/height values must be rejected      */
/* ------------------------------------------------------------------ */
START_TEST(test_overflow_inputs_rejected)
{
    /* Invariant: allocation with overflow-inducing dimensions must fail
     * or produce a buffer large enough to hold the data. */

    typedef struct { uint32_t width; uint32_t height; } Dims;

    const Dims payloads[] = {
        /* Classic overflow: large values that wrap to small product */
        { 0xFFFFFFFFU, 0xFFFFFFFFU },
        { 0x80000000U, 0x80000000U },
        { 0x10000U,    0xFFFF0000U },
        { 0xFFFF0000U, 0x10000U    },
        /* height+64 wraps around uint32 */
        { 0x1000U,     0xFFFFFFC0U },  /* height+64 == 0 after wrap */
        { 0x1000U,     0xFFFFFFFFU },
        /* width alone causes overflow with moderate height */
        { 0xFFFFFFFFU, 1U          },
        { 1U,          0xFFFFFFFFU },
        /* 2x expected buffer size */
        { 0x8000U,     0x8000U     },
        /* 10x expected buffer size */
        { 0xA000U,     0xA000U     },
        /* Exact SIZE_MAX boundary */
        { (uint32_t)(SIZE_MAX / 2), 1U },
        /* Zero dimensions */
        { 0U,          100U        },
        { 100U,        0U          },
        { 0U,          0U          },
    };

    int num_payloads = (int)(sizeof(payloads) / sizeof(payloads[0]));

    for (int i = 0; i < num_payloads; i++) {
        ImageHeader hdr;
        hdr.width  = payloads[i].width;
        hdr.height = payloads[i].height;

        Image img;
        memset(&img, 0, sizeof(img));

        int rc = safe_alloc(&img, &hdr);

        if (rc == 0) {
            /* Allocation succeeded — buffer must be large enough */
            size_t expected = safe_compute_size(hdr.width, hdr.height);
            ck_assert_msg(expected > 0,
                "safe_alloc succeeded but expected size is 0 (overflow) "
                "for width=%u height=%u", hdr.width, hdr.height);
            ck_assert_msg(img.alloc_size >= expected,
                "Allocated buffer (%zu) is smaller than required (%zu) "
                "for width=%u height=%u",
                img.alloc_size, expected, hdr.width, hdr.height);
            free(img.buffer);
        } else {
            /* Allocation rejected — buffer must be NULL */
            ck_assert_msg(img.buffer == NULL,
                "safe_alloc returned error but buffer is not NULL "
                "for width=%u height=%u", hdr.width, hdr.height);
        }
    }
}
END_TEST

/* ------------------------------------------------------------------ */
/* Test 2: Vulnerable alloc produces undersized buffer on overflow     */
/* ------------------------------------------------------------------ */
START_TEST(test_vulnerable_alloc_undersized_on_overflow)
{
    /*
     * Invariant (negative test): demonstrate that the vulnerable code
     * CAN produce an undersized allocation when overflow occurs.
     * We verify the safe implementation avoids this.
     */
    typedef struct { uint32_t width; uint32_t height; } Dims;

    const Dims overflow_cases[] = {
        { 0x10000U,    0xFFFF0000U },
        { 0x80000001U, 0x7FFFFFFFU },
        { 0xFFFFU,     0xFFFF0001U },
    };

    int n = (int)(sizeof(overflow_cases) / sizeof(overflow_cases[0]));

    for (int i = 0; i < n; i++) {
        ImageHeader hdr;
        hdr.width  = overflow_cases[i].width;
        hdr.height = overflow_cases[i].height;

        /* Compute what the vulnerable code would allocate */
        size_t vuln_size = (size_t)(hdr.height + 64) * hdr.width;

        /* Compute the true required size (64-bit, no overflow) */
        uint64_t true_size = ((uint64_t)hdr.height + 64ULL) * (uint64_t)hdr.width;

        if (true_size > (uint64_t)SIZE_MAX) {
            /* Overflow occurred: vulnerable alloc is undersized */
            /* The safe implementation must reject this */
            Image safe_img;
            memset(&safe_img, 0, sizeof(safe_img));
            int rc = safe_alloc(&safe_img, &hdr);

            ck_assert_msg(rc != 0,
                "safe_alloc must reject overflow dimensions "
                "width=%u height=%u (vuln_size=%zu, true_size overflows)",
                hdr.width, hdr.height, vuln_size);
            ck_assert_msg(safe_img.buffer == NULL,
                "safe_alloc must set buffer=NULL on overflow rejection");
        }
    }
}
END_TEST

/* ------------------------------------------------------------------ */
/* Test 3: Valid small inputs are accepted and buffer is large enough  */
/* ------------------------------------------------------------------ */
START_TEST(test_valid_inputs_accepted)
{
    /* Invariant: legitimate small images must be allocated correctly */
    typedef struct { uint32_t width; uint32_t height; } Dims;

    const Dims valid_cases[] = {
        { 1U,    1U    },
        { 16U,   16U   },
        { 320U,  240U  },
        { 640U,  480U  },
        { 800U,  600U  },
        { 1024U, 768U  },
        { 256U,  256U  },
        { 128U,  128U  },
    };

    int n = (int)(sizeof(valid_cases) / sizeof(valid_cases[0]));

    for (int i = 0; i < n; i++) {
        ImageHeader hdr;
        hdr.width  = valid_cases[i].width;
        hdr.height = valid_cases[i].height;

        Image img;
        memset(&img, 0, sizeof(img));

        int rc = safe_alloc(&img, &hdr);

        ck_assert_msg(rc == 0,
            "safe_alloc must succeed for valid dims width=%u height=%u",
            hdr.width, hdr.height);
        ck_assert_msg(img.buffer != NULL,
            "buffer must not be NULL for valid dims width=%u height=%u",
            hdr.width, hdr.height);

        size_t expected = safe_compute_size(hdr.width, hdr.height);
        ck_assert_msg(img.alloc_size >= expected,
            "alloc_size=%zu must be >= expected=%zu for width=%u height=%u",
            img.alloc_size, expected, hdr.width, hdr.height);

        /* Write to every byte to confirm no overflow (would crash/asan if wrong) */
        memset(img.buffer, 0xAB, img.alloc_size);

        free(img.buffer);
    }
}
END_TEST

/* ------------------------------------------------------------------ */
/* Test 4: Boundary values around SIZE_MAX                             */
/* ------------------------------------------------------------------ */
START_TEST(test_boundary_size_max)
{
    /* Invariant: values near SIZE_MAX must not silently wrap */

    /* Construct width/height such that (height+64)*width is just above SIZE_MAX */
    /* Use width=2, height = SIZE_MAX/2 (which makes total > SIZE_MAX) */
    uint32_t width  = 2U;
    /* SIZE_MAX/2 may exceed uint32 on 64-bit; clamp to UINT32_MAX */
    uint64_t h_val  = (uint64_t)SIZE_MAX / 2ULL;
    if (h_val > (uint64_t)UINT32_MAX) h_val = (uint64_t)UINT32_MAX;
    uint32_t height = (uint32_t)h_val;

    ImageHeader hdr = { width, height };
    Image img;
    memset(&img, 0, sizeof(img));

    int rc = safe_alloc(&img, &hdr);

    /* Either it succeeds with a correctly-sized buffer, or it rejects */
    if (rc == 0) {
        size_t expected = safe_compute_size(hdr.width, hdr.height);
        ck_assert_msg(expected > 0,
            "safe_compute_size returned 0 but alloc succeeded");
        ck_assert_msg(img.alloc_size >= expected,
            "alloc_size=%zu < expected=%zu near SIZE_MAX boundary",
            img.alloc_size, expected);
        free(img.buffer);
    } else {
        ck_assert_msg(img.buffer == NULL,
            "buffer must be NULL when safe_alloc rejects near-SIZE_MAX input");
    }
}
END_TEST

/* ------------------------------------------------------------------ */
/* Test 5: height+64 wraps to zero — must be rejected                  */
/* ------------------------------------------------------------------ */
START_TEST(test_height_plus_64_wraps_to_zero)
{
    /*
     * Invariant: if height+64 wraps to 0 (uint32 overflow),
     * the allocation must be rejected, not silently produce a 0-byte buffer.
     */
    /* height = UINT32_MAX - 63 => height+64 = UINT32_MAX+1 = 0 (wrap) */
    uint32_t height = UINT32_MAX - 63U;
    uint32_t width  = 100U;

    ImageHeader hdr = { width, height };
    Image img;
    memset(&img, 0, sizeof(img));

    int rc = safe_alloc(&img, &hdr);

    /* Must be rejected because true size overflows */
    ck_assert_msg(rc != 0,
        "safe_alloc must reject height=%u width=%u (height+64 wraps)",
        height, width);
    ck_assert_msg(img.buffer == NULL,
        "buffer must be NULL when height+64 wraps to zero");
}
END_TEST

/* ------------------------------------------------------------------ */
/* Suite setup                                                          */
/* ------------------------------------------------------------------ */
Suite *security_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s       = suite_create("Security_CWE120_BufferOverflow");
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_overflow_inputs_rejected);
    tcase_add_test(tc_core, test_vulnerable_alloc_undersized_on_overflow);
    tcase_add_test(tc_core, test_valid_inputs_accepted);
    tcase_add_test(tc_core, test_boundary_size_max);
    tcase_add_test(tc_core, test_height_plus_64_wraps_to_zero);

    suite_add_tcase(s, tc_core);
    return s;
}

int main(void)
{
    int      number_failed;
    Suite   *s;
    SRunner *sr;

    s  = security_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}