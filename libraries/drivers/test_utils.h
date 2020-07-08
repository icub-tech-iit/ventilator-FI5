#ifndef __TEST_UTILS__
#define __TEST_UTILS__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

void fail(const char *fmt, ...);
void test(bool cond, const char *fmt, ...);
void test_silent(bool cond, const char *fmt, ...);
long long get_time_ms(void);


#ifdef __cplusplus
}
#endif

#endif
