#include "proj2.h"


//todo: make threee semaphores for each worker - lock them and safely get line value by function

void serve_line(sem_t *service, FILE *output_file, struct process_t identity) {
    if (*(service->__size) > 0) {
        return;
    }

    //zakaznik muze jit k prepazce
    sem_post(officer_rdy);
    //sem_wait(taking_break); //officer cant change line values when another officer is deciding to have a break

    //obsluhuje zakaznika z fronty
    sem_wait(A_write);
    fprintf_flush(output_file, "%u: U %u: serving a service of type %u\n", ++(*A),identity.id, identity.service);
    sem_post(A_write);

    //ceka nahodny cas v intervalu <0,10> -- obsluhuje zakaznika
    usleep(rand() % 10);
    sem_post(service);

    //sem_post(taking_break);//another officer can now decide to have a break

    //vypisuje dokonceni sluzby
    sem_wait(A_write);
    fprintf_flush(output_file, "%u: U %u: service finished\n", ++(*A), identity.id);
    sem_post(A_write);

    return;
}