#include "proj2.h"

#define NZ parameters[0]
#define NU parameters[1]
#define TZ parameters[2]
#define TU parameters[3]
#define F parameters[4]

long *check_arguments(int argc, const char *argv[]) {
    if (argc != 6) {
        return NULL;
    }

    long *parameters = calloc(5, sizeof(long int));
    if (parameters == NULL) {
        return NULL;
    }

    //kontrola command line argumentu a jejich prirazeni do promennych
    NZ = strtol(argv[1], NULL, 10);
    if (errno == ERANGE || NZ == 0) {
        free(parameters);
        return NULL;
    }
    NU = strtol(argv[2], NULL, 10);
    if (errno == ERANGE || NU == 0) {
        free(parameters);
        return NULL;
    }
    TZ = strtol(argv[3], NULL, 10);
    if (errno == ERANGE || TZ < 0 || 10000 < TZ ) {
        free(parameters);
        return NULL;
    }
    TU = strtol(argv[4], NULL, 10);
    if (errno == ERANGE || TU < 0 || 100 < TU) {
        free(parameters);
        return NULL;
    }
    F = strtol(argv[5], NULL, 10);
    if (errno == ERANGE || F < 0 || 10000 < F) {
        free(parameters);
        return NULL;
    }

    return parameters;
}