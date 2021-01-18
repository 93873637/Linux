#include <memory.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

sem_t sem;

void do_work(const char *caller, void *p)
{
    sem_wait(&sem);
    int *data = (int *)p;
    (*data)++;
    printf("%s: data = %d\n", caller, *data);
    sem_post(&sem);
    usleep(200);
}

void *threadfunc_working(void *p)
{
    printf("threadfunc: E...\n");
    while (1)
    {
        //sem_wait(&sem);
        do_work("thread", p);
        //printf("threadfunc: data = %d\n", *((int*)p));
        //sem_post(&sem);
    }
    printf("threadfunc: X.\n");
}

void enter_main_loop(void *p)
{
    printf("mainloop: E...\n");
    while (1)
    {
        //sem_wait(&sem);
        do_work("main", p);
        //printf("mainloop: data = %d\n", *((int*)p));
        //sem_post(&sem);
    }
    printf("mainloop: X.\n");
}

int main()
{
    printf("main: E...\n");
    sem_init(&sem, 0, 1);
    
    pthread_t tid_working;
    volatile int data_thread = 0;

    printf("start working thread...\n");
    int ret = pthread_create(&tid_working, 0, threadfunc_working, (void *)&data_thread);
    if (ret != 0)
    {
        printf("create working thread failed, error = %d\n", ret);
        return -1;
    }
 
    int data_main = 0;
    enter_main_loop((void *)&data_main);

    pthread_join(tid_working, 0);
    printf("working thread stopped.\n");
    
    sem_destroy(&sem);
    printf("main: X.\n");
}
