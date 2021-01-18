#include <memory.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

sem_t sem;

void *threadfunc_working(void *p)
{
    printf("threadfunc_working: E...\n");

    int *data = (int *)p;
    while (1)
    {
        (*data)++;
        
        int value = 0;
        sem_getvalue(&sem, &value);
        printf("threadfunc_working: sem = %d, data = %d\n", value, *data);
        if (value > 0)
        {
            break;  // stop work
        }
        else
        {
            usleep(10000);  // cont work
        }
    }

    printf("threadfunc_working: X.\n");
}

int main()
{
    printf("main: E...\n");
    
    sem_init(&sem, 0, 0);
    
    pthread_t tid_working; //读线程
    int data = 0;
    printf("start working thread...\n");
    int ret = pthread_create(&tid_working, 0, threadfunc_working, (void *)&data);
    if (ret != 0)
    {
        printf("create working thread failed, error = %d\n", ret);
        return;
    }
    
    usleep(200000);

    printf("stop working thread...\n");
    sem_post(&sem);
    pthread_join(tid_working, 0);
    printf("working thread stopped.\n");
    
    sem_destroy(&sem);
    printf("main: X.\n");
}
