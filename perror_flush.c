#include "print.h"


void perror_flush(const char *s) {
    perror(s);
    fflush(stderr);
    return;
}