#include "print.h"

//printf s flush bufferu
void printf_flush(const char *fmt, ...) {
    va_list argv;
    va_start(argv, *fmt);
    vprintf(fmt, argv);
    fflush(stdout);
    va_end(argv);
    return;
}