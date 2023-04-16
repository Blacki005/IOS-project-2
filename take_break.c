#include "proj2.h"

void take_break(FILE *output_file, const unsigned max_break_time, struct process_t identity) {
    struct timeval t;

    sem_wait(A_write);
    fprintf_flush(output_file, "%u: U %u: taking break\n", ++(*A),identity.id);
    sem_post(A_write);

    sem_post(taking_break);
    gettimeofday(&t, NULL);
    srand(t.tv_usec * t.tv_sec);
    usleep(rand() % max_break_time);

    
    sem_wait(A_write);
    fprintf_flush(output_file, "%u: U %u: break finished\n", ++(*A),identity.id);
    sem_post(A_write);

    return;
}