#ifndef __TEST_UTILS__
#define __TEST_UTILS__

void fail(const char *fmt, ...);
void test(bool cond, const char *fmt, ...);
long long get_time_ms(void);

#endif
