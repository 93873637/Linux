#include <memory.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define PERIOD_US    1000000

sem_t sem;

typedef struct
{
    int num;
} PC;

void *th_read(void *p)
{
    printf("th_read: E...\n");

    PC *pc = (PC *)p;
    while (1)
    {
        sem_wait(&sem);
        printf("th_read: read %d\n", pc->num);
        sem_post(&sem);
        usleep(PERIOD_US);
    }

    printf("th_read: X.\n");
}

void *th_write(void *p)
{
    printf("th_write: E...\n");

    PC *pc = (PC *)p;
    while (1)
    {
        sem_post(&sem);
        pc->num ++;
        printf("th_write: write %d\n", pc->num);
        usleep(PERIOD_US);
    }

    printf("th_write: X.\n");
}

int main()
{
    printf("main: E...\n");

    pthread_t tr; //读线程
    pthread_t tw; //写线程

    PC pc;
    memset(&pc, 0, sizeof(PC));

    sem_init(&sem, 0, 0);

    pthread_create(&tr, 0, th_read, (void *)&pc);
    pthread_create(&tw, 0, th_write, (void *)&pc);
    pthread_join(tr, 0);
    pthread_join(tw, 0);

    sem_destroy(&sem);
    printf("main: X.\n");
}
