// n producer and 1 consumer
#include<stdio.h>
#include<time.h>
#include<semaphore.h>
#include<stdlib.h>
#include<pthread.h>
#define N 10000  // size of the queue(queue of product)
int queue[N];
int tail,head;
sem_t not_full, not_empty;
pthread_mutex_t m=PTHREAD_MUTEX_INITIALIZER;
//pthread_mutex_init (&m,NULL);
void * producer(void * args)
{
	srand(time(0));
	int p_id=(int)args;
	int n;
	int item;
	do
	{	
		sem_wait(&not_full);/* wait for empty place in queue */
		pthread_mutex_lock(&m);
		item=rand()%100;
		printf("produced product number: %d by producer %d\n",item,p_id);
//		sem_getvalue(&not_full,&n);
//		printf("sem val:%d\n",n);
	
		queue[tail]=item; 
		tail=(tail+1)%N;
		pthread_mutex_unlock(&m);
		sem_post(&not_empty);  /* tells there are empty in queue*/
		
		sleep(rand()%4); /* rest after product produce */
	}while(item);
}
void * consumer(void * args)
{
	int c_id=(int)args;
	int item;
	do
	{
		sem_wait(&not_empty);/* wait for products in queue */
		item=queue[head]; 
		head=(head+1) % N;
		printf("\t\t\t\tconsumed product number: %d by consumer %d\n",item,c_id);
		sem_post(&not_full);/* tells inc in  free spaces in the queue*/
		
		sleep(rand()%4);/* consume the product */
	}while(item);
}
int main()
{
	
	int i;
	pthread_t prod[10],cons;
	sem_init(&not_full,0,N); //middle zero means sharing b/w thread
	sem_init(&not_empty,0,0);
	
	
//	int a,b;
//	sem_getvalue(&not_full,&a);
//	sem_getvalue(&not_empty,&b);
//	printf("a:%d  b%d  \n",a,b);
	
	

	for(i=0;i<10;i++)
		pthread_create(&prod[i],NULL,&producer,(void *)i+1);
	pthread_create(&cons,NULL,&consumer,(void *)1);
		
	for(i=0;i<10;i++)
		pthread_join(prod[i],NULL);
	pthread_join(cons,NULL);
	
		
	
	sem_destroy(&not_full);
	sem_destroy(&not_empty);
	return 0;
}
