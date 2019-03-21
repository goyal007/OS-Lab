// one producer and one consumer

#include<stdio.h>
#include<semaphore.h>
#include<stdlib.h>
#include<pthread.h>
#define N 10000  // size of the queue(queue of product)
int queue[N];
int tail,head;
sem_t not_full, not_empty;
void * producer(void * args)
{
	int item;
	do
	{
		item=rand()%100;
		printf("produced product number: %d\n",item);
		sem_wait(&not_full);/* wait for empty place in queue */
		queue[tail]=item; 
		tail=(tail+1)%N;
		sem_post(&not_empty);  /* tells there are empty in queue*/
		sleep(rand()%4); /* rest after product produce */
	}while(item);
}
void * consumer(void * args)
{
	int item;
	do
	{
		sem_wait(&not_empty);/* wait for products in queue */
		item=queue[head]; 
		head=(head+1) % N;
		printf("\t\t\t\tconsumed product number: %d\n", item);
		sem_post(&not_full);/* tells inc in  free spaces in the queue*/
		sleep(rand()%4);/* consume the product */
	}while(item);
}
int main()
{
	pthread_t prod,cons;
	sem_init(&not_full,0,N); //middle zero means sharing b/w thread
	sem_init(&not_empty,0,0);
	
	pthread_create(&prod,NULL,&producer,NULL);
	pthread_create(&cons,NULL,&consumer,NULL);
	
	pthread_join(prod,NULL);
	pthread_join(cons,NULL);
	
	sem_destroy(&not_full);
	sem_destroy(&not_empty);
	return 0;
}
