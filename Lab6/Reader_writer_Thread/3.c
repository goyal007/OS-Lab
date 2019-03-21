#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<semaphore.h>
#include<pthread.h>
#define n1 10
#define n2 10
int shared_data=0;
int read_count=0;
sem_t resourceAccess,readCountAccess,serviceQueue;
void *writer(void *args)
{
	int w_id;
  	w_id= (int)args;
  	sem_wait(&serviceQueue);
  	sem_wait(&resourceAccess);

  	sem_post(&serviceQueue);

  	shared_data+=2;
  	printf("value written  by writer%d : %d\n",w_id,shared_data);
  	sleep(1);

  	sem_post(&resourceAccess);
  //	sleep(rand()%4);
}
void * reader(void * args)
{
	int r_id;
  	r_id= (int)args;

  	sem_wait(&serviceQueue);
  	sem_wait(&readCountAccess);
  	if(read_count==0)
  		sem_wait(&resourceAccess);
  	read_count++;
  	sem_post(&serviceQueue);
  	sem_post(&readCountAccess);

  	printf("value read by reader%d : %d\n",r_id,shared_data);
  	sleep(1);
  	sem_wait(&readCountAccess);
  	read_count--;
  	if(read_count==0)
  		sem_post(&resourceAccess);
  	sem_post(&readCountAccess);
  	//sleep(rand()%4);
}
int main()
{
	pthread_t r[n1],w[n2];
	int i;
	sem_init(&resourceAccess,0,1);
	sem_init(&readCountAccess,0,1);
	sem_init(&serviceQueue,0,1);
	for(i=0;i<n1;i++)
	{
		pthread_create(&w[i],NULL,&writer,(void *)i+1);
		pthread_create(&r[i],NULL,&reader,(void *)i+1);

	}
	for(i=0;i<n1;i++)
	{
		pthread_join(w[i],NULL);
		pthread_join(r[i],NULL);
	}

	sem_destroy(&resourceAccess);
	sem_destroy(&readCountAccess);
	sem_destroy(&serviceQueue);
	return 0;
}
