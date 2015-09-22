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
int read_count = 0;

void *Reader(void* arg){
	int x = (intptr_t) arg;
	sem_wait(&mutex);
	
	fflush(stdout);
	read_count += 1;
	if(read_count == 1)
		sem_wait(&wr_sem);
		
	sem_post(&mutex);
	
	printf("Reader %d is reading .... \n",x);
	fflush(stdout);
	
	sem_wait(&mutex);
	
	read_count -=1;
	
	printf("Reader %d finished reading .. \n\n\n",x);
	fflush(stdout);
	if(read_count == 0)
		sem_post(&wr_sem);
		
	sem_post(&mutex);
	return NULL;
}

void *Writer(void* arg){
	int x = (intptr_t) arg,t;

	fflush(stdout);
	
	sem_wait(&wr_sem);

	printf("Writer %d is writing ....\n",x);
	fflush(stdout);

	
	printf("Writer %d finished writing ... \n\n\n",x);
	fflush(stdout);
	
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
		pthread_create(&wr_thread,NULL,Writer,(void*)(intptr_t)i);
	}
	
	for(i=1;i<=RD;i++){
		pthread_join(rd_thread,NULL);
		pthread_join(wr_thread,NULL);
	}
	
	return 0;
}

