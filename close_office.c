#include "proj2.h"

void close_office(unsigned max_shift_time, FILE *output_file) {
    //nahodny seed s vysokou presnosti
    struct timeval t;
    gettimeofday(&t, NULL);
    srand(t.tv_usec * t.tv_sec);

    //max_wait_time / 2 != 0 => max_wait_time > 1
    //osetreni floating point exeption: division by zero
    if (max_shift_time > 1) {
        usleep((max_shift_time / 2) + (rand() % (max_shift_time / 2)));

        //pri zavirani posty nesmi dvermi prochazet zakaznik - ceka se na semafor
        sem_wait(close_rdy);
        sem_wait(taking_break);

        sem_wait(A_write);
        fprintf_flush(output_file, "%u: closing\n", ++(*A));
        sem_post(A_write);
    } else {
        usleep(max_shift_time);

        sem_wait(close_rdy);
        sem_wait(taking_break);

        sem_wait(A_write);
        fprintf_flush(output_file, "%u: closing\n", *A);
        sem_post(A_write);
    }

    *is_open = 0;
    sem_post(taking_break);
    sem_post(close_rdy);

    return;
}