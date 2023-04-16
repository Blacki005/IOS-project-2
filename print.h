#ifndef PRINT_H
#define PRINT_H

#include <stdlib.h>
#include <stdio.h>
#include "stdarg.h"
#include <errno.h>

void printf_flush(const char *fmt, ...);

void fprintf_flush(FILE *file, const char *fmt, ...);

void perror_flush(const char *s);

#endif //PRINT_H