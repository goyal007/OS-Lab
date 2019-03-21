// n producer and n consumer
#include<stdio.h>
#include<time.h>
#include<semaphore.h>
#include<stdlib.h>
#include<pthread.h>
#define N 100000  // size of the queue(queue of product)
int queue[N];
int tail,head;
sem_t not_full, not_empty;
pthread_mutex_t m1=PTHREAD_MUTEX_INITIALIZER;
//pthread_mutex_t m2=PTHREAD_MUTEX_INITIALIZER;
//pthread_mutex_init (&m,NULL);
void * producer(void * args)
{
	
	int p_id=(int)args;
	int n,i;
	int item;
	do
	{	
		sem_wait(&not_full);/* wait for empty place in queue */
		
		pthread_mutex_lock(&m1);
		for(i=0;i<10000000;i++)
		{
			
		}
		item=rand()%100;
		printf("produced product number: %d by producer %d\n",item,p_id);
		//printf("tail: %d head: %d\n",tail,head);
		
//		sem_getvalue(&not_full,&n);
//		printf("sem val:%d\n",n);
		queue[tail]=item; 
		tail=(tail+1)%N;
		
		pthread_mutex_unlock(&m1);
		sem_post(&not_empty);  /* tells there are empty in queue*/
		
		
		sleep(rand()%5); /* rest after product produce */
	}while(item);
}
void * consumer(void * args)
{
	int c_id=(int)args;
	int item;
	do
	{
		sem_wait(&not_empty);/* wait for products in queue */
		
		pthread_mutex_lock(&m1);
		
		item=queue[head]; 
		head=(head+1) % N;
		printf("\t\t\t\tconsumed product number: %d by consumer %d\n",item,c_id);
		pthread_mutex_unlock(&m1);
		sem_post(&not_full);/* tells inc in  free spaces in the queue*/
		
		
		
		sleep(rand()%5);/* consume the product */
	}while(item);
}
int main()
{
	srand(time(0));
	int i;
	pthread_t prod[10],cons[10];
	sem_init(&not_full,0,N); //middle zero means sharing b/w thread
	sem_init(&not_empty,0,0);
	
	
//	int a,b;
//	sem_getvalue(&not_full,&a);
//	sem_getvalue(&not_empty,&b);
//	printf("a:%d  b%d  \n",a,b);
	
	for(i=0;i<10;i++)
		pthread_create(&prod[i],NULL,&producer,(void *)i+1);
	for(i=0;i<10;i++)
		pthread_create(&cons[i],NULL,&consumer,(void *)i+1);
	for(i=0;i<10;i++)
		pthread_join(prod[i],NULL);
	for(i=0;i<10;i++)
		pthread_join(cons[i],NULL);
	
		
	
	sem_destroy(&not_full);
	sem_destroy(&not_empty);
	return 0;
}
