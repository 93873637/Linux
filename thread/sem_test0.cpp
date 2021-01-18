#include<pthread.h>
#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<memory.h>
#include<semaphore.h>

typedef struct{
    int wrok;//线程间通信 标志位 信号灯
    int data;//数据
    int num;
    pthread_t rd;//读线程
    pthread_t wr;//写线程
    sem_t sem;//控制信号量
}PC;


void* th_read(void* p){
    PC* pc=(PC*)p;
    while(1){
    sem_wait(&(pc->sem));
        while(!(pc->wrok)){//写标志，如果写线程没写数据则循环检测是否写好
            sem_post(&(pc->sem));
            usleep(50000);
            sem_wait(&(pc->sem));
        }
    printf("read data %d\n",pc->data);
    pc->data=0;//读完数据删除
    pc->wrok=0;//把写标志置位，写线程可以写数据
    sem_post(&(pc->sem));
    usleep(50000);
    }
}

void* th_write(void*  p){
    PC* pc=(PC*)p;
    while(1){
    sem_wait(&(pc->sem));
        pc->wrok=0; //读线程以读完数据，可以写数据了  
        sleep(1);//模拟去后台获取数据
        pc->data=pc->num++;//写数据
        pc->wrok=1;//通知读线程可以读了
    printf("write %d\n",pc->data);
    sem_post(&(pc->sem));
    usleep(50000);
    }
}

void initSem(sem_t* sem){
    sem_init(sem,0,1);
}

void destroySem(sem_t* sem){
    sem_destroy(sem);
}

void startPthread(pthread_t* r,pthread_t* w,PC* p){
    pthread_create(r,0,th_read,(void*)p);
    pthread_create(w,0,th_write,(void*)p);
    pthread_join(*r,0);
    pthread_join(*w,0);
}

int main(){
   PC pc;
   memset(&pc,0,sizeof(PC));
   initSem(&(pc.sem));
   startPthread(&(pc.rd),&(pc.wr),&pc);
   destroySem(&(pc.sem));
}
