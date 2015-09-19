//Reader writer problem

#include<stdio.h>
#include<pthread.h>
#include<semaphore.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<stdint.h>

#define RD 10
#define WR 10

sem_t rd_sem,wr_sem,mutex;
char ch[10];

void *Reader(void* arg){
	//int x = *((int*)arg);
	int x = (intptr_t) arg;
	sem_wait(&mutex);
	int t;
	sem_getvalue(&wr_sem,&t);
	if(t == 0)
		printf("Reader %d is waiting to read...\n\n\n",x);
	fflush(stdout);
	sem_wait(&wr_sem);
	sem_post(&mutex);
	printf("Reader %d is reading .... \n",x);
	fflush(stdout);
	if(x%2==0)
		sleep(3);
	//sem_post(&mutex);
	printf("Reader %d finished reading .. \n\n\n",x);
	fflush(stdout);
	sem_post(&wr_sem);
	return NULL;
}

void *Writer(void* arg){
	//int x = *((int*)arg);
	int x = (intptr_t) arg,t;
	sem_getvalue(&wr_sem,&t);
	if(t == 0)
		printf("Writer %d is waiting to write....\n\n\n",x);
	fflush(stdout);
	sem_wait(&wr_sem);
	//sem_wait(&mutex);
	printf("Writer %d is writing ....\n",x);
	fflush(stdout);
	if(x%2 == 0)
		sleep(5);
	
	printf("Writer %d finished writing ... \n\n\n",x);
	fflush(stdout);
	
	//sem_post(&mutex);
	sem_post(&wr_sem);
	return NULL;
}


int main(){
	pthread_t rd_thread,wr_thread;
	int i;
	
	sem_init(&mutex,0,1);
	sem_init(&wr_sem,0,1);
	
	for(i=1;i<=RD;i++){
		pthread_create(&rd_thread,NULL,Reader,(void*)(intptr_t)i);
		sleep(3);
		pthread_create(&wr_thread,NULL,Writer,(void*)(intptr_t)i);
	}
	
	for(i=1;i<=RD;i++){
		pthread_join(rd_thread,NULL);
		pthread_join(wr_thread,NULL);
	}
	
	return 0;
}

