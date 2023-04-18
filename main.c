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
sem_t *close_rdy;
sem_t *officer_rdy;
sem_t *choosing_service;
sem_t *taking_break;
sem_t *A_write;

int main(int argc, const char *argv[]) {

    long *parameters = check_arguments(argc, argv);
    if (parameters == NULL) {
        goto invalid_arguments;
    }
    //main arguments are valid
    unsigned customers_cnt = parameters[0];     //NZ
    unsigned officers_cnt = parameters[1];      //NU
    unsigned max_wait_time = parameters[2];     //TZ
    unsigned max_break_time = parameters[3];    //TU
    unsigned max_shift_time = parameters[4];    //F
    
    FILE *output_file = fopen("./proj2.out", "a");
    if (output_file == NULL) {
        goto fopen_failed;
    }


    //vytvoreni sdilene promenne pro stav otevreni posty
    is_open = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, 0, 0); /// @brief Sdilena promenna indikujici otevreni posty. Pokud je 1, posta je otevrena, pokud je 0, posta je zavrena.
    if (is_open == MAP_FAILED) {
        goto map_failed;
    }

    //vytvoreni sdilene promenne pro cislovani jednotlivych akci
    A = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, 0, 0);
    if (A == MAP_FAILED) {
        goto map_failed;
    }

    //open post office - set close_rdy to open
    *is_open = 1;

    
    if (!init_semaphores()) {
        goto sem_failed;
    }

    if (create_customers(customers_cnt, max_wait_time, output_file)) {
        goto miscarriage;
    };
    
    //vytvoreni NU procesu uredniku
    if (create_officers(officers_cnt, max_break_time, output_file)) {
        goto miscarriage;
    };

    //funkce po zavolani pocka do konce smeny, ktery nahodne generuje, a pote zavre postu
    close_office(max_shift_time, output_file);

    //cekani na skonceni vsech procesu
    while(wait(NULL) > 0);

    //uvolneni prostredku
    free_resources();
    fclose(output_file);
    free(parameters);
    munmap(is_open, sizeof(int));
    return 0;

    invalid_arguments:
        errno = EINVAL;
        perror_flush("Error");
        return 1;

    fopen_failed:
        perror_flush("Unable to open the file \"./proj2.out\"");
        free(parameters);
        return 1;
    
    map_failed:
        perror_flush("Creating shared memory failed");
        fclose(output_file);
        free(parameters);
        return 1;

    sem_failed:
        perror_flush("Creating semaphore failed");
        munmap(is_open, sizeof(int));
        free_resources();
        fclose(output_file);
        free(parameters);
        return 1;

    miscarriage:
        while(wait(NULL) > 0);
        munmap(is_open, sizeof(int));
        free_resources();
        fclose(output_file);
        free(parameters);
        return 1;
    
}