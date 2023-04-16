/**TODO:
 * safe endings
 * 
 * 
 * 
 * 
*/

#include "proj2.h"
int *is_open;
unsigned *A;
sem_t *service1;
sem_t *service2;
sem_t *service3;
sem_t *can_close;
sem_t *choosing_service;
sem_t *serving;
sem_t *taking_break;
sem_t *A_write;

int main(int argc, char *argv[]) {
    if (argc != 6) {
        goto invalid_arguments;
    }

    unsigned customers_cnt = 0;     //NZ
    unsigned officers_cnt = 0;      //NU
    unsigned max_wait_time = 0;     //TZ
    unsigned max_break_time = 0;    //TU
    unsigned max_shift_time = 0;    //F

    //kontrola command line argumentu a jejich prirazeni do promennych
    customers_cnt = strtoul(argv[1], NULL, 10);
    if (errno == ERANGE || customers_cnt == 0) {
        goto invalid_arguments;
    }
    officers_cnt = strtoul(argv[2], NULL, 10);
    if (errno == ERANGE || officers_cnt == 0) {
        goto invalid_arguments;
    }
    max_wait_time = strtoul(argv[3], NULL, 10);
    if (errno == ERANGE || max_wait_time <= 0 || 10000 <= max_wait_time ) {
        goto invalid_arguments;
    }
    max_break_time = strtoul(argv[4], NULL, 10);
    if (errno == ERANGE || max_break_time <= 0|| 100 <= max_break_time) {
        goto invalid_arguments;
    }
    max_shift_time = strtoul(argv[5], NULL, 10);
    if (errno == ERANGE || max_shift_time <= 0 || 10000 <= max_shift_time) {
        goto invalid_arguments;
    }

    //vsechny parametry jsou v poradku - otevre se soubor
    FILE *output_file = fopen("./proj2.out", "a");
    if (output_file == NULL) {
        goto fopen_failed;
    }

    //vytvoreni sdilene promenne pro stav otevreni posty
    is_open = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, 0, 0); /// @brief Sdilena promenna indikujici otevreni posty. Pokud je 1, posta je otevrena, pokud je 0, posta je zavrena.
    if (is_open == MAP_FAILED) {
        goto map_failed;
    }

    A = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, 0, 0);
    if (A == MAP_FAILED) {
        goto map_failed;
    }

    //nahodny seed s vysokou rozptylnosti
    struct timeval t;
    gettimeofday(&t, NULL);
    srand(t.tv_usec * t.tv_sec);

    //open post office - set can_close to open
    *is_open = 1;
    can_close = sem_open("xjanst03 - can_close", O_CREAT, S_IRUSR | S_IWUSR, 1);
    //TODO: uvolnit can_close v pripade selhani

    //jen jeden urednik muze v jednom momentu vybirat kterou sluzbu bude obsluhovat:
    choosing_service = sem_open("xjanst03 - choosing_service", O_CREAT, S_IRUSR | S_IWUSR, 1);
    //TODO: korektni ukonceni

    serving = sem_open("xjanst03 - serving", O_CREAT, S_IRUSR | S_IWUSR, 0); //initialized to stop

    //cannot close if officer is just deciding to take break - initialized to open
    taking_break = sem_open("xjanst03 - taking_break", O_CREAT, S_IRUSR | S_IWUSR, 1);

    A_write = sem_open("xjanst03 - A_write", O_CREAT, S_IRUSR | S_IWUSR, 1);

    //inicializace semaforu pro fronty zakazniku - locked
    service1 = sem_open("xjanst03 - sem1", O_CREAT, S_IRUSR | S_IWUSR, 1);
    service2 = sem_open("xjanst03 - sem2", O_CREAT, S_IRUSR | S_IWUSR, 1);
    service3 = sem_open("xjanst03 - sem3", O_CREAT, S_IRUSR | S_IWUSR, 1);

    //check if semaphores were created successfully
    if (service1 == SEM_FAILED) {
        goto sem_failed;
    }
    if (service2 == SEM_FAILED) {
        sem_close(service1);
        sem_unlink("xjanst03 - sem1");
        goto sem_failed;
    }
    if (service3 == SEM_FAILED) {
        sem_close(service1);
        sem_unlink("xjanst03 - sem1");
        sem_close(service2);
        sem_unlink("xjanst03 - sem2");
        goto sem_failed;
    }

    if (create_customers(customers_cnt, max_wait_time, output_file)) {
        goto miscarriage;
    };
    
    //vytvoreni NU procesu uredniku
    if (create_officers(officers_cnt, max_break_time, output_file)) {
        goto miscarriage;
    };

    //max_wait_time / 2 != 0 => max_wait_time > 1
    //osetreni floating point exeption: division by zero
    if (max_shift_time > 1) {
        usleep((max_shift_time / 2) + (rand() % (max_shift_time / 2)));

        //pri zavirani posty nesmi dvermi prochazet zakaznik - ceka se na semafor
        sem_wait(can_close);
        sem_wait(taking_break);

        sem_wait(A_write);
        fprintf_flush(output_file, "%u: closing\n", ++(*A));
        sem_post(A_write);
    } else {
        usleep(max_shift_time);

        sem_wait(can_close);
        sem_wait(taking_break);

        sem_wait(A_write);
        fprintf_flush(output_file, "%u: closing\n", *A);
        sem_post(A_write);
    }
    *is_open = 0;
    sem_post(taking_break);
    sem_post(can_close);


    //cekani na skonceni vsech procesu
    while(wait(NULL) > 0);

    //uvolneni prostredku
    free_resources();
    fclose(output_file);
    munmap(is_open, sizeof(int));
    return 0;

    invalid_arguments:
        errno = EINVAL;
        perror_flush("Error");
        return 1;

    fopen_failed:
        perror_flush("Unable to open the file \"./proj2.out\"");
        return 1;
    
    map_failed:
        perror_flush("Creating shared memory failed");
        fclose(output_file);
        return 1;

    sem_failed:
        perror_flush("Creating semaphore failed");
        munmap(is_open, sizeof(int));
        fclose(output_file);
        return 1;

    miscarriage:
        while(wait(NULL) > 0);
        munmap(is_open, sizeof(int));
        free_resources();
        fclose(output_file);
        return 1;
    
}