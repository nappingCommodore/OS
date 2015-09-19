//Sleeping barber problem

#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>
#include<time.h>

#define CN 5

time_t Time;
sem_t barb_sem,cust_sem,mutex;
int count = 0;
int i = 0;

void *Barber(void*);
void *Customer(void*);

int main(){
	pthread_t b_thread;
	pthread_t c_thread;
	Time = time(NULL)+100000;
	
	sem_init(&mutex,0,1);
	sem_init(&barb_sem,0,1);
	sem_init(&cust_sem,0,0);
	
	int a = pthread_create(&b_thread,NULL,Barber,NULL);
	if(a==-1)
		printf("Error in Barber. He is not going to shop.\n");
	a = pthread_create(&c_thread,NULL,Customer,NULL);
	if(a==-1)
		printf("Error in customers . They think Barber is not good.\n");
		
	int x;
	sem_getvalue(&cust_sem,&x);
	
	//printf("Initial cust_sem = %d\n",x);
	
	fflush(stdout);
	pthread_join(b_thread,NULL);
	pthread_join(c_thread,NULL);
	return 0;
}

void *Barber(void* arg){

	while(time(NULL)<Time){
		i++;
		if(count>0){
			sem_wait(&cust_sem);
			sem_wait(&mutex);
			count--;
			printf("Customer %d is receiving haircut....\n",count);
			printf("Customer %d is leaving the shop.....\n\n\n",count);
			fflush(stdout);
			sem_post(&mutex);
			sem_post(&barb_sem);
			srand(time(NULL));
			sleep((((double)rand())/((double)RAND_MAX))*5);
		}
		else{
			
			printf("No customer....,Barber is going to sleep!!!!\n\n\n");
			fflush(stdout);
			//printf("%d\n",count);
			srand(time(NULL));
			sleep((((double)rand())/((double)RAND_MAX))*5);
		}
		
	
	}
	
}


void *Customer(void *arg){

	while(time(NULL)<Time){
		i++;
		sem_wait(&mutex);
		if(count<CN){
			printf("Customer %d is going to wait for his turn....\n\n\n",count);
			fflush(stdout);
			count++;
			sem_post(&mutex);
			sem_post(&cust_sem);
			int x;
			sem_getvalue(&cust_sem,&x);
			//printf("cust_sem val = %d\n",x);
			fflush(stdout);
			srand(time(NULL));
			sleep((((double)rand())/((double)RAND_MAX))*5);

		}
		else{
			sem_post(&mutex);
			i++;
			printf("Customer %d did not get any seat. He is leaving....\n\n\n",count);
			fflush(stdout);
			srand(time(NULL));
			sleep((((double)rand())/((double)RAND_MAX))*5);

		}
		
		
	}
}
