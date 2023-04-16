#include "proj2.h"

int create_officers(const unsigned officers_cnt, const unsigned max_break_time, FILE *output_file) {
    struct timeval t;
    const pid_t main_pid = getpid();
    int ret = max_break_time;

        for (unsigned i = 0; i < officers_cnt; i++) {

            //do teto sekce vstupuje jen hlavni proces
            if (getpid() == main_pid) {
                ret = fork();
                if (ret < 0) {
                    perror("Creating child failed");
                    return ret;
                }

                if (getpid() != main_pid) {
                    //vytvoreni jedinecne identity procesu
                    struct process_t identity;
                    identity.id = i + 1;
                    identity.type = OFFICER;
                    
                    sem_wait(A_write);
                    fprintf_flush(output_file, "%u: U %u: started\n", ++(*A),identity.id);
                    sem_post(A_write);

                    //TODO: obsluhuje zakazniky, dokud neni posta zavrena a zaroven jsou vsechny fronty prazdne


                    //pokud je otevreno nebo v alespon jedne z front je alespon jeden zakaznik, zamestnanci pracuji
                    //v momente kdy promenna is_open = 0, rady se uz nemohou menit - zakaznici jsou stabilni
                    while(*is_open || *(service1->__size)<=0 || *(service2->__size)<=0 || *(service3->__size)<=0) {
                        

                        sem_wait(taking_break);
                        //if all queues are empty, take a break - cannot close while employuee is taking break
                        //cannot take break in critical section choosing sdervice
                        if (*is_open && *(service1->__size)>0 && *(service2->__size)>0 && *(service3->__size)>0) {

                        //take break posts semaphore taking break inside - no need to post it afterwqards
                        take_break(output_file, max_break_time, identity);

                        } else {
                            sem_post(taking_break);
                        }


                        //only one office worker can work - TODO: make it faster
                        sem_wait(choosing_service);
                        
                        //nahodne vybere frontu, kterou bude obsluhovat: cislo 1 az 3
                        gettimeofday(&t, NULL);
                        srand(t.tv_usec * t.tv_sec);
                        identity.service = (rand() % 3) + 1;


                        //pokud je vybrana fronta prazdna, skace se na dalsi iteraci, jinak se obslouzi jeden zakaznik z dane fronty
                        switch (identity.service) {
                            case 1:
                                //obsluhuje zakaznika z fronty
                                serve_line(service1, output_file, identity);
                                break;
                            case 2:
                                //obsluhuje zakaznika z fronty
                                serve_line(service2, output_file, identity);
                                break;
                            case 3:
                                //obsluhuje zakaznika z fronty
                                serve_line(service3, output_file, identity);
                                break;
                            default:
                                perror_flush("Bad service number!");
                        }

                        //log(stderr, "service1 line: %d\nservice2 line: %d\nservice3 line:%d\n", line1_waiting, line2_waiting, line3_waiting);

                        //propusti zakaznika po dokonceni sluzby - uvolnuje semafor, dokud neni odemceny
                        while (sem_trywait(serving)) {
                            sem_post(serving);
                        }
                        sem_post(serving);
                        
                        //pusti dalsiho zamestnance ke sluzbe
                        sem_post(choosing_service);
                    }

                    sem_wait(A_write);
                    fprintf_flush(output_file, "%u: U %u: going home\n", ++(*A),identity.id);
                    sem_post(A_write);
                    exit(0);
                }
            }


        }

        return 0;
}