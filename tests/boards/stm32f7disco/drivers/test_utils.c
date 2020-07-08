#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

long long get_time_ms(void)
{
    long long ms;
    struct timespec spec;

    clock_gettime(CLOCK_REALTIME, &spec);

    ms = spec.tv_nsec / 1.0e6 +
	    spec.tv_sec * 1000;

    return ms;
}

void fail(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    printf(" ----- FAILED ------\n");
    vprintf(fmt, args);
    va_end(args);
    printf("\n -------------------\n");
    exit(-1);
}


void test_silent(bool cond, const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    if (!cond) {
	    printf(" ----- FAILED ------\n");
	    vprintf(fmt, args);
	    va_end(args);
	    printf("\n -------------------\n");
	    exit(-1);
    }

}
void test(bool cond, const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    if (cond) {
	    printf("PASS: ");
	    vprintf(fmt, args);
	    va_end(args);
	    printf("\n");
    } else {
	    printf(" ----- FAILED ------\n");
	    vprintf(fmt, args);
	    va_end(args);
	    printf("\n -------------------\n");
	    exit(-1);
    }
}
