#include "proj2.h"



void free_resources() {
    int val = 0;
    int ret = 0;
    ret = sem_getvalue(service1, &val);
    if (ret == 0) {
        ret = sem_close(service1);
        if (!ret) {
            //perror_flush("Semaphore service1 failed to close");
        }
        ret = sem_unlink("xjanst03 - sem1");
        if (!ret) {
            //perror_flush("Semaphore service1 failed to unlink");
        }
    }
    

    ret = sem_getvalue(service2, &val);
    if (ret == 0) {
        ret = sem_close(service2);
        if (!ret) {
            //perror_flush("Semaphore service2 failed to close");
        }
        ret = sem_unlink("xjanst03 - sem2");
        if (!ret) {
            //perror_flush("Semaphore service2 failed to unlink");
        }
    }

    ret = sem_getvalue(service3, &val);
    if (ret == 0) {
        ret = sem_close(service3);
        if (!ret) {
            //perror_flush("Semaphore service3 failed to close");
        }
        ret = sem_unlink("xjanst03 - sem3");
        if (!ret) {
            //perror_flush("Semaphore service3 failed to unlink");
        }
    }

    ret = sem_getvalue(can_close, &val);
    if (ret == 0) {
        ret = sem_close(can_close);
        if (!ret) {
            //perror_flush("Semaphore can_close failed to close");
        }
        ret = sem_unlink("xjanst03 - can_close");
        if (!ret) {
            //perror_flush("Semaphore can_close failed to unlink");
        }

    }

    ret = sem_getvalue(choosing_service, &val);
    if (ret == 0) {
        ret = sem_close(choosing_service);
        if (!ret) {
            //perror_flush("Semaphore choosing_service failed to close");
        }
        ret = sem_unlink("xjanst03 - choosing_service");
        if (!ret) {
            //perror_flush("Semaphore choosing_service failed to unlink");
        }
    }

    ret = sem_getvalue(serving, &val);
    if (ret == 0) {
        ret = sem_close(serving);
        if (!ret) {
            //perror_flush("Semaphore serving failed to close");
        }
        ret = sem_unlink("xjanst03 - serving");
        if (!ret) {
            //perror_flush("Semaphore serving failed to unlink");
        }
    }

    ret = sem_getvalue(taking_break, &val);
    if (ret == 0) {
        ret = sem_close(taking_break);
        if (!ret) {
            //perror_flush("Semaphore taking_break failed to close");
        }
        ret = sem_unlink("xjanst03 - taking_break");
        if (!ret) {
            //perror_flush("Semaphore taking_break failed to unlink");
        }
    }

    ret = sem_getvalue(A_write, &val);
    if (ret == 0) {
        ret = sem_close(A_write);
        if (!ret) {
            //perror_flush("Semaphore A_write failed to close");
        }
        ret = sem_unlink("xjanst03 - A_write");
        if (!ret) {
            //perror_flush("Semaphore A_write failed to unlink");
        }
    }

}