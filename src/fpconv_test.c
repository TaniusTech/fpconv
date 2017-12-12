#include <assert.h>
#include <math.h>
#include <stdio.h>

#include "fpconv.c"

static bool isnan_karl(const double d)
{
    union { uint64_t u; double f; } ieee754;
    ieee754.f = d;
    return ((uint32_t)(ieee754.u >> 32) & 0x7fffffff) + ((uint32_t)ieee754.u != 0) > 0x7ff00000;
}

static void TEST_ISNAN_IMPL(const double d)
{
    const uint64_t bits = get_dbits(d);
    const bool naninf = (bits & expmask) == expmask;
    const bool nan = bits & fracmask;
    assert(isnan_karl(d) == (naninf && nan));
}

static void TEST_ISNAN()
{
    TEST_ISNAN_IMPL(0.0);
    TEST_ISNAN_IMPL(3.141592);
    TEST_ISNAN_IMPL(-10000000000.0);
    TEST_ISNAN_IMPL(1.0/0.0);
    TEST_ISNAN_IMPL(-1.0/0.0);
    TEST_ISNAN_IMPL(0.0/0.0);
    TEST_ISNAN_IMPL(-0.0/0.0);

    printf("Test ISNAN passed\n");
}

static void TEST_FILTER_SPECIAL()
{
    char buf[25];

    memset(buf, 0, sizeof(buf));
    assert(filter_special(0.0, buf) == 1);
    assert(buf[0] == '0');

    assert(filter_special(3.141592, buf) == 0);

    assert(filter_special(-10000000000.0, buf) == 0);

    memset(buf, 0, sizeof(buf));
    assert(filter_special(1.0/0.0, buf) == 3);
    assert(strncmp("inf", buf, 3) == 0);

    memset(buf, 0, sizeof(buf));
    assert(filter_special(-1.0/0.0, buf) == 3);
    assert(strncmp("inf", buf, 3) == 0);

    memset(buf, 0, sizeof(buf));
    assert(filter_special(0.0/0.0, buf) == 3);
    assert(strncmp("nan", buf, 3) == 0);

    memset(buf, 0, sizeof(buf));
    assert(filter_special(-0.0/0.0, buf) == 3);
    assert(strncmp("nan", buf, 3) == 0);

    printf("Test FILTER_SPECIAL passed\n");
}

static void TEST_FPCONV_DTOA()
{
    char buf[24];

    memset(buf, 0, sizeof(buf));
    assert(fpconv_dtoa(0.0, buf) == 1);
    assert(buf[0] == '0');

    // TODO(sherry): perhaps test some reals

    memset(buf, 0, sizeof(buf));
    assert(fpconv_dtoa(1.0/0.0, buf) == 3);
    assert(strncmp("inf", buf, 3) == 0);

    memset(buf, 0, sizeof(buf));
    assert(fpconv_dtoa(-1.0/0.0, buf) == 4);
    assert(strncmp("-inf", buf, 4) == 0);

    memset(buf, 0, sizeof(buf));
    assert(fpconv_dtoa(0.0/0.0, buf) == 3);
    assert(strncmp("nan", buf, 3) == 0);

    memset(buf, 0, sizeof(buf));
    assert(fpconv_dtoa(-0.0/0.0, buf) == 3);
    assert(strncmp("nan", buf, 3) == 0);

    printf("Test FPCONV_DTOA passed\n");
}

int main()
{
    TEST_ISNAN();
    TEST_FILTER_SPECIAL();
    TEST_FPCONV_DTOA();

    printf("All tests passed!\n");

    return 0;
}
