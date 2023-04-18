#include "proj2.h"

int init_semaphores() {
    close_rdy = sem_open("xjanst03 - close_rdy", O_CREAT, S_IRUSR | S_IWUSR, 1);
    if (close_rdy == SEM_FAILED) {
        return 0;
    }

    //jen jeden urednik muze v jednom momentu vybirat kterou sluzbu bude obsluhovat:
    choosing_service = sem_open("xjanst03 - choosing_service", O_CREAT, S_IRUSR | S_IWUSR, 1);
    if (choosing_service == SEM_FAILED) {
        return 0;
    }

    //cannot close if officer is just deciding to take break - initialized to open
    taking_break = sem_open("xjanst03 - taking_break", O_CREAT, S_IRUSR | S_IWUSR, 1);
    if (taking_break == SEM_FAILED) {
        return 0;
    }

    A_write = sem_open("xjanst03 - A_write", O_CREAT, S_IRUSR | S_IWUSR, 1);
    if (A_write == SEM_FAILED) {
        return 0;
    }

    //semafor resici bug, kdy mohl byt zakaznik volan k prepazce pred startem ureednika
    //semafor je incializovany jako otevreny
    //prvni vznikly je zavreny, kazdy zamestnanec na nem ceka (zavira ho), prvni vznikly zamestnanec ho otevre a kazdy zamestnanec ho otevira po dokonceni sluzby
    officer_rdy = sem_open("xjanst03 - officer_rdy", O_CREAT, S_IRUSR | S_IWUSR, 0);
    if (officer_rdy == SEM_FAILED) {
        return 0;
    }

    //inicializace semaforu pro fronty zakazniku - unlocked
    service1 = sem_open("xjanst03 - sem1", O_CREAT, S_IRUSR | S_IWUSR, 0);
    if (service1 == SEM_FAILED) {
        return 0;
    }
    service2 = sem_open("xjanst03 - sem2", O_CREAT, S_IRUSR | S_IWUSR, 0);
    if (service2 == SEM_FAILED) {
        return 0;
    }
    service3 = sem_open("xjanst03 - sem3", O_CREAT, S_IRUSR | S_IWUSR, 0);
    if (service3 == SEM_FAILED) {
        return 0;
    }

    return 1;
}