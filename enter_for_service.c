#include "proj2.h"


void enter_for_service (struct process_t identity, FILE *output_file) {

    //nahodne vybira sluzbu: cislo 1 az 3
    struct timeval t;
    gettimeofday(&t, NULL);
    srand(t.tv_usec * t.tv_sec);
    identity.service = (rand() % 3) + 1;

    sem_wait(A_write);
    fprintf_flush(output_file, "%u: Z %u: entering office for a service %u\n", ++(*A),identity.id, identity.service);
    sem_post(A_write);

    //zaradi se do fronty X a ceka na zavolani urednikem. Pote co se zaradil do fronty uz muze byt posta zavrena
    switch (identity.service) {
        case 1:
            sem_wait(service1);
            break;
        case 2:
            sem_wait(service2);
            break;
        case 3:
            sem_wait(service3);
            break;
        default:
            perror_flush("Bad service number!");
    }
    sem_post(can_close);

    //muze byt zavolan az v momente, kdy je urednik na poste
    sem_wait(officer_start);

    //ceka na vyrizeni sluzby (pokud zde dorazi moc brzo, zamkne z defaultu zamceny semafor podruhe)
    sem_wait(A_write);
    fprintf_flush(output_file, "%u: Z %u: called by office worker\n", ++(*A),identity.id);
    sem_post(A_write);

    sem_wait(serving);

    //odchazi domu po dokonceni sluzby
    sem_wait(A_write);
    fprintf_flush(output_file, "%u: Z %u: going home\n", ++(*A),identity.id);
    sem_post(A_write);

    //zamyka semafor cekani na vyrizeni sluzby pro nasledujicicho:
    sem_trywait(serving);

    return;
}