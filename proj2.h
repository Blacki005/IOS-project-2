#ifndef PROJ2_H__
#define PROJ2_H__

#include "print.h"
#include <errno.h>
#include <fcntl.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <unistd.h>

#define CUSTOMER 1
#define OFFICER 0
#define log fprintf_flush
#define IS_CHILD (getpid() != main_pid)

struct process_t {
    bool type; //indikuje, zda se jedna o proces zakaznika ci urednika
    unsigned id; //jeho id - poradi
    unsigned short service;
};

extern int *is_open;
extern unsigned *A;

extern sem_t *service1;
extern sem_t *service2;
extern sem_t *service3;
extern sem_t *close_rdy;
extern sem_t *choosing_service;
extern sem_t *taking_break;
extern sem_t *A_write;
extern sem_t *officer_rdy;

int init_semaphores();

void free_resources();

int create_officers(const unsigned officers_cnt, const unsigned max_break_time, FILE *output_file);

int create_customers(const unsigned customers_cnt, const unsigned max_wait_time, FILE *output_file);

void serve_line(sem_t *service, FILE *output_file, struct process_t identity);

void take_break(FILE *output_file, const unsigned max_break_time, struct process_t identity);

void enter_for_service(struct process_t identity, FILE *output_file);

long *check_arguments(int argc, const char *argv[]);

void close_office(unsigned max_shift_time, FILE *output_file);

#endif //PROJ2_H__