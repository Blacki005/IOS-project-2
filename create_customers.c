#include "proj2.h"

int create_customers(const unsigned customers_cnt, const unsigned max_wait_time, FILE *output_file) {
    const pid_t main_pid = getpid();
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

            if (IS_CHILD) {

                //sekce, kterou projizdi jen nove vytvorene dite
                struct process_t identity;
                identity.id = i+1;
                identity.type = CUSTOMER;

                sem_wait(A_write);
                fprintf_flush(output_file, "%u: Z %u: started\n", ++(*A), identity.id);
                sem_post(A_write);

                //cekani nahodny cas v intervalu <0, TZ> po spusteni
                usleep(rand() % max_wait_time);

                //pokud je posta otevrena, vyrizuje sluzbu, jinak odchazi domu - kdyz prochazi dvermi posty, posta nemuze zavrit - close_rdy semafor
                sem_wait(close_rdy);

                if (*is_open) {
                    enter_for_service(identity, output_file);
                    exit(0);
                } else {
                    //pokud je posta uzavrena, odchazi domu
                    sem_wait(A_write);
                    fprintf_flush(output_file, "%u: Z %u: going home\n", ++(*A),identity.id);
                    sem_post(A_write);
                    sem_post(close_rdy);
                    exit(0);
                }
            }
        }
    }

    return 0;

}