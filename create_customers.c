#include "proj2.h"

int create_customers(const unsigned customers_cnt, const unsigned max_wait_time, FILE *output_file) {
    const pid_t main_pid = getpid();
    struct timeval t;
    int ret = 0;

    //vytvoreni NZ procesu zakazniku
    for (unsigned i = 0; i < customers_cnt; i++) {

        //children processes can't create another processesipcs
        if (getpid() == main_pid) {
            ret = fork();
            if (ret < 0) {
                //failed to create child
                perror("Creating child failed");
                return ret;
            }

            if (getpid() != main_pid) {

                //sekce, kterou projizdi jen nove vytvorene dite
                struct process_t identity;
                identity.id = i+1;
                identity.type = CUSTOMER;

                sem_wait(A_write);
                fprintf_flush(output_file, "%u: Z %u: started\n", ++(*A), identity.id);
                sem_post(A_write);

                //cekani nahodny cas v intervalu <0, TZ> po spusteni
                usleep(rand() % max_wait_time);

                //pokud je posta otevrena, vyrizuje sluzbu, jinak odchazi domu - kdyz prochazi dvermi posty, posta nemuze zavrit - can_close semafor
                sem_wait(can_close);

                if (*is_open) {

                    //nahodne vybira sluzbu: cislo 1 az 3
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

                    exit(0);
                } else {
                    //pokud je posta uzavrena, odchazi domu
                    sem_wait(A_write);
                    fprintf_flush(output_file, "%u: Z %u: going home\n", ++(*A),identity.id);
                    sem_post(A_write);
                    sem_post(can_close);
                    exit(0);
                }
            }
        }
    }

    return 0;

}